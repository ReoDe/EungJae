#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"
#include "./Viewer/Viewport.h"
#include "./Viewer/Perspective.h"

//#include "./Executes/TestEffect.h"
//#include "./Executes/TestLine.h"
#include "./Executes/TestMesh.h"
#include "./Executes/TestModel.h"
#include "./Executes/Test.h"

Program::Program()
{
	Context::Create();


	//executes.push_back(new TestEffect());
	//executes.push_back(new TestLine());
	executes.push_back(new Test());
	//executes.push_back(new TestModel());
	Context::GetMainCamera()->RotationDegree(35, 2);
	Context::GetMainCamera()->Position(48, 68, -61);
}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	Context::Delete();
}

void Program::Update()
{
	Context::GetMainCamera()->Update();

	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	D3DXVECTOR3 position;
	Context::GetMainCamera()->Position(&position);
	ImGui::LabelText("Cam Position", "%0.0f, %0.0f, %0.0f", position.x, position.y, position.z);

	D3DXVECTOR2 rotation;
	Context::GetMainCamera()->RotationDegree(&rotation);
	ImGui::LabelText("Cam Rotation", "%0.0f, %0.0f", rotation.x, rotation.y);

	ImGui::SliderFloat3("Light Direction", (float *)Context::GetGlobalLight()->Direction, -1, 1);

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

	Context::GetPerspective()->Set(desc.Width, desc.Height);
	Context::GetViewport()->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();
}