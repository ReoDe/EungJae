#pragma once


class DrawRect : public Execute
{
public:
	DrawRect(ExecuteValues* values);
	~DrawRect();

	void Update() override {}//override의미는없지만 써주는게좋음
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:

	Shader * shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	VertexColor* vertices;
	UINT* indices;

	RasterizerState* rState[2];

private:
	class ColorBuffer : public ShaderBuffer
	{

	public: // ???

		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Color = D3DXCOLOR(0, 1, 0, 1);
			Data.Color2 = D3DXCOLOR(0, 0.5f, 0, 1);
			Data.Select = 0;
		}

		struct Struct
		{
			D3DXCOLOR Color; // 사용할 쉐이더의 변수명과 같은 이름으로 생성
			D3DXCOLOR Color2;

			int Select;
			float Padding[3]; // 16바이트 맞추기 위한 더미
		
		} Data;

	};

	ColorBuffer* colorBuffer;
};