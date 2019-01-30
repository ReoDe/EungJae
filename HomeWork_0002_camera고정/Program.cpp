#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"
#include "./Viewer/Pixed.h"


#include "./Executes/DrawGrid.h"
#include "./Executes/DrawRect.h"
#include "./Executes/DrawCube.h"

Program::Program()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new ExecuteValues();
	values->ViewProjection = new ViewProjectionBuffer();
	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);

	//values->MainCamera = new Freedom(); // 메인카메라 받기 분석좀
	values->PixedCamera = new Pixed(); // 고정카메라



	// 그리기
	//executes.push_back(new DrawTriangle(values));
	executes.push_back(new DrawGrid(values));
	//executes.push_back(new DrawRect(values));
	executes.push_back(new DrawCube(values));



	//values->PixedCamera->SetForward()
}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	SAFE_DELETE(values->ViewProjection);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->Viewport);

	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values->PixedCamera);

	SAFE_DELETE(values);
}

void Program::Update()
{
	//values->PixedCamera->SetForward(D3DXVECTOR3(0,1,0));
	
	//D3DXVECTOR3 cube = executes.at(1)->XYZ();
	//values->PixedCamera->Setat(cube);


	D3DXVECTOR3 at;
	values->PixedCamera->At(&at);

	at = executes.at(1)->XYZ();





	//values->MainCamera->Update();
	values->PixedCamera->Update();
	



	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	
	//static D3DXVECTOR3 position(50, 10, -5);
	//ImGui::SliderFloat2("Camera", (float*)&position, 0, 200);
	//ImGui::SliderFloat("Camera Z", (float*)&position.z, -1, -20);


	//D3DXVECTOR3 forward(0, 0, 1);
	//D3DXVECTOR3 right(1, 0, 0);
	//D3DXVECTOR3 up(0, 1, 0);

	//// left hand 좌표계
	////                        내 눈의 위치,  바라볼 위치
	//// 지금 000에서 001쪽으로 바라보는중
	//D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up);


	

	D3DXMATRIX V, Pixed, P;
	//values->MainCamera->Matrix(&V);
	values->PixedCamera->Matrix(&Pixed);
	values->Perspective->GetMatrix(&P);


	// 글로벌 버퍼에 퓨 프로젝션 버퍼
	//values->ViewProjection->SetView(V);
	values->ViewProjection->SetView(Pixed);
	values->ViewProjection->SetProjection(P);
	values->ViewProjection->SetVSBuffer(0);

	for (Execute* exe : executes)
		exe->Render();

	values->PixedCamera->Render();

	//executes.at(0)->Render();

}

void Program::PostRender()
{
	for (Execute* exe : executes)
		exe->PostRender();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values->Perspective->Set(desc.Width, desc.Height);
	values->Viewport->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();

	
}