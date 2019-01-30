#pragma once


class DrawRect : public Execute
{
public:
	DrawRect(ExecuteValues* values);
	~DrawRect();

	void Update() override {}//override�ǹ̴¾����� ���ִ°�����
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
			D3DXCOLOR Color; // ����� ���̴��� ������� ���� �̸����� ����
			D3DXCOLOR Color2;

			int Select;
			float Padding[3]; // 16����Ʈ ���߱� ���� ����
		
		} Data;

	};

	ColorBuffer* colorBuffer;
};