#pragma once
#include "Mesh.h"

class MeshCube : public Mesh
{
public:
	MeshCube(Material* material, float width, float height, float depth);
	~MeshCube();

	void CreateData() override;


	D3DXVECTOR3 GetMax() { return Max; }
	D3DXVECTOR3 GetMin() { return Min; }


private:
	float width;
	float height;
	float depth;

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;
};