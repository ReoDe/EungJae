#pragma once


class DrawGrid : public Execute
{
public:
	DrawGrid(ExecuteValues* values);
	~DrawGrid();

	void Update() override; //override�ǹ̴¾����� ���ִ°�����
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:

	void ReadPixels(wstring file, UINT ** heightMap);

	float GetY();
private:

	Shader * shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	VertexTextureColorNormal* vertices; // ���� �Ҷ� vertexcolor
	UINT* indices;
	UINT vertexCount, indexCount;
	UINT width, height; // ĭ��


	RasterizerState* rState[2];


	// �ﰢ��
	Shader* triShader;
	D3DXVECTOR3 triPosition;
	ID3D11Buffer* triVertexBuffer;

	// �븻���� ������
	Shader* shader2;
	Vertex* normalVertices;
	UINT* normalIndices;
	ID3D11Buffer* normalVertexBuffer;
	ID3D11Buffer* normalIndexBuffer;
	UINT vertexCount2, indexCount2;




private:

	class ColorBuffer : public ShaderBuffer
	{

	public: // ???

		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Direction = D3DXVECTOR3(1, 0, 0);
			// �� �� ���� �ݴ��ϱ�
		}

		struct Struct
		{
			D3DXVECTOR3 Direction;

			float Padding;
		

		} Data;

	private:
		//vector<POINT> pointVector;

		/*struct Struct1
		{
			int XX, YY;
			float Padding[2];
		};*/
	};

	ColorBuffer* colorBuffer;


};