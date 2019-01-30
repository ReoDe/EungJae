#include "stdafx.h"
#include "DepthStencilState.h"

DepthStencilState::DepthStencilState()
	: state(NULL)
{
	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = true; // 깊이판단을 하는것
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.StencilEnable = true;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;

	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Changed();
}

DepthStencilState::~DepthStencilState()
{
	SAFE_RELEASE(state);
}

void DepthStencilState::OMSetDepthStencilState(UINT stencilRef)
{
	D3D::GetDC()->OMSetDepthStencilState(state, stencilRef);
}

void DepthStencilState::DepthEnable(bool val)
{
	desc.DepthEnable = val;

	Changed();
}

void DepthStencilState::StencilEnable(bool val)
{
	desc.StencilEnable = val;

	Changed();
}

void DepthStencilState::Changed()
{
	SAFE_RELEASE(state);

	HRESULT hr;

	hr = D3D::GetDevice()->CreateDepthStencilState(&desc, &state);
	assert(SUCCEEDED(hr));
}
