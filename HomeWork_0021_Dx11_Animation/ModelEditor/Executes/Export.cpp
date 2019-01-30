#include "stdafx.h"
#include "Export.h"
#include "Fbx/FbxLoader.h"



void Export::Initialize()
{
	clipNumber = 0;






}

void Export::Ready() 
{
	FbxLoader* loader = NULL;
	vector<wstring> clipList;

	//loader = new FbxLoader
	//(
	//	Assets + L"Weapon/greatsword_of_fn.fbx",
	//	Models + L"Weapon/", L"greatsword_of_fn"
	//);
	//loader->ExportMaterial();
	//loader->ExportMesh();

	//loader->ExportAnimation(0);

	//loader->GetClipList(&clipList);
	SAFE_DELETE(loader);




	//gameModel = new GameModel
	//(
	//	Shaders + L"039_Model.fx",
	//	Models + L"Paladin/Paladin.material",
	//	Models + L"Paladin/Paladin.mesh"
	 
	//);


	//메시 추출
	
	//loader = new FbxLoader
	//(
	//	Assets + L"Starkie/Starkie.fbx",
	//	Models + L"Starkie/", L"Starkie"
	//);

	//loader->ExportMaterial();
	//loader->ExportMesh();
	//loader->ExportAnimation(0);
	//loader->GetClipList(&clipList);
	//SAFE_DELETE(loader);



	//애니메이션 추출
	//{
	//	loader = new FbxLoader
	//	(
	//		Assets + L"Motion/Zombie_Kicking.fbx",
	//		Models + L"Motion/", L"Zombie_Kicking"
	//	);
	//	loader->ExportAnimation(0);

	//	SAFE_DELETE(loader);
	//}





	///////////////////////////////////////////////////


	model = new GameModel
	(
		Shaders + L"046_Model.fx",
		Models + L"Weapon/greatsword_of_fn.material",
		Models + L"Weapon/greatsword_of_fn.mesh"
	);
	model->Ready();
	


	///////////////////////////////////////////////////

	gameModel = new GameAnimator
	(
		Shaders + L"046_Model.fx",
		Models + L"Starkie/Starkie.material",
		Models + L"Starkie/Starkie.mesh"
	);

	gameModel->AddClip(Models + L"Motion/Zombie_Kicking.animation");
	gameModel->AddClip(Models + L"Motion/Punching_1.animation");
	gameModel->AddClip(Models + L"Motion/Punching_2.animation");
	gameModel->AddClip(Models + L"Motion/Punching_3.animation");
	gameModel->AddClip(Models + L"Motion/Punching_Idle.animation");

	gameModel->Ready();

	gameModel->ClipNumber(clipNumber);
	gameModel->SetMeshCube(NULL, 30, 30, 30, 7);

	gameModel->Scale(0.01f, 0.01f, 0.01f);
	//gameModel->GetMeshCube().at(0)->cube->Scale(0.01f, 0.01f, 0.01f);
	//gameModel->Position(10, 0, 10);




}

void Export::Destroy()
{
	SAFE_DELETE(gameModel);

}

void Export::Update()
{

	gameModel->ClipNumber(clipNumber);

	gameModel->Update();

	
	model->Update();


}

void Export::PreRender()
{
	ImGui::SliderInt("ClipNumber", &clipNumber, 0, 4);
	ImGui::Separator();

	
}

void Export::Render()
{
	gameModel->Render();


	model->Render();

}