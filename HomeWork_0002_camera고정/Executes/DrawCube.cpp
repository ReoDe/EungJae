#include "stdafx.h"
#include "DrawCube.h"

DrawCube::DrawCube(ExecuteValues*values)
	:Execute(values), VertexCount(24), indiceCount(36)
{
	

	shader = new Shader(Shaders + L"009_Grid.hlsl");
	worldBuffer = new WorldBuffer();

	// �� 6 * 4 �ʿ�
	float Pos[24][3] = 
	{
		{-0.5f,-0.5f,-0.5f}, {-0.5f,0.5f,-0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f}, // ����
		{-0.5f,-0.5f,0.5f}, {-0.5f,0.5f,0.5f}, {0.5f,-0.5f,0.5f}, {0.5f,0.5f,0.5f},// ��
		{-0.5f,-0.5f,0.5f}, {-0.5f,0.5f,0.5f}, {-0.5f,-0.5f,-0.5f}, {-0.5f,0.5f,-0.5f}, // ��
		{0.5f,-0.5f,0.5f}, {0.5f, 0.5f,0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f}, // ��
		{-0.5f,0.5f,-0.5f}, {-0.5f,0.5f,0.5f}, {0.5f,0.5f,-0.5f}, {0.5f,0.5f,0.5f}, //��
		{-0.5f,-0.5f,-0.5f,},{-0.5f,-0.5f,0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,0.5f}//��

	};



	vertices = new VertexTexture[VertexCount];
	for (int i = 0; i < 6; i++)
	{
		vertices[i * 4 + 0].Position = D3DXVECTOR3(Pos[i * 4 + 0][0], Pos[i * 4 + 0][1], Pos[i * 4 + 0][2]);
		vertices[i * 4 + 1].Position = D3DXVECTOR3(Pos[i * 4 + 1][0], Pos[i * 4 + 1][1], Pos[i * 4 + 1][2]);
		vertices[i * 4 + 2].Position = D3DXVECTOR3(Pos[i * 4 + 2][0], Pos[i * 4 + 2][1], Pos[i * 4 + 2][2]);
		vertices[i * 4 + 3].Position = D3DXVECTOR3(Pos[i * 4 + 3][0], Pos[i * 4 + 3][1], Pos[i * 4 + 3][2]);

	}

	// UV��ǥ��� 3������ �� �׸��� �� ���ؽ� �������� ,0 �����ΰ�?
	for (int i = 0; i < 6; i++)
	{
		vertices[i * 4 + 0].Uv = D3DXVECTOR2(0 + i, 1 + i);
		vertices[i * 4 + 1].Uv = D3DXVECTOR2(0 + i, 0 + i);
		vertices[i * 4 + 2].Uv = D3DXVECTOR2(1 + i, 1 + i);
		vertices[i * 4 + 3].Uv = D3DXVECTOR2(1 + i, 0 + i);
	}



	indices = new UINT[indiceCount];
	for (int i = 0; i < 6; i++) 
	{
		// �ݽð�
		if (i % 2 == 0) {
			indices[i * 6 + 0] = i * 4 + 0;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 1;
			indices[i * 6 + 5] = i * 4 + 3;
		}
		// �ð�������� �׷����� // ���� �ð�������� �׷Ǵٰ� �� ������ �Ʒ� �������Ͱ�
		// �ݴ�� �Ⱥ���, �׷��� ������ �ݴ�� �׷��� ���͹����� �ٲ����.
		// �̺κ��� ���� �ߴ� ����� �ڵ带 �����Խ��ϴ�. �����Ѱ� �ؿ� �ּ�ó���� �κ�
		else {
			indices[i * 6 + 0] = i * 4 + 1;
			indices[i * 6 + 1] = i * 4 + 0;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 1;
			indices[i * 6 + 4] = i * 4 + 2;
			indices[i * 6 + 5] = i * 4 + 3;
		}
	}





	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * VertexCount; //�뷮
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
		desc.ByteWidth = sizeof(indices) * indiceCount; //�뷮
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	colorBuffer = new ColorBuffer();
	
	


}


DrawCube::~DrawCube()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(colorBuffer);


	SAFE_RELEASE(vertexBuffer);//I���ٴ¾ֵ��� ������������ߵ�
	SAFE_RELEASE(indexBuffer);//I���ٴ¾ֵ��� ������������ߵ�



	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}


void DrawCube::Update()
{
	
	//// ȸ��

	//static D3DXVECTOR3 rotaion(0, 0, 0);

	//if (Keyboard::Get()->Press('A'))
	//{
	//	rotaion.y -= 30.0f * Time::Delta();
	//}
	//if (Keyboard::Get()->Press('D'))
	//{
	//	rotaion.y += 30.0f * Time::Delta();
	//}

	//if (Keyboard::Get()->Press('Q'))
	//{
	//	rotaion.x -= 30.0f * Time::Delta();
	//}
	//if (Keyboard::Get()->Press('E'))
	//{
	//	rotaion.x += 30.0f * Time::Delta();
	//}

	//if (Keyboard::Get()->Press('Z'))
	//{
	//	rotaion.z -= 30.0f * Time::Delta();
	//}
	//if (Keyboard::Get()->Press('C'))
	//{
	//	rotaion.z += 30.0f * Time::Delta();
	//}



	//D3DXMATRIX Ry;
	//D3DXMatrixRotationY(&Ry, Math::ToRadian(rotaion.y));

	//D3DXMATRIX Rx;
	//D3DXMatrixRotationX(&Rx, Math::ToRadian(rotaion.x));

	//D3DXMATRIX Rz;
	//D3DXMatrixRotationZ(&Rz, Math::ToRadian(rotaion.z));

	
	//�̵�

	static D3DXVECTOR3 tranceform(0, 0, 0);

	if (Keyboard::Get()->Press('W'))
	{
		tranceform.z += 1.0f * Time::Delta();
	}
	if (Keyboard::Get()->Press('S'))
	{
		tranceform.z -= 1.0f * Time::Delta();
	}

	if (Keyboard::Get()->Press('A'))
	{
		tranceform.x -= 1.0f * Time::Delta();
	}
	if (Keyboard::Get()->Press('D'))
	{
		tranceform.x += 1.0f * Time::Delta();
	}

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, tranceform.x, tranceform.y, tranceform.z);



	worldBuffer->SetMatrix(T);

	xyz = vertices[0].Position;

}

void DrawCube::Render()
{
	ImGui::SliderFloat("x axis", &xyz.x, 0, 5);


	//���úκ�
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();
	colorBuffer->SetPSBuffer(0);





	//�����׸��ºκ�
	//D3D::GetDC()->Draw(6, 0);

	D3D::GetDC()->DrawIndexed(36, 0, 0);


}
