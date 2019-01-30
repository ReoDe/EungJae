#pragma once
#include "Mesh.h"

class MeshCubeMinMax : public Mesh
{
public:
	MeshCubeMinMax(Material* material, D3DXVECTOR3 Min, D3DXVECTOR3 Max);
	~MeshCubeMinMax();

	void CreateData() override;

	D3DXVECTOR3 GetMin() { return Min; }
	D3DXVECTOR3 GetMax() { return Max; }

	float GetWidth() 
	{
		return abs((Max.x - Min.x) / 2);
	}
	float GetHeight() 
	{ 
		return abs((Max.y - Min.y) / 2);
	}
	float GetDepth() 
	{ 
		return abs((Max.z - Min.z) / 2);
	}
private:

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;

	float width;
	float height;
	float depth;

};