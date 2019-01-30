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

	void AdjustHeight(D3DXVECTOR3& location);

	// 과제 시작
	void circleHeight(D3DXVECTOR3& location);
	void coneHeight(D3DXVECTOR3& location);

	void AdjustColorMapA(D3DXVECTOR3& location);
	void AdjustColorMapB(D3DXVECTOR3& location);
	void AdjustColorMapC(D3DXVECTOR3& location);
	void AdjustColorMapD(D3DXVECTOR3& location);

	void circleAdjustColorMapA(D3DXVECTOR3& location);
	void circleAdjustColorMapB(D3DXVECTOR3& location);
	void circleAdjustColorMapC(D3DXVECTOR3& location);
	void circleAdjustColorMapD(D3DXVECTOR3& location);




	void SaveData();


private:
	ExecuteValues * values;
	Material* material;
	WorldBuffer* worldBuffer;

	Texture* heightMap;

	//실험


	//
	Texture* colorMap;
	Texture* colorMap2;
	Texture* colorMap3;
	Texture* alphaMap;


	UINT width, height;

	VertexColorTextureNormal* vertices;
	// VertexTextureColorNormal 로 바꾸고, color에 rgba에 넣자.
	UINT* indices;
	UINT vertexCount, indexCount;
	ID3D11Buffer* vertexBuffer, *indexBuffer;

	// 

	float Angle;

	// 과제 시작
	//vector<D3DXCOLOR> ModifiedData;
	vector<D3DXCOLOR> heights;


	bool isGreen = true;

	float capacity;
	int BrushType;
	Tile tile;
	

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
