#pragma once

class DrawCube : public Execute
{
public:
	DrawCube(ExecuteValues* values);
	~DrawCube();

	void Update() override;

	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	
	bool GetPickingMouse();

	

private:
	// 육면체
	WorldBuffer * worldBuffer;
	Shader* CubeShader;	// 쉐이더
	D3DXVECTOR3 CubePosition; // 중점
	ID3D11Buffer* CubeVertexBuffer; // 버텍스버퍼
	ID3D11Buffer* CubeIndexBuffer;	// 인덱스버퍼
	VertexTextureColorNormal* CubeVertices; // 버텍스
	UINT* CubeIndices; // 인덱스

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;

	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 position;

	bool check;


					   // 이동
	D3DXVECTOR3 CubePositionDirection; // 원하는 방향

private:
	class ColorBuffer : public ShaderBuffer
	{
	public:
		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			
		}

		struct Struct
		{
			float Padding[4];
		} Data;
	};
	ColorBuffer* colorBuffer;
};