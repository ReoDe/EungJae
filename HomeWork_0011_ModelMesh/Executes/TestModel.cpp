#include "stdafx.h"
#include "TestModel.h"
#include "./Objects/GameModel.h"

TestModel::TestModel(ExecuteValues * values)
	: Execute(values)
{
	shader = new Shader(Shaders + L"014_ModelDiffuse.hlsl");

	tank = new GameModel
	(
		shader,
		Models + L"/tank/", L"tank.material",
		Models + L"/tank/", L"tank.mesh"
	);

	//

	for (int i = 0; i < 12; i++)
	{
		location[i] = { 0,0,0 };
		rotation[i] = { 0,0,0 };
		size[i] = { 1,1,1 };
		turretTransform[i] = tank->GetModel()->BoneByIndex(i)->Local();
	}
	pickingIndex = 0;
}

TestModel::~TestModel()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(tank);

}

void TestModel::Update()
{
	D3DXMATRIX S;
	D3DXMatrixScaling(&S, size[pickingIndex].x, size[pickingIndex].y, size[pickingIndex].z);

	D3DXMATRIX Rx;
	D3DXMatrixRotationX(&Rx, rotation[pickingIndex].x);
	D3DXMATRIX Ry;
	D3DXMatrixRotationY(&Ry, rotation[pickingIndex].y);
	D3DXMATRIX Rz;
	D3DXMatrixRotationZ(&Rz, rotation[pickingIndex].z);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, location[pickingIndex].x, location[pickingIndex].y, location[pickingIndex].z);

	ModelBone* turretBone = tank->GetModel()->BoneByIndex(pickingIndex);
	//modelMesh->PickingBox(Index);
	turretBone->Local(S*Rx*Ry*Rz*T*turretTransform[pickingIndex]);

	tank->UpdateWorld();
	tank->Update();
	
	
}

void TestModel::Render()
{
	ImGui::SliderInt("Index", &pickingIndex, 0, 11);

	ImGui::SliderFloat("sizeX", &size[pickingIndex].x, 0.3f, 4.0f);
	ImGui::SliderFloat("sizeY", &size[pickingIndex].y, 0.3f, 4.0f);
	ImGui::SliderFloat("sizeZ", &size[pickingIndex].z, 0.3f, 4.0f);

	ImGui::SliderFloat("locationX", &location[pickingIndex].x, -500, 500);
	ImGui::SliderFloat("locationY", &location[pickingIndex].y, -500, 500);
	ImGui::SliderFloat("locationZ", &location[pickingIndex].z, -500, 500);

	ImGui::SliderFloat("rotationX", &rotation[pickingIndex].x, -2 * Math::PI, 2 * Math::PI);
	ImGui::SliderFloat("rotationY", &rotation[pickingIndex].y, -2 * Math::PI, 2 * Math::PI);
	ImGui::SliderFloat("rotationZ", &rotation[pickingIndex].z, -2 * Math::PI, 2 * Math::PI);
	tank->Render();


}
