#include "stdafx.h"
#include "DrawCube.h"

DrawCube::DrawCube(ExecuteValues*values)
	:Execute(values), VertexCount(24), indiceCount(36)
{
	

	shader = new Shader(Shaders + L"009_Grid.hlsl");
	worldBuffer = new WorldBuffer();

	// 점 6 * 4 필요
	float Pos[24][3] = 
	{
		{-0.5f,-0.5f,-0.5f}, {-0.5f,0.5f,-0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f}, // 정면
		{-0.5f,-0.5f,0.5f}, {-0.5f,0.5f,0.5f}, {0.5f,-0.5f,0.5f}, {0.5f,0.5f,0.5f},// 뒤
		{-0.5f,-0.5f,0.5f}, {-0.5f,0.5f,0.5f}, {-0.5f,-0.5f,-0.5f}, {-0.5f,0.5f,-0.5f}, // 좌
		{0.5f,-0.5f,0.5f}, {0.5f, 0.5f,0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f}, // 우
		{-0.5f,0.5f,-0.5f}, {-0.5f,0.5f,0.5f}, {0.5f,0.5f,-0.5f}, {0.5f,0.5f,0.5f}, //상
		{-0.5f,-0.5f,-0.5f,},{-0.5f,-0.5f,0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,0.5f}//하

	};



	vertices = new VertexTexture[VertexCount];
	for (int i = 0; i < 6; i++)
	{
		vertices[i * 4 + 0].Position = D3DXVECTOR3(Pos[i * 4 + 0][0], Pos[i * 4 + 0][1], Pos[i * 4 + 0][2]);
		vertices[i * 4 + 1].Position = D3DXVECTOR3(Pos[i * 4 + 1][0], Pos[i * 4 + 1][1], Pos[i * 4 + 1][2]);
		vertices[i * 4 + 2].Position = D3DXVECTOR3(Pos[i * 4 + 2][0], Pos[i * 4 + 2][1], Pos[i * 4 + 2][2]);
		vertices[i * 4 + 3].Position = D3DXVECTOR3(Pos[i * 4 + 3][0], Pos[i * 4 + 3][1], Pos[i * 4 + 3][2]);

	}

	// UV좌표계는 3차원을 다 그리고 각 버텍스 왼쪽위가 ,0 기준인가?
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
		// 반시계
		if (i % 2 == 0) {
			indices[i * 6 + 0] = i * 4 + 0;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 1;
			indices[i * 6 + 5] = i * 4 + 3;
		}
		// 시계방향으로 그려야함 // 전부 시계방향으로 그렷다가 뒤 오른쪽 아래 법선벡터가
		// 반대라서 안보임, 그래서 방향을 반대로 그려서 벡터방향을 바꿔야함.
		// 이부분은 전에 했던 사람것 코드를 가져왔습니다. 제가한건 밑에 주석처리된 부분
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
		D3D11_BUFFER_DESC desc = { 0 }; //DESC 디스크립터 무조건 붙는 규칙
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * VertexCount; //용량
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //만드는용도

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices; //pSysMem의p는 포인터

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC 디스크립터 무조건 붙는 규칙
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(indices) * indiceCount; //용량
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //만드는용도

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices; //pSysMem의p는 포인터

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


	SAFE_RELEASE(vertexBuffer);//I가붙는애들은 릴리즈로지워야됨
	SAFE_RELEASE(indexBuffer);//I가붙는애들은 릴리즈로지워야됨



	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}


void DrawCube::Update()
{
	
	//// 회전

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

	
	//이동

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


	//세팅부분
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();
	colorBuffer->SetPSBuffer(0);





	//실제그리는부분
	//D3D::GetDC()->Draw(6, 0);

	D3D::GetDC()->DrawIndexed(36, 0, 0);


}
