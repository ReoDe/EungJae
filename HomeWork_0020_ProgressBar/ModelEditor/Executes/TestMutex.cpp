#include "stdafx.h"
#include "TestMutex.h"
#include "Fbx/FbxLoader.h"
#include "Renders/Progress.h"

#include "Renders/Render2D.h"

void TestMutex::Initialize()
{

	FbxLoader* loader = NULL;

	loader = new FbxLoader
	(
		Assets + L"Kachujin/Mesh.fbx",
		Models + L"Kachujin/", L"Kachujin"
	);
	loader->ExportMaterial();
	Progress::Get()->Rate(0.10f);
	Progress::Get()->RateWidth(0.10f);

	loader->ExportMesh();
	Progress::Get()->Rate(0.40f);
	Progress::Get()->RateWidth(0.40f);
	SAFE_DELETE(loader);

	loader = new FbxLoader
	(
		Assets + L"Kachujin/Idle.fbx",
		Models + L"Kachujin/", L"Idle"
	);
	Progress::Get()->Rate(0.80f);
	Progress::Get()->RateWidth(0.80f);
	loader->ExportAnimation(0);
	Progress::Get()->Rate(1.0f);
	Progress::Get()->RateWidth(1.0f);
	SAFE_DELETE(loader);
}

void TestMutex::Ready()
{
	gameModel = new GameAnimator
	(
		Shaders + L"039_Model.fx",
		Models + L"Kachujin/Kachujin.material",
		Models + L"Kachujin/Kachujin.mesh"
	);
	gameModel->AddClip(Models + L"Kachujin/Idle.animation");
}

void TestMutex::Destroy()
{
	SAFE_DELETE(gameModel);
}

void TestMutex::Update()
{

	gameModel->Update();
	
}

void TestMutex::PreRender()
{

}

void TestMutex::Render()
{

	gameModel->Render();

}

