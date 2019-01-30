#include "stdafx.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "ModelBone.h"

ModelMesh::ModelMesh()
{
	
}

ModelMesh::~ModelMesh()
{
	for (ModelMeshPart* part : meshParts)
		SAFE_DELETE(part);
}

void ModelMesh::Render()
{
	for (ModelMeshPart* part : meshParts)
	{
		part->Render();
	}
}

void ModelMesh::Copy(ModelMesh ** clone)
{
	ModelMesh* mesh = new ModelMesh();
	mesh->name = name;
	mesh->parentBoneIndex = parentBoneIndex;

	for (ModelMeshPart* part : meshParts)
	{
		ModelMeshPart* temp = NULL;
		part->Copy(&temp);

		temp->parent = mesh;
		mesh->meshParts.push_back(temp);
	}
	*clone = mesh;
}

void ModelMesh::Pass(UINT pass)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(pass);
}

void ModelMesh::Binding()
{
	for (ModelMeshPart* part : meshParts)
		part->Binding();
}
