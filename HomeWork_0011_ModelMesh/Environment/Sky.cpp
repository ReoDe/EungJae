#include "stdafx.h"
#include "Sky.h"

Sky::Sky(ExecuteValues * values)
	:values(values)
{
	material = new Material(Shaders + L"018_Sky.hlsl");

	buffer = new Buffer();

	sphere = new MeshSphere(material, 0.5f, 20, 20);

	rasterizer[0] = new RasterizerState();
	rasterizer[1] = new RasterizerState();
	rasterizer[1]->FrontCounterClockwise(true);

	depthStencil[0] = new DepthStencilState();
	depthStencil[1] = new DepthStencilState();
	depthStencil[1]->DepthEnable(false);

}

Sky::~Sky()
{
	
}

void Sky::Update()
{
	D3DXVECTOR3 position;
	values->MainCamera->Position(&position);

	sphere->Position(position);
}

void Sky::Render()
{
	ImGui::ColorEdit3("Apex", (float*)&buffer->Data.Apex);
	ImGui::ColorEdit3("Center", (float*)&buffer->Data.Center);
	ImGui::SliderFloat("Height", &buffer->Data.height, 0 , 100);

	buffer->SetPSBuffer(10);


	rasterizer[1]->RSSetState();
	depthStencil[1]->OMSetDepthStencilState();
	{
		sphere->Render();
	}
	depthStencil[0]->OMSetDepthStencilState();
	rasterizer[0]->RSSetState();
}
