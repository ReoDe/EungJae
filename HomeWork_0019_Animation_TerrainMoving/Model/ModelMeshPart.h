#pragma once

class ModelMeshPart
{
public:
	friend class Model;
	friend class Models;
	friend class ModelMesh;

public:
	void Pass(UINT pass);

private:
	ModelMeshPart();
	~ModelMeshPart();

	void Copy(ModelMeshPart** clone);

	void Binding();
	void Render();

private:
	UINT pass;

	Material* material;
	wstring materialName;

	class ModelMesh* parent;

	vector<VertexTextureNormalBlend> vertices;
	vector<UINT> indices;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};