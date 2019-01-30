#include "stdafx.h"
#include "TestBrush.h"
#include "../Environment/Terrain.h"
#include "../Objects/GameModel.h"

TestBrush::TestBrush(ExecuteValues * values)
	: Execute(values)
{
	terrainMaterial = new Material(Shaders + L"017_TextureSplatting.hlsl");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");

	terrain = new Terrain(values, terrainMaterial, Textures + L"HeightMap256.png");

	position = D3DXVECTOR3(0, 0, 0);


}

TestBrush::~TestBrush()
{
	//SAFE_DELETE(terrainMaterial);
	//SAFE_DELETE(terrain);

}

void TestBrush::Update()
{
	D3DXVECTOR3 position(0,0,0);
	if (terrain->GetHeight(position) == true)
		this->position = position;

	terrain->Update();

}

void TestBrush::Render()
{
	//ImGui::InputFloat3("Picked Position", (float*)&position);

	terrain->Render();

}
