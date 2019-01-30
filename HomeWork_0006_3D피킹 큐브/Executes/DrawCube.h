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
	// ����ü
	WorldBuffer * worldBuffer;
	Shader* CubeShader;	// ���̴�
	D3DXVECTOR3 CubePosition; // ����
	ID3D11Buffer* CubeVertexBuffer; // ���ؽ�����
	ID3D11Buffer* CubeIndexBuffer;	// �ε�������
	VertexTextureColorNormal* CubeVertices; // ���ؽ�
	UINT* CubeIndices; // �ε���

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;

	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 position;

	bool check;


					   // �̵�
	D3DXVECTOR3 CubePositionDirection; // ���ϴ� ����

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