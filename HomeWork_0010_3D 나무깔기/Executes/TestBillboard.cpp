#include "stdafx.h"
#include "TestBillboard.h"
#include "../Environment/Terrain.h"
#include "../Objects/GameModel.h"
#include "../Environment/Sky.h"

TestBillboard::TestBillboard(ExecuteValues * values)
	: Execute(values)
{
	terrainMaterial = new Material(Shaders + L"0010_HomeWork.hlsl");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");

	terrain = new Terrain(values, terrainMaterial, Textures + L"HeightMap256.png");
	position = D3DXVECTOR3(0, 0, 0);
	//buffer = new Buffer();


	

}

TestBillboard::~TestBillboard()
{
	// 지우는건 나중에

	//SAFE_DELETE(terrainMaterial);
	//SAFE_DELETE(terrain);

}

void TestBillboard::Update()
{
	D3DXVECTOR3 position(0, 0, 0);
	if (terrain->GetHeight(position) == true)
		this->position = position;


	terrain->Update();




}

void TestBillboard::Render()
{


	terrain->Render();




}
