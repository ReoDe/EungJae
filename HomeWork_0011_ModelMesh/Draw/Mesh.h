#pragma once

class Mesh
{
public:
	Mesh(Material* material);
	Mesh(Material* material, Shader* shader);
	virtual ~Mesh();

	void Render();

	void Position(float x, float y, float z);
	void Position(D3DXVECTOR3& vec);
	void Position(D3DXVECTOR3* vec);

	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	void Rotation(D3DXVECTOR3* vec);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	void RotationDegree(D3DXVECTOR3* vec);
	
	void Scale(float x, float y, float z);
	void Scale(D3DXVECTOR3& vec);
	void Scale(D3DXVECTOR3* vec);

	void Matrix(D3DXMATRIX& mat);
	void Matrix(D3DXMATRIX* mat);

	VertexTextureNormalTangent* vertice() { return vertices; }
	UINT* Getindices() { return indices; }
	UINT GetvertexCount() { return vertexCount; }
protected:
	virtual void CreateData() = 0;
	void CreateBuffer();

public:
	void UpdateWorld();

protected:
	VertexTextureNormalTangent* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;

	Material* material;
	Shader* shader;

	D3DXMATRIX world;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};