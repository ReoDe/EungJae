#pragma once

class Viewport
{
public:
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	void RSSetViewport();
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidth() { return width; }
	float GetHeight() { return height; }

	D3DXVECTOR3 GetDirection(D3DXMATRIX view, D3DXMATRIX projection);
	void GetRay(struct Ray* ray, D3DXVECTOR3& start, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);
	void GetRayMouse(D3DXVECTOR3 Mouse, struct Ray* ray, D3DXVECTOR3& start, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);
	void Get2DPos(D3DXVECTOR2& Pos2D, D3DXVECTOR3 Position, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);
private:
	float x, y;
	float width, height;
	float minDepth, maxDepth;

	D3D11_VIEWPORT viewport;
};