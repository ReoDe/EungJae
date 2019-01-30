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

	// �̵��Լ�
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

	// �ﰢ��
	Shader* triShader;
	D3DXVECTOR3 triPosition;
	ID3D11Buffer* triVertexBuffer;

	// ����ü

	Shader* CubeShader;	// ���̴�
	D3DXVECTOR3 CubePosition; // ����
	ID3D11Buffer* CubeVertexBuffer; // ���ؽ�����
	ID3D11Buffer* CubeIndexBuffer;	// �ε�������
	VertexTextureColorNormal* CubeVertices; // ���ؽ�
	UINT* CubeIndices; // �ε���


	// �̵�
	D3DXVECTOR3 CubePositionDirection; // ���ϴ� ����
	D3DXMATRIX T;
	D3DXMATRIX Tk;
	float Go, Back;

	//ȸ��
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