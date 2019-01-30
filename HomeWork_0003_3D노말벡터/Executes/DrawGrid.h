#pragma once


class DrawGrid : public Execute
{
public:
	DrawGrid(ExecuteValues* values);
	~DrawGrid();

	void Update() override; //override의미는없지만 써주는게좋음
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

	VertexTextureColorNormal* vertices; // 색을 할때 vertexcolor
	UINT* indices;
	UINT vertexCount, indexCount;
	UINT width, height; // 칸수


	RasterizerState* rState[2];


	// 삼각형
	Shader* triShader;
	D3DXVECTOR3 triPosition;
	ID3D11Buffer* triVertexBuffer;

	// 노말벡터 데이터
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
			// 난 왜 방향 반대일까
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