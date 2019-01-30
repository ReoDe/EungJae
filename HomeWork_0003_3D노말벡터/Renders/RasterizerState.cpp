#include "stdafx.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState()
	: state(NULL)
{
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.AntialiasedLineEnable = false; // 안티엘리어싱, 계단형을 직선으로 보이게
	desc.CullMode = D3D11_CULL_BACK; // 같은 법선벡터를 가지는 안보이는 뒷면을 안보이게
	desc.DepthClipEnable = true; // 깊이
	desc.FillMode = D3D11_FILL_SOLID; // 고체로채움 f12눌러봐서 와이어어쩌구확인
	desc.FrontCounterClockwise = false; //시계방향으로 그린것을 앞으로 판단

	Changed();
}

RasterizerState::~RasterizerState()
{
	SAFE_RELEASE(state);
}

void RasterizerState::RSSetState()
{
	D3D::GetDC()->RSSetState(state); // 상태변경할꺼다
}

void RasterizerState::FillMode(D3D11_FILL_MODE val)
{
	desc.FillMode = val;

	Changed();

}

void RasterizerState::Changed()
{
	SAFE_RELEASE(state); //만들어져있다면 지운다.

	HRESULT hr = D3D::GetDevice()->CreateRasterizerState(&desc, &state);

	assert(SUCCEEDED(hr));

}
