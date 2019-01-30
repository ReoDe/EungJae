#pragma once
#include "Mesh.h"

class MeshGrid : public Mesh
{
public:
	MeshGrid(Material* material, UINT countX, UINT countZ, float sizeX, float sizeZ, float offSetU = 1.0f, float offSetV = 1.0f);
	~MeshGrid();

	void CreateData() override;

private:
	UINT countX, countZ;
	float sizeX, sizeZ;
	float offSetU, offSetV;

};