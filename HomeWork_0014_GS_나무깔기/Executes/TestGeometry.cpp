#include "stdafx.h"
#include "TestGeometry.h"
#include "Environment\Terrain.h"

TestGeometry::TestGeometry() :position(0, 0, 0), WindPower(0.3f), WindDirection(1)
{
	terrainMaterial = new Material(Shaders + L"028_Terrain.fx");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");
	terrain = new Terrain(terrainMaterial, Textures + L"HeightMap256.png");

	//

	shader = new Shader(Shaders + L"028_HomeWork.fx");
	texture = new Texture(Textures + L"Tree.png");
	shader->AsShaderResource("Map")->SetResource(texture->SRV());

	

}

TestGeometry::~TestGeometry()
{
	SAFE_DELETE(terrain);
	SAFE_DELETE(terrainMaterial);

	SAFE_DELETE(shader);

}

void TestGeometry::Update()
{
	terrain->Update();

	//


	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	shader->AsMatrix("World")->SetMatrix(world);

	//

	D3DXVECTOR3 position;
	if (terrain->GetHeight(position) == true)
		this->position = position;


	if (Mouse::Get()->Down(0))
	{
		CreateVertex();
	}


	for (Vertexes* vs : Vertex)
		vs->VertexUpdate();

	
}

void TestGeometry::PreRender()
{

}

void TestGeometry::Render()
{
	terrain->Render();

	//
	for (Vertexes* vs : Vertex)
		vs->VertexRender();

	int vectersize = Vertex.size();
	ImGui::SliderInt("Vector Size", &vectersize, 0, 1000);
	ImGui::SliderInt("WindPower", &WindPower, 0, 3);
	ImGui::SliderInt("WindDirection", &WindDirection, -1, 1);

	


}

void TestGeometry::CreateVertex()
{
	int rand = 0;
	rand = Math::Random(0, 10);
	float frand = (float)rand / 10;
	Vertexes* vertex = new Vertexes(terrain, shader, texture, position, frand);

	vertex->SetWindPower(WindPower, WindDirection);


	Vertex.push_back(vertex);
}

TestGeometry::Vertexes::Vertexes(class Terrain* terrain, Shader* shader, Texture* texture, D3DXVECTOR3 position, float ClickTime)
	: terrain(terrain), shader(shader), texture(texture), ClickTime(ClickTime)
{

	shader->AsShaderResource("Map")->SetResource(texture->SRV());

	// 정점만들기

	VertexSizeClick vertex;

	D3DXVECTOR2 scale;
	scale.x = Math::Random(5.0f, 10.0f);
	scale.y = Math::Random(5.0f, 10.0f);

	//position.x = Math::Random(0.0f, 256.0f);
	//position.z = Math::Random(0.0f, 256.0f);


	float y = terrain->GetHeight(position.x, position.z);

	position.y = y + scale.y * 0.5f;

	vertex.Position = position;
	vertex.Size = scale;
	vertex.ClickTime = ClickTime;
	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexSizeClick);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &vertex;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}


}


void TestGeometry::Vertexes::VertexUpdate()
{
	D3DXMATRIX Mat;
	D3DXMatrixTranslation(&Mat, sinf(Time::Get()->Running() * 5 * ClickTime) - 1, 0, 0);

	shader->AsMatrix("W")->SetMatrix(Mat);

	shader->AsScalar("WindPower")->SetFloat(WindPower);
	shader->AsScalar("WindDirection")->SetInt(WindDirection);


}

void TestGeometry::Vertexes::VertexRender()
{
	UINT stride = sizeof(VertexSizeClick);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	shader->Draw(0, 0, 1);
}
