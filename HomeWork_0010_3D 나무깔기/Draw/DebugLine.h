#pragma once

class DebugLine
{
public:
	DebugLine();
	~DebugLine();

	void Color(float r, float g, float b);
	void Color(D3DXCOLOR& vec);

	void Draw(D3DXMATRIX& world, class ILine* line);
	void Render();

private:
	static const UINT MaxCount;

private:
	Shader* shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;

	struct Line
	{
		D3DXVECTOR3 start;
		D3DXVECTOR3 end;
	};
	vector<Line> lines;

private:
	class Buffer : public ShaderBuffer
	{
	public:
		Buffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Color = D3DXCOLOR(1, 1, 1, 1);
		}

		struct Struct
		{
			D3DXCOLOR Color;
		} Data;
	};
	Buffer* buffer;
};