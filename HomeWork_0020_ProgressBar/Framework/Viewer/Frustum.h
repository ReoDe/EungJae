#pragma once

class Frustum
{
public:
	Frustum(float zFar, Camera* camera = NULL, Perspective* perspective = NULL);
	~Frustum();

	void ZFar(float val) { zFar = val; }
	void Update();

	bool ContainPoint(D3DXVECTOR3& position);
	bool ContainRect(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
	bool ContainRect(D3DXVECTOR3 center, D3DXVECTOR3 size);
	bool ContainCube(D3DXVECTOR3& center, float radius);

private:
	D3DXPLANE planes[6];

	float zFar;

	Camera* camera;
	Perspective* perspective;
};