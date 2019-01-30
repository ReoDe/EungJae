#pragma once

class Terrain
{

public:
	Terrain(ExecuteValues* values, Material* material, wstring heightMap);
	~Terrain();

	void Update();
	void Render();


	bool GetHeight(OUT D3DXVECTOR3& picked);
	float GetHeightY(D3DXVECTOR3& position);
	D3DXVECTOR3 GetPickingMouseY();
	// 이동함수
	void MovePoint(D3DXVECTOR3* Origin,
		D3DXVECTOR3* Direction,
		float Speed,
		D3DXVECTOR3& Rotation);

private:
	void CreateData();
	void CreateNormalData();
	void CreateBuffer();




private:
	ExecuteValues * values;
	Material* material;
	WorldBuffer* worldBuffer;

	Texture* heightMap;

	UINT width, height;

	VertexTextureNormal* vertices;
	UINT* indices;
	UINT vertexCount, indexCount;
	ID3D11Buffer* vertexBuffer, *indexBuffer;

	// 

	float Angle;

private:
	class BrushBuffer : public ShaderBuffer
	{
	public:
		BrushBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Type = 0;
			Data.Location = D3DXVECTOR3(0, 0, 0);
			Data.Range = 0;
			Data.Color = D3DXVECTOR3(0, 1, 0);
		}

		struct Struct
		{
			int Type;
			D3DXVECTOR3 Location;

			int Range;
			D3DXVECTOR3 Color;


		}Data;
	};
	BrushBuffer* brushBuffer;
};
