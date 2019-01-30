#pragma once
#include "Camera.h"


class Pixed : public Camera
{
public:

	Pixed(float moveSpeed = 20.0f, float rotationSpeed = 2.5f);
	~Pixed();

	void Update();
	void Render();

private:
	float moveSpeed;
	float rotationSpeed;

	float angle;
	
};