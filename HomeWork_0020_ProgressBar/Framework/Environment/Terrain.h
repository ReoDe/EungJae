#pragma once

class Terrain
{
public:
	Terrain(Material* material, wstring heightMap);
	~Terrain();

	void Update();
	void Render();

	float GetHeight(float positionX, float positionZ);

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
};