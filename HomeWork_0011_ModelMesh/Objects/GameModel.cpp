#include "stdafx.h"
#include "GameModel.h"

GameModel::GameModel(Shader* shader, wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile)
	: shader(shader), BindingBoxVisible(true)
{
	model = new Model();
	model->ReadMaterial(matFolder, matFile);
	model->ReadMesh(meshFolder, meshFile);

	buffer = new Buffer();

	model->CopyGlobalBoneTo(transforms);

	model->SetbindingBox();




	for (Material* material : model->Materials())
	{
		material->SetShader(shader);
	}

	rState[0] = new RasterizerState();
	rState[1] = new RasterizerState();
	rState[1]->FillMode(D3D11_FILL_WIREFRAME);

	scale = D3DXVECTOR3(1, 1, 1);
	rotation = D3DXVECTOR3(0, 0, 0);
	position = D3DXVECTOR3(0, 0, 0);

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

	

	for (CubeStruct* box : model->BindingBox())
	{
		D3DXMATRIX pGlobal = model->BoneByName(box->PartsName)->Global();
		box->meshCubeParts->Matrix(pGlobal);
	}
}

void GameModel::SRT(wstring name)
{
	
	D3DXMATRIX S, R, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	ModelBone* Bones = model->BoneByName(name);
	Bones->Local(S * R * T * model->BoneByName(name)->Local());

	scale = D3DXVECTOR3(1, 1, 1);
	rotation = D3DXVECTOR3(0, 0, 0);
	position = D3DXVECTOR3(0, 0, 0);

}

void GameModel::Update()
{

	UpdateWorld();



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



	rState[1]->RSSetState();
	for (CubeStruct* cube : model->BindingBox())
	{
		cube->meshCubeParts->Render();
		//cube->meshCube->Render();
	}
	rState[0]->RSSetState();




	D3DXVECTOR3 Speed(1, 1, 1);

	if (Keyboard::Get()->Press(VK_UP))
		scale += Speed * 0.5f * Time::Delta();
	if (Keyboard::Get()->Press(VK_DOWN))
		scale -= Speed * 0.5f * Time::Delta();
	if (Keyboard::Get()->Press(VK_LEFT))
		rotation += Speed * 0.5f * Time::Delta();
	if (Keyboard::Get()->Press(VK_RIGHT))
		rotation -= Speed * 0.5f * Time::Delta();

	if (Keyboard::Get()->Press('A'))
		position.x += Speed.x * 50.5f * Time::Delta();
	if (Keyboard::Get()->Press('D'))
		position.x -= Speed.x * 50.5f * Time::Delta();
	if (Keyboard::Get()->Press('Q'))
		position.y += Speed.y * 50.5f * Time::Delta();
	if (Keyboard::Get()->Press('E'))
		position.y-= Speed.y * 50.5f * Time::Delta();
	if (Keyboard::Get()->Press('W'))
		position.z += Speed.z * 50.5f * Time::Delta();
	if (Keyboard::Get()->Press('S'))
		position.z -= Speed.z * 50.5f * Time::Delta();

}

Model * GameModel::GetModel()
{
	return model;
}

