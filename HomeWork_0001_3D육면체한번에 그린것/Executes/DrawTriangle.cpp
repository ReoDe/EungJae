#include "stdafx.h"
#include "DrawTriangle.h"

DrawTriangle::DrawTriangle(ExecuteValues*values)
	:Execute(values)
{
	shader = new Shader(Shaders + L"002_Vertex.hlsl");
	worldBuffer = new WorldBuffer();

	vertices = new Vertex[3];
	vertices[0].Position = D3DXVECTOR3(0, 0, 0);
	vertices[1].Position = D3DXVECTOR3(0, 1, 0);
	vertices[2].Position = D3DXVECTOR3(1, 0, 0);
	
	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 3; //�뷮
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
	SAFE_DELETE_ARRAY(vertices);
}


DrawTriangle::~DrawTriangle()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);

	SAFE_RELEASE(vertexBuffer);//I���ٴ¾ֵ��� ������������ߵ�

	SAFE_DELETE_ARRAY(vertices);
}

void DrawTriangle::Render()
{
	//���úκ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();

	//�����׸��ºκ�
	D3D::GetDC()->Draw(3, 0);


}
