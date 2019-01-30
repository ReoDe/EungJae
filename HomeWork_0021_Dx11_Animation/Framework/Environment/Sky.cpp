#include "framework.h"
#include "Sky.h"
#include "../Viewer/Camera.h"

Sky::Sky()
	: apex(0xFF9BCDFF), center(0xFF0080FF), height(4.5f)
{
	material = new Material(Shaders + L"018_Sky.fx");
	sphere = new MeshSphere(material, 0.5f, 20, 20);

	material->GetShader()->AsVector("Apex")->SetFloatVector(apex);
	material->GetShader()->AsVector("Center")->SetFloatVector(center);
	material->GetShader()->AsScalar("Height")->SetFloat(height);
}

Sky::~Sky()
{
	SAFE_DELETE(sphere);
	SAFE_DELETE(material);
}

void Sky::Update()
{
	D3DXVECTOR3 position;
	Context::Get()->GetMainCamera()->Position(&position);

	sphere->Position(position);
}

void Sky::Render()
{
	sphere->Render();
}
