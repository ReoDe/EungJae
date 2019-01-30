#pragma once

class Terrain
{
public:
	Terrain(Material* material, wstring heightMap);
	~Terrain();

	void Update();
	void Render();
	bool GetHeight(OUT D3DXVECTOR3& picked);

	float GetHeight(float positionX, float positionZ);
	float GetHeightY(D3DXVECTOR3& position);
	D3DXVECTOR3 GetPickingMouseY();
	void MovePoint(D3DXVECTOR3* Origin,
		D3DXVECTOR3* Direction,
		float Speed,
		D3DXVECTOR3& Rotation);

private:
	void CreateData();
	void CreateNormalData();
	void CreateBuffer();

private:
	Shader* shader;
	Material* material;
	Texture* heightMap;

	UINT width, height;

	VertexTextureNormal* vertices;
	UINT* indices;
	UINT vertexCount, indexCount;
	ID3D11Buffer* vertexBuffer, *indexBuffer;

	//
	float Angle;
};