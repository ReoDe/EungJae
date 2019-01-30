#include "stdafx.h"
#include "GameModel.h"

GameModel::GameModel(Shader* shader, wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile)
	: shader(shader)
{
	model = new Model();
	model->ReadMaterial(matFolder, matFile);
	model->ReadMesh(meshFolder, meshFile);

	model->SetbindingBox(); // �ӽ�

	model->CopyGlobalBoneTo(transforms);

	for (Material* material : model->Materials())
		material->SetShader(shader);	
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
		const float* data = transforms[0]; // float���� �޴� ����, ��Ʈ������ float�� �迭
		UINT const count = transforms.size();

		material->GetShader()->AsMatrix("Bones")->SetMatrixArray(data, 0, count);

	}

	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->Render();
	}
	
	// model->GetMainBox()->Render();
}

Model * GameModel::GetModel()
{
	return model;
}
