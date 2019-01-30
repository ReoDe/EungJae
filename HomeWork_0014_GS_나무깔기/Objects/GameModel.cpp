#include "stdafx.h"
#include "GameModel.h"

GameModel::GameModel(wstring shaderFile, wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile)
{
	model = new Model();
	model->ReadMaterial(matFolder, matFile);
	model->ReadMesh(meshFolder, meshFile);
	model->CopyGlobalBoneTo(transforms);

	SetShader(shaderFile);
}

GameModel::~GameModel()
{
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
	for (Material* material : model->Materials())
	{
		const float* data = transforms[0];
		UINT count = transforms.size();

		material->GetShader()->AsMatrix("Bones")->SetMatrixArray(data, 0, count);
	}
	
	for (ModelMesh* mesh : model->Meshes())
		mesh->Render();
}

Model * GameModel::GetModel()
{
	return model;
}

void GameModel::SetShader(wstring shaderFile)
{
	for (Material* material : model->Materials())
		material->SetShader(shaderFile);
}

void GameModel::SetDiffuse(float r, float g, float b, float a)
{
	SetDiffuse(D3DXCOLOR(r, g, b, a));
}

void GameModel::SetDiffuse(D3DXCOLOR & color)
{
	for (Material* material : model->Materials())
		material->SetDiffuse(color);
}

void GameModel::SetDiffuseMap(wstring file)
{
	for (Material* material : model->Materials())
		material->SetDiffuseMap(file);
}

void GameModel::SetSpecular(float r, float g, float b, float a)
{
	SetSpecular(D3DXCOLOR(r, g, b, a));
}

void GameModel::SetSpecular(D3DXCOLOR & color)
{
	for (Material* material : model->Materials())
		material->SetSpecular(color);
}

void GameModel::SetSpecularMap(wstring file)
{
	for (Material* material : model->Materials())
		material->SetSpecularMap(file);
}

void GameModel::SetNormalMap(wstring file)
{
	for (Material* material : model->Materials())
		material->SetNormalMap(file);
}