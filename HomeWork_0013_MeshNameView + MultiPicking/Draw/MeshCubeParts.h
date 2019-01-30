#pragma once
#include "Mesh.h"

class MeshCubeParts : public Mesh
{
public:
	MeshCubeParts(Material* material, D3DXVECTOR3 Min, D3DXVECTOR3 Max);
	~MeshCubeParts();

	void CreateData() override;

	D3DXVECTOR3 GetMin() { return Min; }
	D3DXVECTOR3 GetMax() { return Max; }
private:

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;

};