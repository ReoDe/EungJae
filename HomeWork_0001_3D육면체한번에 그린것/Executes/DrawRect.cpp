#include "stdafx.h"
#include "DrawRect.h"

DrawRect::DrawRect(ExecuteValues*values)
	:Execute(values), VertexCount(24), indiceCount(36)
{
	
	srvs[2] = NULL; // 초기화
	srvs[3] = NULL; // 초기화
	srvs[4] = NULL; // 초기화
	srvs[5] = NULL; // 초기화
	srvs[6] = NULL; // 초기화
	srvs[7] = NULL; // 초기화
	srvs[8] = NULL; // 초기화
	srvs[9] = NULL; // 초기화
	srvs[10] = NULL; // 초기화
	srvs[11] = NULL; // 초기화

	shader = new Shader(Shaders + L"0001_Vertex.hlsl");
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

	////선 이어주기 0,1,2 2,1,3
	//indices = new UINT[indiceCount];
	//for (int i = 0; i < 6; i++)
	//{
	//	indices[i * 6 + 0] = i * 4 + 0;
	//	indices[i * 6 + 1] = i * 4 + 1;
	//	indices[i * 6 + 2] = i * 4 + 2;
	//	indices[i * 6 + 3] = i * 4 + 2;
	//	indices[i * 6 + 4] = i * 4 + 1;
	//	indices[i * 6 + 5] = i * 4 + 3;
	//};




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
	
	//Create SRV
	{

		wstring Texture = Textures + L"Box.png";
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			D3D::GetDevice(),
			Texture.c_str(),
			NULL,
			NULL,
			&srv,
			NULL
		);

	assert(SUCCEEDED(hr));

	}

	//Create SRV2
	{

		wstring Texture = Textures + L"Bricks.png";
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			D3D::GetDevice(),
			Texture.c_str(),
			NULL,
			NULL,
			&srv2,
			NULL
		);

		assert(SUCCEEDED(hr));

	}

	//Create SRVS[2]
	{

		wstring Texture = Textures + L"Bricks.png";
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			D3D::GetDevice(),
			Texture.c_str(),
			NULL,
			NULL,
			&srvs[1],
			NULL
		);

		assert(SUCCEEDED(hr));

	}

	//Create SRVS[2]
	{

		wstring Texture = Textures + L"Wall.png";
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			D3D::GetDevice(),
			Texture.c_str(),
			NULL,
			NULL,
			&srvs[0],
			NULL
		);

		assert(SUCCEEDED(hr));

	}



}


DrawRect::~DrawRect()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(colorBuffer);


	SAFE_RELEASE(vertexBuffer);//I가붙는애들은 릴리즈로지워야됨
	SAFE_RELEASE(indexBuffer);//I가붙는애들은 릴리즈로지워야됨
	SAFE_RELEASE(srv);//I가붙는애들은 릴리즈로지워야됨
	SAFE_RELEASE(srv2);//I가붙는애들은 릴리즈로지워야됨


	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}


void DrawRect::Update()
{
	static D3DXVECTOR3 rotaion(0, 0, 0);

	if (Keyboard::Get()->Press('A'))
	{
		rotaion.y -= 30.0f * Time::Delta();
	}
	if (Keyboard::Get()->Press('D'))
	{
		rotaion.y += 30.0f * Time::Delta();
	}

	if (Keyboard::Get()->Press('Q'))
	{
		rotaion.x -= 30.0f * Time::Delta();
	}
	if (Keyboard::Get()->Press('E'))
	{
		rotaion.x += 30.0f * Time::Delta();
	}

	if (Keyboard::Get()->Press('Z'))
	{
		rotaion.z -= 30.0f * Time::Delta();
	}
	if (Keyboard::Get()->Press('C'))
	{
		rotaion.z += 30.0f * Time::Delta();
	}



	D3DXMATRIX Ry;
	D3DXMatrixRotationY(&Ry, Math::ToRadian(rotaion.y));

	D3DXMATRIX Rx;
	D3DXMatrixRotationX(&Rx, Math::ToRadian(rotaion.x));

	D3DXMATRIX Rz;
	D3DXMatrixRotationZ(&Rz, Math::ToRadian(rotaion.z));

	worldBuffer->SetMatrix(Rx * Ry * Rz);


}

