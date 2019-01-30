#include "stdafx.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState()
	: state(NULL)
{
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.AntialiasedLineEnable = false; // ��Ƽ�������, ������� �������� ���̰�
	desc.CullMode = D3D11_CULL_BACK; // ���� �������͸� ������ �Ⱥ��̴� �޸��� �Ⱥ��̰�
	desc.DepthClipEnable = true; // ����
	desc.FillMode = D3D11_FILL_SOLID; // ��ü��ä�� f12�������� ���̾��¼��Ȯ��
	desc.FrontCounterClockwise = false; //�ð�������� �׸����� ������ �Ǵ�

	Changed();
}

RasterizerState::~RasterizerState()
{
	SAFE_RELEASE(state);
}

void RasterizerState::RSSetState()
{
	D3D::GetDC()->RSSetState(state); // ���º����Ҳ���
}

void RasterizerState::FillMode(D3D11_FILL_MODE val)
{
	desc.FillMode = val;

	Changed();

}

void RasterizerState::Changed()
{
	SAFE_RELEASE(state); //��������ִٸ� �����.

	HRESULT hr = D3D::GetDevice()->CreateRasterizerState(&desc, &state);

	assert(SUCCEEDED(hr));

}
