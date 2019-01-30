#include "stdafx.h"
#include "Sky.h"

#include "./Viewer/Camera.h"

Sky::Sky()
	:apex(0xFF0080FF), center(0xFF9BCDFF), height(4.5f)
{
	material = new Material(Shaders + L"018_Sky.fx");
	sphere = new MeshSphere(material, 0.5f, 20, 20);

	material->GetShader()->AsVector("Apex")->SetFloatVector(apex);
	material->GetShader()->AsVector("Center")->SetFloatVector(center);
	material->GetShader()->AsScalar("Height")->SetFloat(height);

}

Sky::~Sky()
{
	
}

void Sky::Update()
{
	D3DXVECTOR3 position;
	Context::Get()->GetMainCamera()->Position(&position);

	sphere->Position(position);
}

void Sky::Render()
{
	//ImGui::ColorEdit3("Apex", (float*)&buffer->Data.Apex);
	//ImGui::ColorEdit3("Center", (float*)&buffer->Data.Center);
	//ImGui::SliderFloat("Height", &buffer->Data.height, 0 , 100);

	//buffer->SetPSBuffer(10);

	sphere->Render();
	

}
