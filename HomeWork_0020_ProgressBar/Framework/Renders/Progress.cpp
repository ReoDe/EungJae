#include "Framework.h"
#include "Progress.h"
#include "Renders/Render2D.h"

Progress* Progress::instance = nullptr;

Progress * Progress::Get()
{
	return instance;
}

void Progress::Create()
{
	assert(instance == nullptr);

	instance = new Progress();
}

void Progress::Delete()
{
	SAFE_DELETE(instance);
}

void Progress::Render()
{
	ImGui::ProgressBar(rate);

	render2D->Render();

	Bar->Scale(Width, 50);
	Bar->Render();
}

void Progress::RateWidth(float val)
{

	D3DDesc desc;
	D3D::GetDesc(&desc);

	float width = (float)desc.Width;

	Width = width * val;
}

Progress::Progress() : rate(0.0f), Width(0.0f)
{


	D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), (Textures + L"water.png").c_str(), NULL, NULL, &srv, NULL
	);


	D3DDesc desc;
	D3D::GetDesc(&desc);

	float width = (float)desc.Width, height = (float)desc.Height;


	render2D = new class Render2D();
	render2D->Scale(width, height);
	render2D->GetShader()->AsShaderResource("Map")->SetResource(srv);

	//

	D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), (Textures + L"Red.png").c_str(), NULL, NULL, &barSrv, NULL
	);


	Bar = new class Render2D();
	Bar->Scale(Width, 50);
	Bar->GetShader()->AsShaderResource("Map")->SetResource(barSrv);
	

	
}

Progress::~Progress()
{
}
