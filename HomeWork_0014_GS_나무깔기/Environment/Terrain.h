#pragma once

enum Shape
{
	Rect = 1,
	Circle,
	Cone
};

enum Tile
{
	Tile_A = 0,
	Tile_B,
	Tile_C,
	Tile_D
};


class Terrain
{

public:
	Terrain(Material* material, wstring heightMap);
	~Terrain();

	void Update();
	void Render();

	bool GetHeight(OUT D3DXVECTOR3& picked);
	float GetHeightY(D3DXVECTOR3& position);
	
	float GetHeight(float positionX, float positionZ);

private:
	void CreateData();
	void CreateNormalData();
	void CreateBuffer();



private:
	Material * material;
	Texture* heightMap;
	Shader* shader;

	UINT width, height;

	VertexTextureNormal* vertices;
	UINT* indices;
	UINT vertexCount, indexCount;
	ID3D11Buffer* vertexBuffer, *indexBuffer;

	vector<D3DXCOLOR> heights;

	D3DXMATRIX world;
};
