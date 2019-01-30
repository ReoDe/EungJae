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

	D3DXVECTOR3 XYZ() { return xyz; }
private:

private:

	Shader * shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	VertexTextureColor* vertices; // 색을 할때 vertexcolor
	UINT* indices;

	UINT vertexCount, indexCount;
	UINT width, height; // 칸수


	RasterizerState* rState[2];

	D3DXVECTOR3 xyz = { 0,0,0 };

private:

	class ColorBuffer : public ShaderBuffer
	{

	public: // ???

		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
		
		}

		struct Struct
		{

		
			float Padding[4];
		

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