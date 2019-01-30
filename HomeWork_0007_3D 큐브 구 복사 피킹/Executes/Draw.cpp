#include "stdafx.h"
#include "Draw.h"


Draw::Draw(ExecuteValues * values)
	: Execute(values), CubeSelect(0), SphereSelect(0)
{
	rasterizerState[0] = new RasterizerState;
	rasterizerState[1] = new RasterizerState;
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

	// ray
	ray = new Ray();
	
	// 기본적으로 1개씩
	InsertCube();
	InsertSphere();

}

Draw::~Draw()
{

	for (CubeStruct* Scube : Cube)
	{
		SAFE_DELETE(Scube)
	}

	for (SphereStruct* Ssphere : Sphere)
	{
		SAFE_DELETE(Ssphere)
	}
}

void Draw::Update()
{
	

	D3DXMATRIX V, P;
	values->MainCamera->Matrix(&V);
	values->Perspective->GetMatrix(&P);

	D3DXVECTOR3 S;
	values->MainCamera->Position(&S);




	D3DXMATRIX W;

	for (CubeStruct* Scube : Cube)
	{
		// 큐브 이동
		{
			Scube->cubeMesh->Scale(Scube->Scale);
			Scube->cubeMesh->Position(Scube->Position);
			Scube->cubeMesh->Rotation(Scube->Rotation);

			Scube->cubeMesh->Matrix(&W);
			values->Viewport->GetRay(ray, S, W, V, P);



			float result;
			Scube->cubeTest = Scube->cubeBounding->Intersect(ray, result);
			Scube->cubeLine->Draw(W, Scube->cubeBounding);

			if (Mouse::Get()->Down(0) && Scube->cubeBounding->Intersect(ray, result))
				CubeSelect = Scube->Number;
		}
	}

	for (SphereStruct* Ssphere : Sphere)
	{
		// 구 이동
		{
		
			Ssphere->sphereMesh->Scale(Ssphere->radius, Ssphere->radius, Ssphere->radius);

			Ssphere->sphereMesh->Position(Ssphere->Center);
			Ssphere->sphereBounding->Center = Ssphere->Center;

			Ssphere->sphereMesh->Matrix(&W);
			values->Viewport->GetRay(ray, S, W, V, P);

			float result;
			Ssphere->sphereTest = Ssphere->sphereBounding->Intersect(ray, result);
			Ssphere->sphereLine->Draw(W, Ssphere->sphereBounding);

			if (Mouse::Get()->Down(0) && Ssphere->sphereBounding->Intersect(ray, result))
				SphereSelect = Ssphere->Number;
		}
	}
	


}

void Draw::Render()
{
	//큐브 랜더
	{
		// 벡터 추가
	
		if (ImGui::Button("CButton")) InsertCube();
		float vectorSize = Cube.size();
		ImGui::SliderInt("CubeSelect", &CubeSelect, 0, vectorSize - 1);


		ImGui::SliderFloat3("Cscale", (float*)&Cube.at(CubeSelect)->Scale, 0, 5);
		ImGui::SliderFloat3("CPosition", (float*)&Cube.at(CubeSelect)->Position, -5, 5);
		ImGui::SliderFloat3("CRotation", (float*)&Cube.at(CubeSelect)->Rotation, -3.13, 3.14);
		ImGui::Checkbox("Ccheck", &Cube.at(CubeSelect)->cubeTest);


		for (CubeStruct* Scube : Cube)
		{
			Scube->cubeMesh->Render();
			Scube->cubeLine->Render();
		}
	}
	
	
	// 구 랜더
	{
	
		if (ImGui::Button("SButton")) InsertSphere();
		float vectorSize = Sphere.size();
		ImGui::SliderInt("SphereSelect", &SphereSelect, 0, vectorSize - 1);


		ImGui::SliderFloat("Sradius", (float*)&Sphere.at(SphereSelect)->radius, 0, 5);
		ImGui::SliderFloat3("SPosition", (float*)&Sphere.at(SphereSelect)->Center, -5, 5);
		ImGui::Checkbox("Scheck", &Sphere.at(SphereSelect)->sphereTest);

		for (SphereStruct* Ssphere : Sphere)
		{
			Ssphere->sphereMesh->Render();
			Ssphere->sphereLine->Render(); 
		}
	}
}

void Draw::InsertCube()
{
	CubeStruct* cube = new CubeStruct();

	cube->cubeMaterial = new Material(Shaders + L"015_Mesh.hlsl");
	cube->cubeMaterial->SetDiffuseMap(Textures + L"Red.png");

	cube->cubeMesh = new MeshCube(cube->cubeMaterial, 1, 1, 1);

	cube->cubeBounding = new BBox();
	cube->cubeBounding->Min = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	cube->cubeBounding->Max = D3DXVECTOR3(+0.5f, +0.5f, +0.5f);
	cube->cubeLine = new DebugLine();

	cube->Scale = D3DXVECTOR3(1, 1, 1);
	cube->Position = D3DXVECTOR3(0, 0, 0);
	cube->Rotation = D3DXVECTOR3(0, 0, 0);


	cube->Number = (int)Cube.size(); // 사이즈가 곧 넘버

	Cube.push_back(cube);


}

void Draw::InsertSphere()
{
	SphereStruct* sphere = new SphereStruct();

	sphere->sphereMaterial = new Material(Shaders + L"015_Mesh.hlsl");
	sphere->sphereMaterial->SetDiffuseMap(Textures + L"Green.png");

	sphere->sphereMesh = new MeshSphere(sphere->sphereMaterial, 1);
	sphere->sphereMesh->SetRadius(1);

	sphere->sphereBounding = new BSphere();
	sphere->sphereBounding->Center = D3DXVECTOR3(0, 0, 0);
	sphere->sphereBounding->Radius = 1.0f;
	
	sphere->sphereLine = new DebugLine();

	sphere->radius = 1.0f;
	sphere->Center = D3DXVECTOR3(0, 0, 0);

	sphere->Number = (int)Sphere.size(); // 사이즈가 곧 넘버

	Sphere.push_back(sphere);
}


