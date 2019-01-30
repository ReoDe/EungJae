#include "stdafx.h"
#include "Model.h"
#include "ModelBone.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "../Utilities/Xml.h"
#include "../Utilities/BinaryFile.h"


void Model::ReadMaterial(wstring folder, wstring file)
{
	Models::LoadMaterial(folder + file, &materials);
}

void Model::ReadMesh(wstring folder, wstring file)
{
	Models::LoadMesh(folder + file, &bones, &meshes);

	BindingBone();
	BindingMesh();
}



void Model::BindingBone()
{
	this->root = bones[0];
	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->childs.push_back(bone);
		}
		else
			bone->parent = NULL;
	}
}

void Model::BindingMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->parentBoneIndex == bone->index)
			{
				mesh->parentBone = bone;

				break;
			}
		}

		for (ModelMeshPart* part : mesh->meshParts)
		{
			for (Material* material : materials)
			{
				if (part->materialName == material->Name())
				{
					part->material = material;

					break;
				}
			}
		}

		mesh->Binding();
	}
}

///////////////////////////////////////////////////////////////////////////////

map<wstring, vector<Material *>> Models::materialMap;
void Models::LoadMaterial(wstring file, vector<Material*>* materials)
{
	if (materialMap.count(file) < 1)
		ReadMaterialData(file);

	for (Material* material : materialMap[file])
	{
		Material* temp = NULL;
		material->Copy(&temp);

		materials->push_back(temp);
	}
}

void Models::ReadMaterialData(wstring file)
{
	vector<Material *> materials;

	Xml::XMLDocument* document = new Xml::XMLDocument();

	wstring tempFile = file;
	Xml::XMLError error = document->LoadFile(String::ToString(tempFile).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* matNode = root->FirstChildElement();


	do
	{
		Xml::XMLElement* node = NULL;

		Material* material = new Material();

		node = matNode->FirstChildElement();
		material->Name(String::ToWString(node->GetText()));


		wstring directory = Path::GetDirectoryName(tempFile);

		node = node->NextSiblingElement();
		wstring diffuseTexture = String::ToWString(node->GetText());
		if (diffuseTexture.length() > 0)
			material->SetDiffuseMap(directory + diffuseTexture);

		node = node->NextSiblingElement();
		wstring specularTexture = String::ToWString(node->GetText());
		if (specularTexture.length() > 0)
			material->SetSpecularMap(directory + specularTexture);

		node = node->NextSiblingElement();
		wstring normalTexture = String::ToWString(node->GetText());
		if (normalTexture.length() > 0)
			material->SetNormalMap(directory + normalTexture);


		D3DXCOLOR dxColor;
		Xml::XMLElement* color;

		//DiffuseColor
		node = node->NextSiblingElement();
		color = node->FirstChildElement();
		dxColor.r = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.g = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.b = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.a = color->FloatText();
		material->SetDiffuse(dxColor);


		//SpecularColor
		node = node->NextSiblingElement();
		color = node->FirstChildElement();
		dxColor.r = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.g = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.b = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.a = color->FloatText();
		material->SetSpecular(dxColor);


		node = node->NextSiblingElement();
		material->SetShininess(node->FloatText());

		materials.push_back(material);

		matNode = matNode->NextSiblingElement();
	} while (matNode != NULL);

	materialMap[file] = materials;
}

///////////////////////////////////////////////////////////////////////////////
map<wstring, Models::MeshData> Models::meshDataMap;
void Models::LoadMesh(wstring file, vector<ModelBone*>* bones, vector<ModelMesh*>* meshes)
{
	if (meshDataMap.count(file) < 1)
		ReadMeshData(file);


	MeshData data = meshDataMap[file];
	for (ModelBone* bone : data.Bones)
	{
		ModelBone* temp = NULL;
		bone->Copy(&temp);

		bones->push_back(temp);
	}

	for (ModelMesh* mesh : data.Meshes)
	{
		ModelMesh* temp = NULL;
		mesh->Copy(&temp);

		meshes->push_back(temp);
	}
}

void Models::ReadMeshData(wstring file)
{
	BinaryReader* r = new BinaryReader();
	r->Open(file);

	vector<ModelBone *> bones;
	vector<ModelMesh *> meshes;


	UINT count = 0;

	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = String::ToWString(r->String());

		bone->parentIndex = r->Int();

		bone->local = r->Matrix();
		bone->global = r->Matrix();

		bones.push_back(bone);
	}

	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();

		mesh->name = String::ToWString(r->String());
		mesh->parentBoneIndex = r->Int();

		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			meshPart->materialName = String::ToWString(r->String());

			//VertexData
			{
				UINT temp = r->UInt();
				meshPart->vertices.assign(temp, VertexTextureNormal());

				void* ptr = (void *)&(meshPart->vertices[0]);
				r->Byte(&ptr, sizeof(VertexTextureNormal) * temp);
			}

			//IndexData
			{
				UINT temp = r->UInt();
				meshPart->indices.assign(temp, UINT());

				void* ptr = (void *)&(meshPart->indices[0]);
				r->Byte(&ptr, sizeof(UINT) * temp);
			}

			mesh->meshParts.push_back(meshPart);
		}//for(k)

		meshes.push_back(mesh);
	}//for(i)

	r->Close();
	SAFE_DELETE(r);


	MeshData data;
	data.Bones.assign(bones.begin(), bones.end());
	data.Meshes.assign(meshes.begin(), meshes.end());

	meshDataMap[file] = data;
}


