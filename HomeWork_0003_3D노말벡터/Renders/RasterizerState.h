#pragma once

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	void RSSetState();

	void FillMode(D3D11_FILL_MODE val);
private:
	void Changed();

private:

	D3D11_RASTERIZER_DESC desc; // ??? Áú¹®
	ID3D11RasterizerState* state;
};