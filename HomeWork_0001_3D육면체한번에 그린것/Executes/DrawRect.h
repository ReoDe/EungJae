#pragma once


class DrawRect : public Execute
{
public:
	DrawRect(ExecuteValues* values);
	~DrawRect();

	void Update() override; //override의미는없지만 써주는게좋음
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}
	void ResizeScreen() override {}

private:

	void Front_Left(wstring val);
	void Front_Right(wstring val);

	void Back_Left(wstring val);
	void Back_Right(wstring val);

	void Left_Left(wstring val);
	void Left_Right(wstring val);

	void Right_Left(wstring val);
	void Right_Right(wstring val);

	void Top_Left(wstring val);
	void Top_Right(wstring val);

	void Bottom_Left(wstring val);
	void Bottom_Right(wstring val);
private:

	Shader * shader;
	WorldBuffer* worldBuffer;


	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	VertexTexture* vertices;
	UINT* indices;
	
	// 과제
	VertexTexture* dice;
	UINT* indices2;
	int VertexCount;
	int indiceCount;






	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* srv2;

	ID3D11ShaderResourceView* srvs[12];


private:
	class ColorBuffer : public ShaderBuffer
	{

	public: // ???

		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.X1 = 0.5f;
			Data.X2 = 1.5f;
			Data.X3 = 2.5f;
			Data.X4 = 3.5f;
			Data.X5 = 4.5f;
			Data.X6 = 5.5f;
		}

		struct Struct
		{
			float X1;
			float X2;
			float X3;
			float X4;
			float X5;
			float X6;

			float Padding[2];

		} Data;

	};

	ColorBuffer* colorBuffer;
};