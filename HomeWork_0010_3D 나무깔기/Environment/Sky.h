#pragma once

class Sky
{
public:
	Sky(ExecuteValues* values);
	~Sky();

	void Update();
	void Render();

private:
	ExecuteValues * values;

	//Shader* shader;
	Material* material;

	MeshSphere * sphere;

	RasterizerState* rasterizer[2];
	DepthStencilState* depthStencil[2];

private:

	class Buffer : public ShaderBuffer
	{

	public:

		Buffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Center = D3DXCOLOR(0xFF0080FF);
			Data.Apex = D3DXCOLOR(0xFF9BCDFF);
			Data.height = 4.5f;
		}


		struct Struct
		{
			D3DXCOLOR Center;
			D3DXCOLOR Apex;

			float height;
			float Padding[3];
		}Data;
	};

	Buffer* buffer;


};