void Model::SetSphere()
{
	Material* sphereMaterial = new Material(Shaders + L"024_Lighting.fx");
	
	// 메시 
	for (ModelMesh* mesh : meshes)
	{
		// 메시 파츠
		for (ModelMeshPart* meshpart : mesh->meshParts)
		{
			// 구조체 하나 생성
			SphereStruct* sphereStruct = new SphereStruct();

			// 이름
			sphereStruct->PartsName = mesh->Name();




			ModelBone* parentBone = mesh->ParentBone()->parent; // 부모가 있다면



																// 월드     부모매트릭스, 로컬매트릭스
			D3DXMATRIX matWorld, matParent, matLocal;

			if (parentBone)
			{
				matParent = mesh->ParentBone()->parent->Global();
			}
			else
			{
				D3DXMatrixIdentity(&matParent);
			}

			matLocal = mesh->ParentBone()->Local(); // 로컬도 가져옴

			matWorld = matLocal * matParent;  // 진짜 월드는 부모의 글로벌과 로컬을 곱한것




			D3DXVECTOR3 Min, Max;
			SetMinMax(Min, Max, meshpart); // 최대최소 계산


			sphereStruct->sphereMaterial = sphereMaterial;
			//sphereStruct->sphereMaterial->SetDiffuseMap(Textures + L"Red.png");
			
			// 구한 최대최소로 반지름 만들기
			D3DXVECTOR3 MinRadius;
			
			MinRadius.x = (Max.x - Min.x)/2;
			MinRadius.y = (Max.y - Min.y)/2;
			MinRadius.z = (Max.z - Min.z)/2;


			// 가장 작은 길이의 반절이 반지름으로
			sphereStruct->Radius = min(MinRadius.x, min(MinRadius.y, MinRadius.z));

			// 그것의 반절하자
			sphereStruct->Radius = sphereStruct->Radius / 2;
			

			MeshSphere* sphere = new MeshSphere(sphereStruct->sphereMaterial, sphereStruct->Radius); // 최대 최소완성
			sphere->Matrix(matWorld);
		

			// 테스트
			MeshCubeParts* cube = new MeshCubeParts(sphereStruct->sphereMaterial, Min, Max);
	
			matWorld._11 = matWorld._11 * 0.4f;
			matWorld._22 = matWorld._22 * 0.4f;
			matWorld._33 = matWorld._33 * 0.4f;

			cube->Matrix(matWorld);


			sphereStruct->meshSphere = sphere;
			sphereStruct->meshCube = cube;
			sphereStruct->Number = spheres.size();

			spheres.push_back(sphereStruct); // 구조체담음



		}
	}
}

void Model::SetMinMax(D3DXVECTOR3 & min, D3DXVECTOR3 & max, ModelMeshPart * meshParts)
{
	// 사이즈
	//auto size = meshpart->vertices.size();

	min = meshParts->vertices[0].Position;
	max = meshParts->vertices[1].Position;



	for (VertexTextureNormal ver : meshParts->vertices)
	{
		D3DXVECTOR3 MinValue;
		D3DXVECTOR3 MaxValue;

		MinValue.x = ver.Position.x;
		MinValue.y = ver.Position.y;
		MinValue.z = ver.Position.z;

		MaxValue.x = ver.Position.x;
		MaxValue.y = ver.Position.y;
		MaxValue.z = ver.Position.z;

		// 최대최소 비교
		if (MinValue.x < min.x)	min.x = MinValue.x;
		if (MinValue.y < min.y)	min.y = MinValue.y;
		if (MinValue.z < min.z)	min.z = MinValue.z;

		if (MaxValue.x > max.x)	max.x = MaxValue.x;
		if (MaxValue.y > max.y)	max.y = MaxValue.y;
		if (MaxValue.z > max.z)	max.z = MaxValue.z;
	}

}


void Model::SetbindingBox()
{
	ModelMeshPart* asdf = meshes.at(0)->meshParts.at(0);

	SetMinMax(Min, Max, asdf);


}
