#pragma once


struct CubeStruct
{
	MeshCubeParts* meshCubeParts;
	MeshCube* meshCube;
	wstring PartsName;
	D3DXVECTOR3 MinV;
	D3DXVECTOR3 MaxV;

	D3DXVECTOR3 Scale;
	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 Position;

	UINT Number;

	D3DXMATRIX asdf;
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


	void SetbindingBox();
	void SetMinMax(D3DXVECTOR3& min, D3DXVECTOR3& max, class ModelMeshPart* meshParts);

	//
	vector<CubeStruct*>& BindingBox() { return bindingBox; }


	///


	vector<class Cube*>& BoundingBoxes() { return boundingBoxes; }
	
private:
	void BindingBone();
	void BindingMesh();

private:
	class ModelBone* root;

	vector<Material *> materials;
	vector<class ModelMesh *> meshes;
	vector<class ModelBone *> bones;

	vector<CubeStruct*> bindingBox;

	//

	vector<class Cube*> boundingBoxes;

	Shader * shader;
	Material* CubeMaterial;

private:
	class BoneBuffer : public ShaderBuffer
	{
	public:
		BoneBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			for (int i = 0; i < 128; i++)
				D3DXMatrixIdentity(&Data.Bones[i]);
		}

		void Bones(D3DXMATRIX* m, UINT count)
		{
			memcpy(Data.Bones, m, sizeof(D3DXMATRIX) * count);
			
			for (UINT i = 0; i < count; i++)
				D3DXMatrixTranspose(&Data.Bones[i], &Data.Bones[i]);
		}

	private:
		struct Struct
		{
			D3DXMATRIX Bones[128];
		} Data;
	};

	BoneBuffer* buffer;

public:
	BoneBuffer* Buffer() { return buffer; }
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