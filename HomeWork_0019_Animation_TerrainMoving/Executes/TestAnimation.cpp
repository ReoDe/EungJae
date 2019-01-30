#include "stdafx.h"
#include "TestAnimation.h"
#include "Fbx/FbxLoader.h"
#include "Environment\Terrain.h"

TestAnimation::TestAnimation()
	:position(0,0,0), charactorPosition(0,0,0), charactorPositionDirection(0,0,0),
	charactorRotation(0,0,0), charactorSpeed(5.0f), charactorScale(0.01f,0.01f,0.01f)
{

	terrainMaterial = new Material(Shaders + L"028_Terrain.fx");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");
	terrain = new Terrain(terrainMaterial, Textures + L"HeightMap256.png");

	charactorState = STATE_Idle;

	FbxLoader* loader = NULL;
	vector<wstring> clipList;

	//loader = new FbxLoader
	//(
	//	Assets + L"Paladin/Mesh.fbx",
	//	Models + L"Paladin/", L"Paladin"
	//);
	//loader->ExportMaterial();
	//loader->ExportMesh();

	//loader->ExportAnimation(0);
	//
	//loader->GetClipList(&clipList);
	//SAFE_DELETE(loader);

	//gameModel = new GameModel
	//(
	//	Shaders + L"039_Model.fx",
	//	Models + L"Paladin/Paladin.material",
	//	Models + L"Paladin/Paladin.mesh"

	//);

	//loader = new FbxLoader
	//(
	//	Assets + L"Kachujin/Mesh.fbx",
	//	Models + L"Kachujin/", L"Kachujin"
	//);
	//loader->ExportMaterial();
	//loader->ExportMesh();

	//SAFE_DELETE(loader);

	//loader = new FbxLoader
	//(
	//	Assets + L"Kachujin/Running.fbx",
	//	Models + L"Kachujin/", L"Running"
	//);
	//loader->ExportAnimation(0);

	//SAFE_DELETE(loader);

	/*gameModel = new GameAnimator
	(
		Shaders + L"039_Model.fx",
		Models + L"Kachujin/Kachujin.material",
		Models + L"Kachujin/Kachujin.mesh"
	);

	gameModel->AddClip(Models + L"Kachujin/Taunt.animation");
	gameModel->AddClip(Models + L"Kachujin/Running.animation");
	gameModel->ClipNumber(0);
	gameModel->Position(10, 0, 10);
	gameModel->Scale(charactorScale.x, charactorScale.y, charactorScale.z);*/





}

TestAnimation::~TestAnimation()
{
	//SAFE_DELETE(gameModel);
	SAFE_DELETE(terrain);
}

void TestAnimation::Update()
{
	terrain->Update();

	//D3DXVECTOR3 position;
	//if (terrain->GetHeight(position) == true)
	//	this->position = position;

	//{// 캐릭터 움직임

	//	if (charactorPosition.x - charactorPositionDirection.x > 0.1f ||
	//		charactorPosition.x - charactorPositionDirection.x < -0.1f)
	//		// 움직이는중
	//	{
	//		terrain->MovePoint(&charactorPosition, &charactorPositionDirection, charactorSpeed, charactorRotation);

	//		charactorState = STATE_Running;
	//	}
	//	else
	//	{
	//		charactorState = STATE_Idle;
	//	}

	//	if (Mouse::Get()->Press(0))
	//	{
	//		charactorPositionDirection = terrain->GetPickingMouseY();
	//		// 클릭값을 원하는 방향이
	//	}

	//	charactorPosition.y = terrain->GetHeightY(charactorPosition);

	//	gameModel->Scale(charactorScale);
	//	gameModel->Rotation(charactorRotation);
	//	gameModel->Position(charactorPosition);

	//}



	////gameModel->Position(position);

	//switch (charactorState)
	//{
	//case TestAnimation::STATE_Idle:
	//	gameModel->ClipNumber(STATE_Idle);
	//	break;
	//case TestAnimation::STATE_Running:
	//	gameModel->ClipNumber(STATE_Running);
	//	break;
	//default:
	//	break;
	//}

	//gameModel->Update();


}

void TestAnimation::PreRender()
{
	
}

void TestAnimation::Render()
{
	//gameModel->Render();
	terrain->Render();

	/*ImGui::SliderFloat("Speed", &charactorSpeed, 0, 25);
	ImGui::SliderFloat3("Scale", (float*)&charactorScale, 0, 1);*/
	

}