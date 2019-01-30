#include "stdafx.h"
#include "Test.h"
#include "./Environment/Sky.h"

#include "./Viewer/Freedom.h"
#include "./Viewer/Viewport.h"
#include "./Viewer/Perspective.h"


Test::Test() : tankPos(0, 0, 0), MyPos(0, 0), tempVec(0, 0, 0), FirstClick(0, 0,0), EndClick(0, 0, 0)
	, PickingNum(4)
{

	sky = new Sky();

	//
	
	tank = new GameModel
	(
		Shaders + L"HomeWork_0013.fx",
		Models + L"Tank/", L"Tank.material",
		Models + L"Tank/", L"Tank.mesh"
	);
	tank->Position(tankPos);
	tank->Scale(1,1,1);
	tank->SetDiffuse(0.8f, 0.8f, 0.8f);
	tank->SetSpecular(0.8f, 0.8f, 0.8f, 16.0f);
	



	for (ModelMesh* mesh : tank->GetModel()->Meshes())
		mesh->Pass(1);



	// �ʱ� ����

	tankModel = tank->GetModel();
	for (auto& exe : tankModel->SphereBox())
		exe->meshSphere->Pass(0);

	temp = L"";
	
}

Test::~Test()
{
	SAFE_DELETE(tank);
	SAFE_DELETE(sky);
}

void Test::Update()
{
	sky->Update();
	tank->Update();

	// 

	D3DXVECTOR3 Ss; // ī�޶� ��ġ
	D3DXMATRIX V, P;
	D3DXMATRIX W;

	Context::Get()->GetMainCamera()->Position(&Ss);
	Context::Get()->GetMainCamera()->Matrix(&V);
	Context::Get()->GetPerspective()->GetMatrix(&P);

	D3DXMatrixIdentity(&W);

	Context::Get()->GetViewport()->GetRay(&ray, Ss, W, V, P);
	// ray.position = ī�޶���ġ, ray.direction ������ġ



	if (Mouse::Get()->Press(0))
	{

		for (auto& exe : tankModel->SphereBox())
		{
			float u, v, distance;

			W = tankModel->BoneByName(exe->PartsName)->Global();

			Context::Get()->GetViewport()->GetRay(&ray, Ss, W, V, P);
			


			for (int i = 0; i < exe->meshSphere->GetindicesCount() / 3; i++)
			{
				UINT index[3];
				index[0] = exe->meshSphere->Getindices()[i * 3 + 0];
				index[1] = exe->meshSphere->Getindices()[i * 3 + 1];
				index[2] = exe->meshSphere->Getindices()[i * 3 + 2];


				D3DXVECTOR3 p[3];
				for (int k = 0; k < 3; k++)
					p[k] = exe->meshSphere->vertice()[index[k]].Position;

				if (D3DXIntersectTri(&p[0], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance))
				{
					temp = exe->PartsName;
				}

			}


		}

	} // if
	else
		temp.clear();
	

	if (Keyboard::Get()->Press(VK_UP))
	{
		tankPos.x += 5.0f * Time::Delta();
	}
	if (Keyboard::Get()->Press(VK_RIGHT))
	{
		tankPos.z += 5.0f * Time::Delta();
	}
	tank->Position(tankPos);


	// ��Ƽ ��ŷ

	 ClickMouse(PickingNum); // ���콺 Ŭ�������� �������� ��ǥ ����

}

void Test::Render()
{
	ImGui::Text(String::ToString(temp).c_str());
	ImGui::SliderFloat3("Direction", (float*)&ray.Direction, 0, 1000);
	ImGui::SliderFloat3("Tanseform Camera Pos", (float*)&ray.Position, 0, 1000);
	D3DXVECTOR3 Ss;
	Context::Get()->GetMainCamera()->Position(&Ss);
	ImGui::SliderFloat3("Camera Pos", (float*)&Ss, 0, 1000);
	ImGui::Separator();


	// ��Ƽ ��ŷ
	int Vectorsize = multiPicking.size();
	ImGui::SliderInt("multiPicking.size", &Vectorsize, 0, 100);
	
	if (Vectorsize != 0)
	{
		for (int i = 0; i < Vectorsize; i++)
		{
			//ImGui::SliderFloat3("Test" - i, (float*)&test.at(i), 0, 1280);
			ImGui::Text(String::ToString(multiPicking.at(i)).c_str());
		}
	}

	ImGui::SliderInt("MultiPicking Num^2", &PickingNum, 0, 15);



	sky->Render();

	tank->Render();

}

