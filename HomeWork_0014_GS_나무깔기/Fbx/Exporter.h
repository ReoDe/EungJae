#pragma once

namespace Xml
{
	class XMLDocument;
	class XMLElement;
}

namespace Fbx
{
	class Exporter
	{
	public:
		Exporter(wstring file);
		~Exporter();

		void ExportMaterial(wstring saveFolder, wstring fileName);
		void ExportMesh(wstring saveFolder, wstring fileName);

	private:
		void ReadMaterial();
		void WriteMaterial(wstring saveFolder, wstring fileName);

		void ReadBoneData(FbxNode* node, int index, int parent);
		void ReadMeshData(FbxNode* node, int parentBone);
		void WriteMeshData(wstring saveFolder, wstring fileName);

		void CopyTextureFile(OUT string& textureFile, wstring& saveFolder);
		void WriteXmlColor(Xml::XMLDocument* document, Xml::XMLElement* element, D3DXCOLOR& color);
		

	private:
		FbxManager* manager;
		FbxImporter* importer;
		FbxScene* scene;
		FbxIOSettings* ios;
		FbxGeometryConverter* converter;

	private:
		vector<struct FbxMaterial *> materials;

		vector<struct FbxBoneData *> boneDatas;
		vector<struct FbxMeshData *> meshDatas;
	};
}