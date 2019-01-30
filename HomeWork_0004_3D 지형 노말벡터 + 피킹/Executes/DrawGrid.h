#pragma once

class DrawGrid : public Execute
{
public:
	DrawGrid(ExecuteValues* values);
	~DrawGrid();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	void ReadPixels(wstring file, UINT** heightMap);
	float GetY();
	float GetPickingY();
	D3DXVECTOR3 GetPickingMouseY();

	// 이동함수
	void MovePoint(D3DXVECTOR3* Origin, 
		D3DXVECTOR3* Direction, 
		float Speed, 
		D3DXMATRIX* Ry,
		D3DXMATRIX* Rx,
		D3DXMATRIX* Rz);
	
private:
	Shader* shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	VertexTextureColorNormal* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;
	UINT width, height;

	RasterizerState* rState[2];

	// 삼각형
	Shader* triShader;
	D3DXVECTOR3 triPosition;
	ID3D11Buffer* triVertexBuffer;

	// 육면체

	Shader* CubeShader;	// 쉐이더
	D3DXVECTOR3 CubePosition; // 중점
	ID3D11Buffer* CubeVertexBuffer; // 버텍스버퍼
	ID3D11Buffer* CubeIndexBuffer;	// 인덱스버퍼
	VertexTextureColorNormal* CubeVertices; // 버텍스
	UINT* CubeIndices; // 인덱스


	// 이동
	D3DXVECTOR3 CubePositionDirection; // 원하는 방향
	D3DXMATRIX T;
	D3DXMATRIX Tk;
	float Go, Back;

	//회전
	D3DXMATRIX Ry;
	D3DXMATRIX Rx;
	D3DXMATRIX Rz;


private:
	class ColorBuffer : public ShaderBuffer
	{
	public:
		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Direction = D3DXVECTOR3(1, 0, 0);
		}

		struct Struct
		{
			D3DXVECTOR3 Direction;

			float Padding;
		} Data;
	};
	ColorBuffer* colorBuffer;
};