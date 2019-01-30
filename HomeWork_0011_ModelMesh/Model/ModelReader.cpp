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

					//part->material->SetDiffuseMap(Textures + L"Red.png");
					//part->material = new Material(Textures + L"Red.png");
					break;
				}


			}
			
			/*if (part->materialName == L"tank_geo")
				part->material = new Material(Textures + L"Red.png");*/


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


void Model::SetbindingBox()
{
	//shader = new Shader(Shaders + L"011_BoundingBox.hlsl");
	CubeMaterial = new Material(Shaders + L"011_BoundingBox.hlsl");
	CubeMaterial->SetDiffuseMap(Textures + L"Red.png");

	// 메시 
	for (ModelMesh* mesh : meshes)
	{
		// 메시 파츠
		for (ModelMeshPart* meshpart : mesh->meshParts)
		{
			// 구조체 하나 생성
			CubeStruct* cubeStruct = new CubeStruct();

			// 이름
			cubeStruct->PartsName = mesh->Name();
			

			//
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



			cubeStruct->asdf = matWorld;


			D3DXVECTOR3 Min, Max;
			SetMinMax(Min, Max, meshpart); // 최대최소 계산

			MeshCubeParts* cubeParts = new MeshCubeParts(CubeMaterial, Min, Max); // 최대 최소완성
			cubeParts->Matrix(matWorld);


			D3DXVECTOR3 w;
			w.x = Max.x - Min.x;
			w.y = Max.y - Min.y;
			w.z = Max.z - Min.z;

			//D3DXVec3TransformNormal(&w, &w, &matWorld);
			D3DXVec3TransformCoord(&w, &w, &matWorld);

			MeshCube* cube = new MeshCube(CubeMaterial, Max.x - Min.x, Max.y - Min.y, Max.z - Min.z);
			cubeParts->Matrix(matWorld);






			cubeStruct->MinV = Min;
			cubeStruct->MaxV = Max;

			cubeStruct->Scale = D3DXVECTOR3(1,1,1);
			cubeStruct->Position = D3DXVECTOR3(0, 0, 0);
			cubeStruct->Rotation = D3DXVECTOR3(0, 0, 0);


			cubeStruct->meshCube = cube;
			cubeStruct->meshCubeParts = cubeParts;
			cubeStruct->Number = bindingBox.size();

			bindingBox.push_back(cubeStruct); // 구조체담음
	


		}
	}
}

void Model::SetMinMax(D3DXVECTOR3& min, D3DXVECTOR3& max, class ModelMeshPart* meshParts)
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

