#pragma once
#include "Mesh.h"

class MeshSphere : public Mesh
{
public:
	MeshSphere(Material* material, float radius, UINT stackCount = 10, UINT sliceCount = 10);
	~MeshSphere();

	void CreateData() override;
	float GetRadius()
	{
		return radius;
	}
	void SetRadius(float Radius)
	{
		radius = Radius;

	}
private:
	float radius;

	UINT stackCount;
	UINT sliceCount;
};