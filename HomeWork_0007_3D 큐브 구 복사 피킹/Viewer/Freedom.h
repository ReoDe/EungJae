#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom(float moveSpeed = 20.0f, float rotationSpeed = 2.5f);
	~Freedom();

	void Update();

private:
	float moveSpeed;
	float rotationSpeed;
};