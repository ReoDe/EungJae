#include "framework.h"
#include "ModelMeshPart.h"
#include "ModelMesh.h"

void ModelMeshPart::Pass(UINT pass)
{
	this->pass = pass;
}

ModelMeshPart::ModelMeshPart()
	: pass(0)
{
}

ModelMeshPart::~ModelMeshPart()
{
}

void ModelMeshPart::Copy(ModelMeshPart ** clone)
{
	ModelMeshPart* part = new ModelMeshPart();
	part->materialName = materialName;
	part->startVertex = startVertex;
	part->vertexCount = vertexCount;

	*clone = part;
}

void ModelMeshPart::Render()
{
	material->GetShader()->Draw(0, pass, vertexCount, startVertex);
}