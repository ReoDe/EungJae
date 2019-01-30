#include "stdafx.h"
#include "GameModel.h"

GameModel::GameModel(Shader* shader, wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile)
	: shader(shader)
{
	model = new Model();
	model->ReadMaterial(matFolder, matFile);
	model->ReadMesh(meshFolder, meshFile);

	buffer = new Buffer();

	model->CopyGlobalBoneTo(transforms);

	for (Material* material : model->Materials())
		material->SetShader(shader);	
}

GameModel::~GameModel()
{
	SAFE_DELETE(buffer);
	SAFE_DELETE(model);
}

void GameModel::UpdateWorld()
{
	__super::UpdateWorld();

	D3DXMATRIX W;
	World(&W);

	model->CopyGlobalBoneTo(transforms, W);
}

void GameModel::Update()
{
	
}

void GameModel::Render()
{
	model->Buffer()->Bones(&transforms[0], transforms.size());
	model->Buffer()->SetVSBuffer(2);

	for (ModelMesh* mesh : model->Meshes())
	{
		int index = mesh->ParentBoneIndex();

		buffer->Data.Index = index;
		buffer->SetVSBuffer(3);

		mesh->Render();
	}
}

Model * GameModel::GetModel()
{
	return model;
}
