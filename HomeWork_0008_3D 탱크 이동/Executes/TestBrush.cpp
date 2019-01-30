#include "stdafx.h"
#include "TestBrush.h"
#include "../Environment/Terrain.h"
#include "../Objects/GameModel.h"

TestBrush::TestBrush(ExecuteValues * values)
	: Execute(values), tankPosition(0, 0, 0), tankPositionDirection(0, 0, 0)
	, tankRotation(0,0,0), tankRotationtest(0,0,0), tankSpeed(25.0f)
{
	terrainMaterial = new Material(Shaders + L"016_TerrainBrush.hlsl");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");
	terrain = new Terrain(values, terrainMaterial, Textures + L"HeightMap256.png");


	position = D3DXVECTOR3(0, 0, 0);

	//쉐이더
	shader = new Shader(Shaders + L"014_ModelDiffuse.hlsl");

	// 탱크 불러오기
	tank = new GameModel
	(
		shader,
		Models + L"/Tank/", L"Tank.material",
		Models + L"/Tank/", L"Tank.mesh"
	);
	tank->Scale(0.5f, 0.5f, 0.5f);
	tank->Position(23, 0, 63);
	
	// 본 이름
	lFront = tank->GetModel()->BoneByName(L"l_front_wheel_geo")->Local();
	rFront = tank->GetModel()->BoneByName(L"r_front_wheel_geo")->Local();
	lBack = tank->GetModel()->BoneByName(L"l_back_wheel_geo")->Local();
	rBack = tank->GetModel()->BoneByName(L"r_back_wheel_geo")->Local();


}

TestBrush::~TestBrush()
{
	//SAFE_DELETE(terrainMaterial);
	//SAFE_DELETE(terrain);

	SAFE_DELETE(shader);
	SAFE_DELETE(tank);
}

void TestBrush::Update()
{
	D3DXVECTOR3 position;
	if (terrain->GetHeight(position) == true)
		this->position = position;

	terrain->Update();




	// 탱크

	// 움직이는중
	if (tankPosition.x - tankPositionDirection.x > 0.1f ||
		tankPosition.x - tankPositionDirection.x < -0.1f)
	{
		terrain->MovePoint(&tankPosition, &tankPositionDirection, tankSpeed, tankRotation);
		
		Wheel(lFront, rFront, lBack, rBack);
		
	}
		

	if (Mouse::Get()->Press(0))
	{
		tankPositionDirection = terrain->GetPickingMouseY();
		// 클릭값을 원하는 방향이 넣어놈
	}

	tankPosition.y = terrain->GetHeightY(tankPosition);
	


	//tank->Rotation(tankRotationtest);
	tank->Rotation(tankRotation);
	tank->Position(tankPosition);

	// 탱크

	// 움직이는중
	if (tankPosition.x - tankPositionDirection.x > 0.1f ||
		tankPosition.x - tankPositionDirection.x < -0.1f)
	{
		terrain->MovePoint(&tankPosition, &tankPositionDirection, tankSpeed, tankRotation);
		
		Wheel(lFront, rFront, lBack, rBack);
		
	}
		

	if (Mouse::Get()->Press(0))
	{
		tankPositionDirection = terrain->GetPickingMouseY();
		// 클릭값을 원하는 방향이 넣어놈
	}

	tankPosition.y = terrain->GetHeightY(tankPosition);
	


	//tank->Rotation(tankRotationtest);
	tank->Rotation(tankRotation);
	tank->Position(tankPosition);




	tank->Update();
	
	

}

void TestBrush::Render()
{
	ImGui::InputFloat3("Picked Position", (float*)&position);

	terrain->Render();

	// 탱크

	ImGui::SliderFloat3("tank Position", (float*)&tankPosition, 0, 256);
	ImGui::SliderFloat("tank Speed", &tankSpeed, 0, 100);
	//ImGui::SliderFloat3("tank tankRotationtest", (float*)&tankRotationtest, -3.14, 3.14);

	tank->Render();



}

void TestBrush::Wheel(D3DXMATRIX & lFront, D3DXMATRIX & rFront, D3DXMATRIX & lBack, D3DXMATRIX & rBack)
{
	//바퀴회전
	D3DXMATRIX R;
	D3DXMatrixRotationX(&R, -Math::PI * 2 * Time::Get()->Running());

	ModelBone* qq = tank->GetModel()->BoneByName(L"l_front_wheel_geo");
	qq->Local(R * lFront);
	ModelBone* ww = tank->GetModel()->BoneByName(L"r_front_wheel_geo");
	ww->Local(R * rFront);
	ModelBone* ee = tank->GetModel()->BoneByName(L"l_back_wheel_geo");
	ee->Local(R * lBack);
	ModelBone* rr = tank->GetModel()->BoneByName(L"r_back_wheel_geo");
	rr->Local(R * rBack);




}


