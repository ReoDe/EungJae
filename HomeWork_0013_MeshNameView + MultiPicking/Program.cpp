#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"
#include "./Viewer/Viewport.h"
#include "./Viewer/Perspective.h"

#include "./Executes/TestModel.h"
#include "./Executes/Test.h"
#include "./Executes/Test2.h"

Program::Program()
{
	Context::Create();
	Context::Get()->GetMainCamera()->RotationDegree(23, 26);
	Context::Get()->GetMainCamera()->Position(-3,7, -11);
	// -3, 7, -11,   23  26
	//executes.push_back(new TestMesh());
	executes.push_back(new Test());
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

}

void Program::Render()
{
	ImGui::SliderFloat3("Light Direction", (float *)Context::Get()->GetGlobalLight()->Direction, -1, 1);
	Context::Get()->ChangeGlobalLight();

	for (Execute* exe : executes)
		exe->Render();
}

void Program::PostRender()
{
	wstring str;
	RECT rect = { 0,0,300,20 };

	str = String::Format(L"Frame Per Second : %0.0f", ImGui::GetIO().Framerate);
	DirectWrite::RenderText(str, rect, 12);

	D3DXVECTOR3 position;
	Context::Get()->GetMainCamera()->Position(&position);
	str = String::Format(L"Cam Position : %0.0f, %0.0f, %0.0f", position.x, position.y, position.z);

	rect.top += 20;
	rect.bottom += 20;
	DirectWrite::RenderText(str, rect, 12);

	D3DXVECTOR2 rotation;
	Context::Get()->GetMainCamera()->RotationDegree(&rotation);
	str = String::Format(L"Cam Rotation : %0.0f, %0.0f", rotation.x, rotation.y);

	rect.top += 20;
	rect.bottom += 20;
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