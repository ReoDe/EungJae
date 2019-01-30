#include "stdafx.h"
#include "Exporter.h"
#include "Type.h"
#include "Utility.h"
#include "../Utilities/Xml.h"
#include "../Utilities/BinaryFile.h"

Fbx::Exporter::Exporter(wstring file)
{
	manager = FbxManager::Create();
	scene = FbxScene::Create(manager, "");

	ios = FbxIOSettings::Create(manager, IOSROOT);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	manager->SetIOSettings(ios);


	importer = FbxImporter::Create(manager, "");

	string loadFile = String::ToString(file);
	bool b = importer->Initialize(loadFile.c_str(), -1, ios);
	assert(b == true);
		
	b = importer->Import(scene);
	assert(b == true);

	converter = new FbxGeometryConverter(manager);
}

Fbx::Exporter::~Exporter()
{
	SAFE_DELETE(converter);

	ios->Destroy();
	importer->Destroy();
	scene->Destroy();
	manager->Destroy();
}

void Fbx::Exporter::ExportMaterial(wstring saveFolder, wstring fileName)
{
	ReadMaterial();

	WriteMaterial(saveFolder, fileName);
}

void Fbx::Exporter::ExportMesh(wstring saveFolder, wstring fileName)
{
	ReadBoneData(scene->GetRootNode(), -1, -1);

	WriteMeshData(saveFolder, fileName);
}

void Fbx::Exporter::ReadMaterial()
{
	int count = scene->GetMaterialCount();

	for (int i = 0; i < count; i++)
	{
		FbxSurfaceMaterial* fbxMaterial = scene->GetMaterial(i);

		FbxMaterial* material = new FbxMaterial();
		material->Name = fbxMaterial->GetName();

		if (fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) == true)
		{
			FbxSurfaceLambert* lambert = (FbxSurfaceLambert *)fbxMaterial;
			material->Diffuse = Utility::ToColor(lambert->Diffuse, lambert->DiffuseFactor);
		}

		if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId) == true)
		{
			FbxSurfacePhong* phong = (FbxSurfacePhong *)fbxMaterial;
			material->Specular = Utility::ToColor(phong->Specular, phong->SpecularFactor);
			material->SpecularExp = (float)phong->Shininess;
		}


		FbxProperty prop;

		prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		material->DiffuseFile = Utility::GetTextureFile(prop);

		prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
		material->SpecularFile = Utility::GetTextureFile(prop);

		prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
		material->NormalFile = Utility::GetTextureFile(prop);

		materials.push_back(material);
	}
}

void Fbx::Exporter::WriteMaterial(wstring saveFolder, wstring fileName)
{
	Path::CreateFolders(saveFolder);

	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLDeclaration *decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);


	for (FbxMaterial* material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);


		Xml::XMLElement* element = NULL;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());
		node->LinkEndChild(element);


		element = document->NewElement("DiffuseFile");
		CopyTextureFile(material->DiffuseFile, saveFolder);
		element->SetText(material->DiffuseFile.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		CopyTextureFile(material->SpecularFile, saveFolder);
		element->SetText(material->SpecularFile.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		CopyTextureFile(material->NormalFile, saveFolder);
		element->SetText(material->NormalFile.c_str());
		node->LinkEndChild(element);

		
		element = document->NewElement("Diffuse");
		node->LinkEndChild(element);

		WriteXmlColor(document, element, material->Diffuse);

		element = document->NewElement("Specular");
		node->LinkEndChild(element);

		WriteXmlColor(document, element, material->Specular);

		element = document->NewElement("SpecularExp");
		element->SetText(material->SpecularExp);
		node->LinkEndChild(element);

		SAFE_DELETE(material);
	}

	string file = String::ToString(saveFolder + fileName);
	document->SaveFile(file.c_str());
}

