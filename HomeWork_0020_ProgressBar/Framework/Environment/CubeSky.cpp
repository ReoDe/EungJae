#include "framework.h"
#include "CubeSky.h"
#include "../Viewer/Camera.h"

CubeSky::CubeSky(wstring cubeFile)
{
	material = new Material(Shaders + L"026_CubeSky.fx");
	sphere = new MeshSphere(material, 500, 30, 30);

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), cubeFile.c_str(), NULL, NULL, &srv, NULL
	);
	assert(SUCCEEDED(hr));

	material->GetShader()->AsShaderResource("CubeMap")->SetResource(srv);
}

CubeSky::~CubeSky()
{
	SAFE_RELEASE(srv);

	SAFE_DELETE(sphere);
	SAFE_DELETE(material);
}

void CubeSky::Update()
{
	D3DXVECTOR3 position;
	Context::Get()->GetMainCamera()->Position(&position);

	sphere->Position(position);
}

void CubeSky::Render()
{
	sphere->Render();
}
