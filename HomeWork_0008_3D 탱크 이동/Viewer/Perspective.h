#pragma once

class Perspective
{
public:
	Perspective(float width, float height, float fov = (float)D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);
	~Perspective();

	void GetMatrix(D3DXMATRIX* mat);
	void Set(float width, float height, float fov = (float)D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);

private:
	float width, height;
	float fov;
	float aspect;
	float zn, zf;

	D3DXMATRIX projection;
};