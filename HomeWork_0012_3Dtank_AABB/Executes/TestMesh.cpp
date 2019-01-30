#include "stdafx.h"
#include "TestMesh.h"

TestMesh::TestMesh()
{
	//material = new Material(Shaders + L"022_SamplerFilter.fx");
	//material->SetDiffuseMap(Textures + L"Bricks.png");

	//quad = new MeshQuad(material, 1);
	//quad->Scale(100, 100, 100);
	
	shader = new Shader(Shaders + L"022_SamplerAddress.fx");


	VertexTexture vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 2);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(2, 2);
	vertices[3].Uv = D3DXVECTOR2(2, 2);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(2, 0);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	texture = new Texture(Textures + L"Box.png");

}

TestMesh::~TestMesh()
{
	/*SAFE_DELETE(quad);
	SAFE_DELETE(material);*/

	SAFE_DELETE(shader);
	//SAFE_DELETE(vertexBuffer);

}

void TestMesh::Update()
{
		
}

void TestMesh::Render()
{
	/*static int i = 0;
	ImGui::SliderInt("Filter", &i, 0, 1);

	quad->Pass(i);

	quad->Render();*/

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Context::SetValues(shader);

	D3DXMATRIX W;
	D3DXMatrixScaling(&W, 10, 10, 1);


	shader->AsMatrix("World")->SetMatrix(W);
	static int test = 0;

	ImGui::SliderInt("Select", &test, 0, 4);
	shader->AsScalar("Selected")->SetInt(test);

	shader->AsShaderResource("DiffuseMap")->SetResource(texture->SRV());
	// 여기 공부좀

	shader->Draw(0, 0, 6);


}

