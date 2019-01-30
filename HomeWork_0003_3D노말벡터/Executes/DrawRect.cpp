#include "stdafx.h"
#include "DrawRect.h"

DrawRect::DrawRect(ExecuteValues*values)
	:Execute(values)
{
	//shader = new Shader(Shaders + L"009_Rect2D.hlsl");
	shader = new Shader(Shaders + L"009_Rect.hlsl");
	worldBuffer = new WorldBuffer();

	

	vertices = new VertexColor[4];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	vertices[0].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[1].Color = D3DXCOLOR(0, 1, 0, 1);
	vertices[2].Color = D3DXCOLOR(0, 0, 1, 1);
	vertices[3].Color = D3DXCOLOR(1, 1, 1, 1);

	indices = new UINT[6]{ 0,1,2, 2,1,3 };



	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexColor) * 4; //�뷮
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(indices) * 6; //�뷮
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	colorBuffer = new ColorBuffer();

	// ???
	rState[0] = new RasterizerState();

	rState[1] = new RasterizerState();
	rState[1]->FillMode(D3D11_FILL_WIREFRAME);


}


DrawRect::~DrawRect()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(colorBuffer);


	SAFE_RELEASE(vertexBuffer);//I���ٴ¾ֵ��� ������������ߵ�
	SAFE_RELEASE(indexBuffer);//I���ٴ¾ֵ��� ������������ߵ�

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void DrawRect::Render()
{
	
	//���úκ�
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//1���簢��
	{
		D3DXMATRIX W;
		D3DXMatrixTranslation(&W, 2, 0, 0);

		// ������۸� �̿��ϸ� �������� �ϳ��� �̿��ؼ� ���������� �����ϵ���
		// �ٲ� �� �ִ�.
		worldBuffer->SetMatrix(W);
		worldBuffer->SetVSBuffer(1);


		colorBuffer->Data.Color = D3DXCOLOR(1, 0, 0, 1);
		colorBuffer->SetPSBuffer(0);




		shader->Render();
		
		rState[1]->RSSetState();
		{
			D3D::GetDC()->DrawIndexed(6, 0, 0);
		}
		rState[0]->RSSetState();
	}

	//2���簢��
	{
		D3DXMATRIX W;
		D3DXMatrixTranslation(&W, -2, 0, 0);

		worldBuffer->SetMatrix(W);
		worldBuffer->SetVSBuffer(1);


		colorBuffer->Data.Color = D3DXCOLOR(0, 0, 1, 1);
		colorBuffer->SetPSBuffer(0);




		shader->Render();

		D3D::GetDC()->DrawIndexed(6, 0, 0);
	}







}
