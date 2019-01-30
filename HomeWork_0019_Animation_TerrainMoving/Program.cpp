#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"
#include "./Viewer/Viewport.h"
#include "./Viewer/Perspective.h"

#include "./Executes/TestAnimation.h"

Program::Program()
{
	Context::Create();
	Context::Get()->GetMainCamera()->RotationDegree(0, 0);
	Context::Get()->GetMainCamera()->Position(0, 10, -10);

	executes.push_back(new TestAnimation());
}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	Context::Delete();
}

void Program::Update()
{
	Context::Get()->Update();

	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{
	for (Execute* exe : executes)
		exe->PreRender();
}

void Program::Render()
{
	Context::Get()->GetViewport()->RSSetViewport();

	ImGui::SliderFloat3("Light Direction", (float *)Context::Get()->GetGlobalLight()->Direction, -1, 1);
	Context::Get()->ChangeGlobalLight();

	for (Execute* exe : executes)
		exe->Render();
}

void Program::PostRender()
{
	wstring str;
	RECT rect = { 0, 0, 300, 15 };

	str = String::Format(L"Frame Per Second : %0.0f", ImGui::GetIO().Framerate);
	DirectWrite::RenderText(str, rect, 12);

	
	D3DXVECTOR3 position;
	Context::Get()->GetMainCamera()->Position(&position);
	str = String::Format(L"Cam Position : %0.0f, %0.0f, %0.0f", position.x, position.y, position.z);

	rect.top += 15;
	rect.bottom += 15;
	DirectWrite::RenderText(str, rect, 12);



	D3DXVECTOR2 rotation;
	Context::Get()->GetMainCamera()->RotationDegree(&rotation);
	str = String::Format(L"Cam Rotation : %0.0f, %0.0f", rotation.x, rotation.y);

	rect.top += 15;
	rect.bottom += 15;
	DirectWrite::RenderText(str, rect, 12);

	for (Execute* exe : executes)
		exe->PostRender();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	Context::Get()->GetPerspective()->Set(desc.Width, desc.Height);
	Context::Get()->GetViewport()->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();
}