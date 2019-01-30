#include "stdafx.h"
#include "TestOBB.h"
#include "./Objects/GameModel.h"


TestOBB::TestOBB(ExecuteValues * values)
	: Execute(values)
{
	
	shader = new Shader(Shaders + L"011_Model.hlsl");

	tank = new GameModel
	(
		shader,
		Models + L"/tank/", L"tank.material",
		Models + L"/tank/", L"tank.mesh"
	);

	temp = L"tank_geo";

	MeshCubeParts* cube = tank->GetModel()->BindingBox()[0]->meshCubeParts;
	obbcheck = false;
	Obb = new OBB(&ray, cube);



}

TestOBB::~TestOBB()
{
	

}

void TestOBB::Update()
{


	D3DXVECTOR3 Ss; // 카메라 위치
	D3DXMATRIX V, P;
	D3DXMATRIX W;

	values->MainCamera->Position(&Ss);
	values->MainCamera->Matrix(&V);
	values->Perspective->GetMatrix(&P);

	D3DXMatrixIdentity(&W);
	
	values->Viewport->GetRay(&ray, Ss, W, V, P);
	// ray.position = 카메라위치, ray.direction 찍은위치

	Model* tankModel = tank->GetModel();

	if (Mouse::Get()->Down(0))
	{

		for (auto& exe : tankModel->BindingBox())
		{
			float u, v, distance;

			W = tankModel->BoneByName(exe->PartsName)->Global();

			values->Viewport->GetRay(&ray, Ss, W, V, P);

			
			for (int i = 0; i < 12; i++)
			{
				UINT index[3];
				index[0] = exe->meshCubeParts->Getindices()[i * 3 + 0];
				index[1] = exe->meshCubeParts->Getindices()[i * 3 + 1];
				index[2] = exe->meshCubeParts->Getindices()[i * 3 + 2];
			

				D3DXVECTOR3 p[3];
				for (int k = 0; k < 3; k++)
					p[k] = exe->meshCubeParts->vertice()[index[k]].Position;

				if (D3DXIntersectTri(&p[0], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance))
				{
					temp = exe->PartsName;
				}

			}


		}

	}

	//W = tankModel->BoneByIndex(0)->Global();

	//values->Viewport->GetRay(&ray, Ss, W, V, P);

	//Obb->SetRay(&ray);
	//obbcheck = Obb->InterSectRay();


	tank->SRT(temp); //
	
	tank->Update();

	
}

void TestOBB::Render()
{

	ImGui::Text(String::ToString(temp).c_str());
	ImGui::Checkbox("obb check", &obbcheck);

	tank->Render();

}