void Test::PostRender()
{

}

void Test::ClickMouse(int Ratio) // ���е�
{
	if (Mouse::Get()->Down(0))
	{
		FirstClick = Mouse::Get()->GetPosition();
		multiPicking.clear();
	}
	else if (Mouse::Get()->Up(0))
	{
		EndClick = Mouse::Get()->GetPosition();


		int Width = abs((int)FirstClick.x - (int)EndClick.x);
		int Height = abs((int)FirstClick.y - (int)EndClick.y);

		int ratio = Ratio;

		int dx = Width / ratio; // ���μ��� ����
		int dy = Height / ratio;

		D3DXVECTOR3 startPoint; // ������
		startPoint.x = min(FirstClick.x, EndClick.x);
		startPoint.y = min(FirstClick.y, EndClick.y);
		startPoint.z = 0;

		D3DXVECTOR3 ConstPoint = startPoint;
		// ray ����



		D3DXVECTOR3 Ss; // ī�޶� ��ġ
		D3DXMATRIX V, P;
		D3DXMATRIX W;

		Context::Get()->GetMainCamera()->Position(&Ss); // ī�޶� 
		Context::Get()->GetMainCamera()->Matrix(&V);
		Context::Get()->GetPerspective()->GetMatrix(&P);

		D3DXMatrixIdentity(&W);


		wstring str; // �ӽ� �̸�


		if (ConstPoint.x != 0)
		{
			for (int q = 0; q < ratio; q++) // ����
			{
				for (int w = 0; w < ratio; w++) // ����
				{
					ConstPoint.x = startPoint.x + dx * w;
					ConstPoint.y = startPoint.y + dy * q;

					//test.push_back(ConstPoint);


					for (auto& exe : tankModel->SphereBox())
						{
							float u, v, distance;

							W = tankModel->BoneByName(exe->PartsName)->Global();

							// ���⼭ ray �� ����
							Context::Get()->GetViewport()->GetRayMouse(ConstPoint, &Multiray, Ss, W, V, P);



							for (int i = 0; i < exe->meshSphere->GetindicesCount() / 3; i++)
							{
								UINT index[3];
								index[0] = exe->meshSphere->Getindices()[i * 3 + 0];
								index[1] = exe->meshSphere->Getindices()[i * 3 + 1];
								index[2] = exe->meshSphere->Getindices()[i * 3 + 2];


								D3DXVECTOR3 p[3];
								for (int k = 0; k < 3; k++)
									p[k] = exe->meshSphere->vertice()[index[k]].Position;

								if (D3DXIntersectTri(&p[0], &p[1], &p[2], &Multiray.Position, &Multiray.Direction, &u, &v, &distance))
								{
									str = exe->PartsName;

									if (multiPicking.size() > 0)
									{
										bool istrue = true;

										// ����Ž��
										for (int i = 0; i < multiPicking.size(); i++)
										{
											if (multiPicking.at(i) == str)
												istrue = false; // �ߺ��Ǵ°��� ������ false
										}

										if (istrue) multiPicking.push_back(str);


									}
									else multiPicking.push_back(str);
								

									//temp = exe->PartsName;
								}

							}


						}// for(tankmodel->sphere)




				} // w
			} // q

		}// if


		FirstClick = D3DXVECTOR3(0, 0, 0);
		EndClick = D3DXVECTOR3(0, 0, 0);
		str = L"";
	}

	
	

}

