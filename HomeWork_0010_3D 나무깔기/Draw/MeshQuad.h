#pragma once
#include "Mesh.h"

class MeshQuad : public Mesh
{
public:
	MeshQuad(Material* material, float width, float heighth);
	~MeshQuad();

	void CreateData() override;

private:
	float width;
	float height;

};