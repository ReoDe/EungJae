#include "stdafx.h"
#include "TestModel.h"
#include "./Environment/Sky.h"



TestModel::TestModel()
{

	sky = new Sky();


	floor = (Textures + L"Floor.png");
	stone = (Textures + L"Stones.png");
	brick = (Textures + L"Bricks.png");



	// ½¦ÀÌ´õ ÀÌ¸§
	wstring shaderFile = Shaders + L"024_Lighting.fx";

	// ¹Ú½º
	boxMaterial = new Material(shaderFile);
	box = new MeshCube(boxMaterial, 1, 1, 1);
	boxMaterial->SetDiffuseMap(stone);
	boxMaterial->SetAmbient(1.0f, 1.0f, 1.0f);
	boxMaterial->SetDiffuse(1.0f, 1.0f, 1.0f);
	boxMaterial->SetSpecular(0.8f, 0.8f, 0.8f, 16.0f);


	box->Position(0, 0.5f, 0);
	box->Scale(3.0f, 1.0f, 3.0f);



	// ¹Ù´Ú
	
	planeMaterial = new Material(shaderFile);
	plane = new MeshGrid(planeMaterial,60,40,20,30,3,3);
	planeMaterial->SetDiffuseMap(floor);
	planeMaterial->SetAmbient(0.8f, 0.8f, 0.8f);
	planeMaterial->SetDiffuse(0.8f, 0.8f, 0.8f);
	planeMaterial->SetSpecular(0.8f, 0.8f, 0.5f, 16.0f);




	// ±âµÕ

	cylinderMaterial = new Material(shaderFile);
	cylinderMaterial->SetDiffuseMap(brick);
	cylinderMaterial->SetAmbient(1.0f, 1.0f, 1.0f);
	cylinderMaterial->SetDiffuse(1.0f, 1.0f, 1.0f);
	cylinderMaterial->SetSpecular(0.8f, 0.8f, 0.5f, 16.0f);







	sphereMaterial = new Material(shaderFile);
	sphereMaterial->SetDiffuseMap(stone);
	sphereMaterial->SetAmbient(0.6f, 0.8f, 0.9f);
	sphereMaterial->SetDiffuse(0.6f, 0.8f, 0.9f);
	sphereMaterial->SetSpecular(0.9f, 0.9f, 0.9f, 16.0f);




	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(cylinderMaterial, 0.5f, 0.3f, 3.0f, 20, 20);
		cylinder[i * 2]->Position(-5.0f, 1.5f, -10.0f + i * 5.0f);

		cylinder[i * 2 + 1] = new MeshCylinder(cylinderMaterial, 0.5f, 0.3f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->Position(5.0f, 1.5f, -10.0f + i * 5.0f);


		sphere[i * 2] = new MeshSphere(sphereMaterial, 0.5f, 20, 20);
		sphere[i * 2]->Position(-5.0f, 3.5f, -10.0f +i * 5.0f);

		sphere[i * 2 + 1] = new MeshSphere(sphereMaterial, 0.5f, 20, 20);
		sphere[i * 2 + 1]->Position(5.0f, 3.5f, -10.0f + i * 5.0f);



	}


	//

	tank = new GameModel
	(
		Shaders + L"024_Lighting.fx",
		Models + L"Tank/", L"Tank.material",
		Models + L"Tank/", L"Tank.mesh"
	);
	tank->Position(0, 1.0f, 0);
	tank->Scale(0.4f, 0.4f, 0.4f);

	tank->SetDiffuse(0.8f, 0.8f, 0.8f);
	tank->SetSpecular(0.8f, 0.8f, 0.8f, 16.0f);

	for (ModelMesh* mesh : tank->GetModel()->Meshes())
		mesh->Pass(1);
	
}

TestModel::~TestModel()
{
	SAFE_DELETE(tank);

	SAFE_DELETE(boxMaterial);
	SAFE_DELETE(box);
	SAFE_DELETE(planeMaterial);
	SAFE_DELETE(plane);

	SAFE_DELETE(cylinderMaterial);
	for (MeshCylinder* exe : cylinder)
		SAFE_DELETE(exe);
	SAFE_DELETE(sphereMaterial);
	for (MeshSphere* exe : sphere)
		SAFE_DELETE(exe);
}

void TestModel::Update()
{
	sky->Update();
	tank->Update();
}

void TestModel::Render()
{
	sky->Render();

	plane->Render();
	box->Render();

	for (MeshCylinder* exe : cylinder)
		exe->Render();
	for (MeshSphere* exe : sphere)
		exe->Render();

	tank->Render();
}

