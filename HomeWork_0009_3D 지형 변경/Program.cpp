#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"

#include "./Executes/Export.h"
#include "./Executes/TestBrush.h"

Program::Program()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new ExecuteValues();
	values->ViewProjection = new ViewProjectionBuffer();
	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);
	values->GlovalLight = new LightBuffer();



	values->MainCamera = new Freedom();
	values->MainCamera->RotationDegree(44, 87);
	values->MainCamera->Position(-47, 103, 74);



	executes.push_back(new Export(values));
	executes.push_back(new TestBrush(values));
}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	SAFE_DELETE(values->GlovalLight);
	SAFE_DELETE(values->ViewProjection);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->Viewport);
	SAFE_DELETE(values);
}

void Program::Update()
{
	values->MainCamera->Update();

	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	D3DXMATRIX V, P;
	values->MainCamera->Matrix(&V);
	values->Perspective->GetMatrix(&P);

	values->ViewProjection->SetView(V);
	values->ViewProjection->SetProjection(P);
	values->ViewProjection->SetVSBuffer(0);

	// Àü¿ª ºû
	values->GlovalLight->SetPSBuffer(0);
	



	D3DXVECTOR3 position;
	values->MainCamera->Position(&position);
	ImGui::LabelText("Cam Position", "%0.0f, %0.0f, %0.0f", position.x, position.y, position.z);

	D3DXVECTOR2 rotation;
	values->MainCamera->RotationDegree(&rotation);
	ImGui::LabelText("Cam Rotation", "%0.0f, %0.0f", rotation.x, rotation.y);


	ImGui::SliderFloat3("Light Direction", (float*)values->GlovalLight->Data.Direction, -1, 1);
	ImGui::Separator();


	for (Execute* exe : executes)
		exe->Render();
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