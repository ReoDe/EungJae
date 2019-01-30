#pragma once
#include "Mesh.h"

class MeshCube : public Mesh
{
public:
	MeshCube(Material* material, float width, float height, float depth);
	~MeshCube();

	void CreateData() override;

	D3DXVECTOR3 GetMax() 
	{
		float w = width * 0.5f;
		float h = height * 0.5f;
		float d = depth * 0.5f;

		Max = D3DXVECTOR3(w, h, d);

		return Max;
	}
	D3DXVECTOR3 GetMin() 
	{ 
		float w = width * 0.5f;
		float h = height * 0.5f;
		float d = depth * 0.5f;

		Min = D3DXVECTOR3(-w, -h, -d);

		return Min; 
	}

private:
	float width;
	float height;
	float depth;

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;


};