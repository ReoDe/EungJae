#include "stdafx.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState()
	: state(NULL)
{
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_BACK;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;

	Changed();
}

RasterizerState::~RasterizerState()
{
	SAFE_RELEASE(state);
}

void RasterizerState::RSSetState()
{
	D3D::GetDC()->RSSetState(state);
}

void RasterizerState::FillMode(D3D11_FILL_MODE val)
{
	desc.FillMode = val;

	Changed();
}

void RasterizerState::Changed()
{
	SAFE_RELEASE(state);

	HRESULT hr = D3D::GetDevice()->CreateRasterizerState(&desc, &state);
	assert(SUCCEEDED(hr));
}
