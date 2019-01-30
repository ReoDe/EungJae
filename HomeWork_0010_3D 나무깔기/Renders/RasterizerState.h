#pragma once

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	void RSSetState();

	void AntialiasedLineEnable(bool val);
	void CullMode(D3D11_CULL_MODE val);
	void DepthClipEnable(bool val);
	void FillMode(D3D11_FILL_MODE val);
	void FrontCounterClockwise(bool val);
	void MultisampleEnable(bool val);

private:
	void Changed();

private:
	D3D11_RASTERIZER_DESC desc;
	ID3D11RasterizerState* state;
};