#pragma once


class DrawCube : public Execute
{
public:
	DrawCube(ExecuteValues* values);
	~DrawCube();

	void Update() override; //override의미는없지만 써주는게좋음
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}
	void ResizeScreen() override {}

	D3DXVECTOR3 XYZ() { return xyz; }
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

	};

	ColorBuffer* colorBuffer;
};