#include "framework.h"
#include "Model.h"
#include "ModelBone.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "../Utilities/Xml.h"
#include "../Utilities/BinaryFile.h"


void Model::ReadMaterial(wstring file)
{
	Models::LoadMaterial(file, &materials);
}

void Model::ReadMesh(wstring file)
{
	Models::LoadMesh(file, &bones, &meshes);

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

	SAFE_DELETE(document);

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

		//VertexData
		{
			UINT count = r->UInt();

			vector<VertexTextureNormalBlend> vertices;
			vertices.assign(count, VertexTextureNormalBlend());

			void* ptr = (void *)&(vertices[0]);
			r->Byte(&ptr, sizeof(VertexTextureNormalBlend) * count);


			mesh->vertices = new VertexTextureNormalBlend[count];
			mesh->vertexCount = count;
			copy
			(
				vertices.begin(), vertices.end(),
				stdext::checked_array_iterator<VertexTextureNormalBlend *>(mesh->vertices, count)
			);
		}


		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			meshPart->materialName = String::ToWString(r->String());

			meshPart->startVertex = r->UInt();
			meshPart->vertexCount = r->UInt();

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


//////////////////////////////////////////


void Model::SetBindingBox()
{
	for (ModelMesh* mesh : meshes)
	{
		CubeStruct* cubeStruct = new CubeStruct();

		// 이름
		cubeStruct->meshName = mesh->Name();

		// 본
		ModelBone* parentBone = mesh->ParentBone()->parent;

		D3DXMATRIX matWorld, matParent, matLocal;

		if (parentBone)
		{
			matParent = mesh->ParentBone()->parent->Global();
		}
		else
		{
			D3DXMatrixIdentity(&matParent);
		}

		matLocal = mesh->ParentBone()->Local(); // 로컬

		matWorld = matLocal * matParent;

		D3DXVECTOR3 Min, Max;
		SetMinMax(Min, Max, mesh); // 최대최소 계산

		MeshCubeMinMax* cubeMinMax = new MeshCubeMinMax(NULL, Max, Min);
		cubeMinMax->Color(D3DXCOLOR(0,1,0,1));
		cubeMinMax->Matrix(matWorld);

		D3DXVECTOR3 w;
		w.x = Max.x - Min.x;
		w.y = Max.y - Min.y;
		w.z = Max.z - Min.z;
		
		D3DXVec3TransformCoord(&w, &w, &matWorld);

		cubeStruct->MaxV = Max;
		cubeStruct->MinV = Min;

		cubeStruct->Scale = D3DXVECTOR3(1, 1, 1);
		cubeStruct->Position = D3DXVECTOR3(0, 0, 0);
		cubeStruct->Rotation = D3DXVECTOR3(0, 0, 0);

		cubeStruct->meshCubeMinMax = cubeMinMax;
		cubeStruct->Number = bindingBox.size();


		bindingBox.push_back(cubeStruct);
	}
}

void Model::SetMinMax(D3DXVECTOR3 & min, D3DXVECTOR3 & max, ModelMesh * Meshes)
{


	
	VertexTextureNormalBlend* vertices = Meshes->Vertices();

	min = vertices[0].Position;
	max = vertices[1].Position;

	for (UINT i = 0; i < Meshes->VertexCount(); i++)
	{
		D3DXVECTOR3 MinValue;
		D3DXVECTOR3 MaxValue;

		

		MinValue.x = vertices[i].Position.x;
		MinValue.y = vertices[i].Position.y;
		MinValue.z = vertices[i].Position.z;
								
		MaxValue.x = vertices[i].Position.x;
		MaxValue.y = vertices[i].Position.y;
		MaxValue.z = vertices[i].Position.z;

		// 최대최소 비교
		if (MinValue.x < min.x)	min.x = MinValue.x;
		if (MinValue.y < min.y)	min.y = MinValue.y;
		if (MinValue.z < min.z)	min.z = MinValue.z;

		if (MaxValue.x > max.x)	max.x = MaxValue.x;
		if (MaxValue.y > max.y)	max.y = MaxValue.y;
		if (MaxValue.z > max.z)	max.z = MaxValue.z;
	}

	
}