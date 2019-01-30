#pragma once


struct SphereStruct
{
	Material* sphereMaterial; // 구 매터리얼
	MeshSphere* meshSphere; // 구 생성
	MeshCubeParts* meshCube;
	wstring PartsName; // 구 이름

	D3DXVECTOR3 MinV; // 최대최소
	D3DXVECTOR3 MaxV; // 구하고

	float Radius;

	UINT Number;
};


class ModelBone;
class ModelMesh;

class Model
{
public:
	Model();
	~Model();

	UINT MaterialCount() { return materials.size(); }
	vector<Material *>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

	UINT BoneCount() { return bones.size(); }
	vector<ModelBone *>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh *>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }
	ModelMesh* MeshByName(wstring name);


	void ReadMaterial(wstring folder, wstring file);
	void ReadMesh(wstring folder, wstring file);

	void CopyGlobalBoneTo(vector<D3DXMATRIX>& transforms);
	void CopyGlobalBoneTo(vector<D3DXMATRIX>& transforms, D3DXMATRIX& w);
	
	
	// 과제

	void SetSphere();
	void SetMinMax(D3DXVECTOR3& min, D3DXVECTOR3& max, class ModelMeshPart* meshParts);
	vector<SphereStruct*>& SphereBox() { return spheres; }


	void SetbindingBox();
	D3DXVECTOR3 Min, Max;
private:
	void BindingBone();
	void BindingMesh();

private:
	class ModelBone* root;

	vector<Material *> materials;
	vector<class ModelMesh *> meshes;
	vector<class ModelBone *> bones;

	vector<struct SphereStruct*> spheres;

};

class Models
{
public:
	friend class Model;

private:
	static void LoadMaterial(wstring file, vector<Material *>* materials);
	static void ReadMaterialData(wstring file);

	static void LoadMesh(wstring file, vector<ModelBone *>* bones, vector<ModelMesh *>* meshes);
	static void ReadMeshData(wstring file);

private:
	static map<wstring, vector<Material *>> materialMap;

	struct MeshData
	{
		vector<class ModelBone *> Bones;
		vector<class ModelMesh *> Meshes;
	};
	static map<wstring, MeshData> meshDataMap;
};