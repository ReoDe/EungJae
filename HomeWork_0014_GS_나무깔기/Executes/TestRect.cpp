#include "stdafx.h"
#include "TestRect.h"
#include "Environment\Terrain.h"

TestRect::TestRect()
{
	shader = new Shader(Shaders + L"028_Rect.fx");
	texture = new Texture(Textures + L"Tree.png");
	shader->AsShaderResource("Map")->SetResource(texture->SRV());

	VertexSize vertex;
	vertex.Position = D3DXVECTOR3(0, 0, 0);
	vertex.Size = D3DXVECTOR2(1,1);

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexSize);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &vertex;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

}

TestRect::~TestRect()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);

}

void TestRect::Update()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	shader->AsMatrix("World")->SetMatrix(world);
}

void TestRect::PreRender()
{

}

void TestRect::Render()
{
	UINT stride = sizeof(VertexSize);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	shader->Draw(0, 0, 1);
}


