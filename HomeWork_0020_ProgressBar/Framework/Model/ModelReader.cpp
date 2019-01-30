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

		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			meshPart->materialName = String::ToWString(r->String());

			//VertexData
			{
				UINT temp = r->UInt();
				meshPart->vertices.assign(temp, VertexTextureNormalBlend());

				void* ptr = (void *)&(meshPart->vertices[0]);
				r->Byte(&ptr, sizeof(VertexTextureNormalBlend) * temp);
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
