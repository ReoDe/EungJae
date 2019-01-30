#pragma once

namespace Xml
{
	class XMLDocument;
	class XMLElement;
}

class FbxLoader
{
public:
	FbxLoader(wstring file, wstring saveFolder, wstring saveName);
	~FbxLoader();

	void ExportMaterial(wstring saveFolder = L"", wstring fileName = L"");
	void ExportMesh(wstring saveFolder = L"", wstring fileName = L"");

	void GetClipList(vector<wstring>* list);
	void ExportAnimation(UINT clipNumber, wstring saveFolder = L"", wstring fileName = L"");
	void ExportAnimation(wstring clipName, wstring saveFolder = L"", wstring fileName = L"");

private:
	void ReadMaterial();
	void WriteMaterial(wstring saveFolder, wstring fileName);

	void ReadBoneData(FbxNode* node, int index, int parent);
	void ReadMeshData(FbxNode* node, int parentBone);
	void ReadSkinData();
	void WriteMeshData(wstring saveFolder, wstring fileName);

	void CopyTextureFile(OUT string& textureFile, wstring& saveFolder);
	void WriteXmlColor(Xml::XMLDocument* document, Xml::XMLElement* element, D3DXCOLOR& color);
	
	void ReadAnimationData(UINT index);
	void ReadAnimationData(wstring name);
	void ReadKeyframeData(FbxNode* node, FbxLongLong start, FbxLongLong end);

	void WriteAnimationData(wstring saveFolder, wstring fileName);

private:
	FbxManager* manager;
	FbxImporter* importer;
	FbxScene* scene;
	FbxIOSettings* ios;
	FbxGeometryConverter* converter;

private:
	wstring fbxFile;
	wstring saveFolder;
	wstring saveName;

	vector<struct FbxMaterial *> materials;

	vector<struct FbxBoneData *> boneDatas;
	vector<struct FbxMeshData *> meshDatas;

	struct FbxClip* clipData;
};