#include "stdafx.h"
#include "DrawCube.h"

DrawCube::DrawCube(ExecuteValues * values)
	: Execute(values)
	, CubePositionDirection(0, 0, 0)
	, Min (-0.5f, -0.5f, -0.5f), Max(0.5f, 0.5f, 0.5f)
	, scale(1,1,1), rotation(0,0,0), position(0,0,0)
	, check(false)
{
	
	//큐브 만들기
	worldBuffer = new WorldBuffer();
	CubeShader = new Shader(Shaders + L"010_Triangle.hlsl");
	CubePosition = D3DXVECTOR3(0, 0, 0);

	// 점 6 * 4 필요
	float Pos[24][3] =
	{
		{ Min.x, Min.y, Min.z },{ Min.x, Max.y, Min.z },{ Max.x,Min.y,Min.z },{ Max.x,Max.y,Min.z }, // 정면
		{ Min.x, Min.y, Max.z },{ Min.x, Max.y, Max.z },{ Max.x, Min.y,Max.z },{ Max.x,Max.y,Max.z },// 뒤
		{ Min.x ,Min.y, Max.z },{ Min.x, Max.y, Max.z },{ Min.x, Min.y,Min.z },{ Min.x,Max.y,Min.z }, // 좌
		{ Max.x ,Min.y, Max.z },{ Max.x, Max.y, Max.z },{ Max.x, Min.y,Min.z },{ Max.x,Max.y,Min.z }, // 우
		{ Min.x, Max.y, Min.z },{ Min.x, Max.y, Max.z },{ Max.x,Max.y,Min.z },{ Max.x,Max.y,Max.z }, //상
		{ Min.x, Min.y, Min.z },{ Min.x, Min.y, Max.z },{ Max.x, Min.y,Min.z },{ Max.x,Min.y,Max.z }//하

	};

	CubeVertices = new VertexTextureColorNormal[24];
	for (int i = 0; i < 6; i++)
	{
		CubeVertices[i * 4 + 0].Position = D3DXVECTOR3(Pos[i * 4 + 0][0], Pos[i * 4 + 0][1], Pos[i * 4 + 0][2]);
		CubeVertices[i * 4 + 1].Position = D3DXVECTOR3(Pos[i * 4 + 1][0], Pos[i * 4 + 1][1], Pos[i * 4 + 1][2]);
		CubeVertices[i * 4 + 2].Position = D3DXVECTOR3(Pos[i * 4 + 2][0], Pos[i * 4 + 2][1], Pos[i * 4 + 2][2]);
		CubeVertices[i * 4 + 3].Position = D3DXVECTOR3(Pos[i * 4 + 3][0], Pos[i * 4 + 3][1], Pos[i * 4 + 3][2]);


		CubeVertices[i * 4 + 0].Uv = D3DXVECTOR2(0 + i, 1 + i);
		CubeVertices[i * 4 + 1].Uv = D3DXVECTOR2(0 + i, 0 + i);
		CubeVertices[i * 4 + 2].Uv = D3DXVECTOR2(1 + i, 1 + i);
		CubeVertices[i * 4 + 3].Uv = D3DXVECTOR2(1 + i, 0 + i);
		// 여긴 uv좌표 
	}

	CubeIndices = new UINT[36];
	for (int i = 0; i < 6; i++)
	{
		// 반시계
		if (i % 2 == 0) {
			CubeIndices[i * 6 + 0] = i * 4 + 0;
			CubeIndices[i * 6 + 1] = i * 4 + 1;
			CubeIndices[i * 6 + 2] = i * 4 + 2;
			CubeIndices[i * 6 + 3] = i * 4 + 2;
			CubeIndices[i * 6 + 4] = i * 4 + 1;
			CubeIndices[i * 6 + 5] = i * 4 + 3;
		}
		// 시계방향으로 그려야함 // 전부 시계방향으로 그렷다가 뒤 오른쪽 아래 법선벡터가
		// 반대라서 안보임, 그래서 방향을 반대로 그려서 벡터방향을 바꿔야함.
		// 이부분은 전에 했던 사람것 코드를 가져왔습니다. 제가한건 밑에 주석처리된 부분
		else {
			CubeIndices[i * 6 + 0] = i * 4 + 1;
			CubeIndices[i * 6 + 1] = i * 4 + 0;
			CubeIndices[i * 6 + 2] = i * 4 + 2;
			CubeIndices[i * 6 + 3] = i * 4 + 1;
			CubeIndices[i * 6 + 4] = i * 4 + 2;
			CubeIndices[i * 6 + 5] = i * 4 + 3;
		}
	}



	//Create CubeVertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC 디스크립터 무조건 붙는 규칙
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureColorNormal) * 24; //용량
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //만드는용도

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = CubeVertices; //pSysMem의p는 포인터

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &CubeVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create CubeIndex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC 디스크립터 무조건 붙는 규칙
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(CubeIndices) * 36; //용량
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //만드는용도

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = CubeIndices; //pSysMem의p는 포인터

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &CubeIndexBuffer);
		assert(SUCCEEDED(hr));
	}


}

DrawCube::~DrawCube()
{


}

void DrawCube::Update()
{

	if (Mouse::Get()->Down(0) && GetPickingMouse())
	{
		check = true;
	}

	if (check)
	{
		D3DXMATRIX S, R, T;
		D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
		D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);


		worldBuffer->SetMatrix(S * R * T);
	}


}

void DrawCube::Render()
{

	ImGui::Checkbox("check", &check);
	ImGui::SliderFloat3("scale", (float *)&scale, -10, 10);
	ImGui::SliderFloat3("rotation", (float *)&rotation, -1.5f, 1.5f);
	ImGui::SliderFloat3("position", (float *)&position, -10, 10);

	// 육면체 그리기

	UINT stride = sizeof(VertexTextureColorNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &CubeVertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(CubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	worldBuffer->SetVSBuffer(1);
	CubeShader->Render();


	D3D::GetDC()->DrawIndexed(36, 0, 0);

}

bool DrawCube::GetPickingMouse()
{

	Viewport* vp = values->Viewport;

	D3DXVECTOR3 start;
	values->MainCamera->Position(&start);

	D3DXMATRIX V, P;
	values->MainCamera->Matrix(&V);
	values->Perspective->GetMatrix(&P);

	D3DXVECTOR3 direction = vp->GetDirection(V, P);



	for (UINT i = 0; i < 6; i++)
	{
		UINT index[4];
		// 정면
		index[0] = 0 + i * 4;
		index[1] = 1 + i * 4;
		index[2] = 2 + i * 4;
		index[3] = 3 + i * 4;


		D3DXVECTOR3 p[4];
		for (int i = 0; i < 4; i++)
			p[i] = CubeVertices[index[i]].Position;


		D3DXVECTOR3 out;
		float u, v, distance;
		if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		{
			out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

			return true;
		}

		if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		{
			out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

			return true;
		}



	}

	return false;

}
