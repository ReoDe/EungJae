#include "stdafx.h"
#include "TestBillboard.h"
#include "../Environment/Terrain.h"
#include "../Objects/GameModel.h"
#include "../Environment/Sky.h"

TestBillboard::TestBillboard(ExecuteValues * values)
	: Execute(values)
{
	terrainMaterial = new Material(Shaders + L"020_TerrainGrid.hlsl");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");

	terrain = new Terrain(values, terrainMaterial, Textures + L"HeightMap256.png");

	sky = new Sky(values);

	//
	//buffer = new Buffer();
	billboardMaterial = new Material(Shaders + L"019_BillBoard.hlsl");
	billboardMaterial->SetDiffuseMap(Textures + L"Tree.png");

	for (int i = 0; i < 30; i++)
	{
		MeshQuad* quad = new MeshQuad(billboardMaterial, 1, 1);

		D3DXVECTOR3 scale;
		scale.x = Math::Random(5.0f, 15.0f);
		scale.y = Math::Random(5.0f, 15.0f);
		scale.z = 1.0f;

		D3DXVECTOR3 position;
		position.x = Math::Random(0.0f, 255.f);
		position.z = Math::Random(0.0f, 255.f);
		position.y = terrain->GetHeight(position.x, position.z);
		position.y += scale.y * 0.5f;

		quad->Scale(scale);
		quad->Position(position);

		billboards.push_back(quad);
	}

	Position = D3DXVECTOR3(0, 0, 0);
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
	if (terrain->GetHeight(position) == true) {}
		//this->Position = position;

	sky->Update();
	terrain->Update();


	D3DXMATRIX V;
	// view 얻어옴
	values->MainCamera->Matrix(&V);
	D3DXMatrixInverse(&V, NULL, &V);
	//V._41 = V._42 = V._43 = 0.0f;
	//buffer->Billboard(V);

	// D3DXMatrixDecompose(), 쿼터니온, 4원수회전방식, r 세타, 파이
	float x = asin(-V._32);
	float y = atan2(V._31, V._33);
	float z = atan2(V._12, V._22);

	for (MeshQuad* quad : billboards)
		quad->Rotation(x, y, z);

}

void TestBillboard::Render()
{

	sky->Render();
	terrain->Render();

	//buffer->SetVSBuffer(10);

	for (MeshQuad* quad : billboards)
		quad->Render();


}
