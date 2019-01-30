#include "stdafx.h"
#include "TestModel.h"


TestModel::TestModel()
{
	shader = new Shader( Shaders + L"023_Lighting.fx");

	buuny = new GameModel
	(
		shader,
		Models + L"Meshes/", L"Buuny.material",
		Models + L"Meshes/", L"Buuny.mesh"
	);


	buuny->Scale(0.01f, 0.01f, 0.01f);

	for (Material* material : buuny->GetModel()->Materials())
	{
		material->SetShader(shader);
		material->SetDiffuseMap(Textures + L"White.png");
	}
}

TestModel::~TestModel()
{
	SAFE_DELETE(buuny);
}

void TestModel::Update()
{
	buuny->Update();
}

void TestModel::Render()
{
	buuny->Render();
}