void Fbx::Exporter::ReadBoneData(FbxNode * node, int index, int parent)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute != NULL)
	{
		FbxNodeAttribute::EType nodeType = attribute->GetAttributeType();

		bool b = false;
		b |= (nodeType == FbxNodeAttribute::eSkeleton);
		b |= (nodeType == FbxNodeAttribute::eMesh);
		b |= (nodeType == FbxNodeAttribute::eNull);
		b |= (nodeType == FbxNodeAttribute::eMarker);

		if (b == true)
		{
			FbxBoneData* bone = new FbxBoneData();
			bone->Index = index;
			bone->Parent = parent;
			bone->Name = node->GetName();
			bone->LocalTransform = Utility::ToMatrix(node->EvaluateLocalTransform());
			bone->GlobalTransform = Utility::ToMatrix(node->EvaluateGlobalTransform());
			boneDatas.push_back(bone);

			if (nodeType == FbxNodeAttribute::eMesh)
			{
				converter->Triangulate(attribute, true, true);

				ReadMeshData(node, index);
			}
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
		ReadBoneData(node->GetChild(i), boneDatas.size(), index);
}

void Fbx::Exporter::ReadMeshData(FbxNode * node, int parentBone)
{
	FbxMesh* mesh = node->GetMesh();

	vector<FbxVertex *> vertices;
	for (int p = 0; p < mesh->GetPolygonCount(); p++)
	{
		int vertexInPolygon = mesh->GetPolygonSize(p);
		assert(vertexInPolygon == 3);

		for (int vi = vertexInPolygon - 1; vi >= 0; vi--)
		{
			FbxVertex* vertex = new FbxVertex();

			int cpIndex = mesh->GetPolygonVertex(p, vi);
			vertex->ControlPoint = cpIndex;


			D3DXVECTOR3 temp;

			FbxVector4 position = mesh->GetControlPointAt(cpIndex);
			temp = Utility::ToVector3(position);
			D3DXVec3TransformCoord(&vertex->Vertex.Position, &temp, &Utility::Negative());

			
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(p, vi, normal);
			normal.Normalize();

			temp = Utility::ToVector3(normal);
			D3DXVec3TransformNormal(&vertex->Vertex.Normal, &temp, &Utility::Negative());
			

			vertex->MaterialName = Utility::GetMaterialName(mesh, p, cpIndex);

			int uvIndex = mesh->GetTextureUVIndex(p, vi);
			vertex->Vertex.Uv = Utility::GetUv(mesh, cpIndex, uvIndex);

			vertices.push_back(vertex);
		}//for(vi)
	}//for(p)

	FbxMeshData* meshData = new FbxMeshData();
	meshData->Name = node->GetName();
	meshData->ParentBone = parentBone;
	meshData->Vertices = vertices;
	meshData->Mesh = mesh;
	meshDatas.push_back(meshData);


	for (int i = 0; i < scene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* material = scene->GetMaterial(i);
		string materialName = material->GetName();

		vector<FbxVertex *> gather;
		for (FbxVertex* temp : meshData->Vertices)
		{
			if (temp->MaterialName == materialName)
				gather.push_back(temp);
		}
		if (gather.size() < 1) continue;


		FbxMeshPartData* meshPart = new FbxMeshPartData();
		meshPart->MaterialName = materialName;

		for (FbxVertex* temp : gather)
		{
			VertexTextureNormal vertex;
			vertex = temp->Vertex;

			meshPart->Vertices.push_back(vertex);
			meshPart->Indices.push_back(meshPart->Indices.size());
		}
		meshData->MeshParts.push_back(meshPart);
	}
}

void Fbx::Exporter::WriteMeshData(wstring saveFolder, wstring fileName)
{
	Path::CreateFolders(saveFolder);

	BinaryWriter* w = new BinaryWriter();
	w->Open(saveFolder + fileName);

	w->UInt(boneDatas.size());
	for (FbxBoneData* bone : boneDatas)
	{
		w->Int(bone->Index);
		w->String(bone->Name);

		w->Int(bone->Parent);

		w->Matrix(bone->LocalTransform);
		w->Matrix(bone->GlobalTransform);

		SAFE_DELETE(bone);
	}


	w->UInt(meshDatas.size());
	for (FbxMeshData* meshData : meshDatas)
	{
		w->String(meshData->Name);
		w->Int(meshData->ParentBone);

		w->UInt(meshData->MeshParts.size());
		for (FbxMeshPartData* part : meshData->MeshParts)
		{
			w->String(part->MaterialName);

			w->UInt(part->Vertices.size());
			w->Byte(&part->Vertices[0], sizeof(VertexTextureNormal) * part->Vertices.size());

			w->UInt(part->Indices.size());
			w->Byte(&part->Indices[0], sizeof(UINT) * part->Indices.size());

			SAFE_DELETE(part);
		}

		SAFE_DELETE(meshData);
	}

	w->Close();
	SAFE_DELETE(w);
}

void Fbx::Exporter::CopyTextureFile(OUT string & textureFile, wstring & saveFolder)
{
	if (textureFile.length() < 1)
		return;

	
	wstring file = String::ToWString(textureFile);
	wstring fileName = Path::GetFileName(file);

	if (Path::ExistFile(textureFile) == true)
		CopyFile(file.c_str(), (saveFolder + fileName).c_str(), FALSE);

	textureFile = String::ToString(fileName);
}

void Fbx::Exporter::WriteXmlColor(Xml::XMLDocument * document, Xml::XMLElement * element, D3DXCOLOR & color)
{
	Xml::XMLElement* r = document->NewElement("R");
	r->SetText(color.r);
	element->LinkEndChild(r);

	Xml::XMLElement* g = document->NewElement("G");
	g->SetText(color.g);
	element->LinkEndChild(g);

	Xml::XMLElement* b = document->NewElement("B");
	b->SetText(color.b);
	element->LinkEndChild(b);

	Xml::XMLElement* a = document->NewElement("A");
	a->SetText(color.a);
	element->LinkEndChild(a);
}