void DrawRect::Render()
{
	function<void(wstring)> func;
	func = bind(&DrawRect::Front_Left, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Front_Left"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func);
	}


	function<void(wstring)> func1;
	func1 = bind(&DrawRect::Front_Right, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Front_Right"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func1);
	}

	ImGui::SliderFloat("X1", &colorBuffer->Data.X1, 0, 1);
	
	// 정명


	function<void(wstring)> func2;
	func2 = bind(&DrawRect::Back_Left, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Back_Left"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func2);
	}


	function<void(wstring)> func3;
	func3 = bind(&DrawRect::Back_Right, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Back_Right"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func3);
	}

	ImGui::SliderFloat("X2", &colorBuffer->Data.X2, 1, 2);

	// 뒤변



	function<void(wstring)> func4;
	func4 = bind(&DrawRect::Left_Left, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Left_Left"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func4);
	}

	function<void(wstring)> func5;
	func5 = bind(&DrawRect::Left_Right, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Left_Right"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func5);
	}

	ImGui::SliderFloat("X3", &colorBuffer->Data.X3, 2, 3);

	// 왼쪽


	function<void(wstring)> func6;
	func6 = bind(&DrawRect::Right_Left, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Right_Left"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func6);
	}

	function<void(wstring)> func7;
	func7 = bind(&DrawRect::Right_Right, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Right_Right"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func7);
	}

	ImGui::SliderFloat("X4", &colorBuffer->Data.X4, 3, 4);

	// 오른쪽



	function<void(wstring)> func8;
	func8 = bind(&DrawRect::Top_Left, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Top_Left"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func8);
	}

	function<void(wstring)> func9;
	func9 = bind(&DrawRect::Top_Right, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Top_Right"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func9);
	}

	ImGui::SliderFloat("X5", &colorBuffer->Data.X5, 4, 5);

	// 위

	function<void(wstring)> func10;
	func10 = bind(&DrawRect::Bottom_Left, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Bottom_Left"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func10);
	}

	function<void(wstring)> func11;
	func11 = bind(&DrawRect::Bottom_Right, this, placeholders::_1); // 파일 받아서 그림바꾸는부분

	if (ImGui::Button("Bottom_Right"))
	{
		Path::OpenFileDialog(L"", L"*.*", Textures, func11);
	}

	ImGui::SliderFloat("X6", &colorBuffer->Data.X6, 5, 6);

	// 아래
























	//세팅부분
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();

	colorBuffer->SetPSBuffer(0);

	//D3D::GetDC()->PSSetShaderResources(0,1,&srv); // 0번슬롯

	//D3D::GetDC()->PSSetShaderResources(1, 1,&srv2); // 1번슬롯

	D3D::GetDC()->PSSetShaderResources(2, 12, srvs); // 2번슬롯



	//실제그리는부분
	//D3D::GetDC()->Draw(6, 0);

	D3D::GetDC()->DrawIndexed(36, 0, 0);


}

void DrawRect::Front_Left(wstring val)
{
	//MessageBox(NULL, val.c_str(), L"확인", MB_OK);

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[0],
		NULL
	);

	assert(SUCCEEDED(hr));
}

void DrawRect::Front_Right(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[1],
		NULL
	);

}

void DrawRect::Back_Left(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[2],
		NULL
	);
}

void DrawRect::Back_Right(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[3],
		NULL
	);
}

void DrawRect::Left_Left(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[4],
		NULL
	);
}

void DrawRect::Left_Right(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[5],
		NULL
	);
}

void DrawRect::Right_Left(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[6],
		NULL
	);
}

void DrawRect::Right_Right(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[7],
		NULL
	);
}

void DrawRect::Top_Left(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[8],
		NULL
	);
}

void DrawRect::Top_Right(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[9],
		NULL
	);
}

void DrawRect::Bottom_Left(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[10],
		NULL
	);
}

void DrawRect::Bottom_Right(wstring val)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		val.c_str(),
		NULL,
		NULL,
		&srvs[11],
		NULL
	);
}

