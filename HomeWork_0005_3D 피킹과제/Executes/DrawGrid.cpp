#include "stdafx.h"
#include "DrawGrid.h"

DrawGrid::DrawGrid(ExecuteValues * values)
	: Execute(values)
	, width(256), height(256)
	, CubePositionDirection(0,0,0)
{
	UINT* heightMap = NULL;
	//ReadPixels(Textures + L"HeightRed256.png", &heightMap);
	ReadPixels(Textures + L"HeightMap256.png", &heightMap);

	shader = new Shader(Shaders + L"010_HeightColor.hlsl");
	worldBuffer = new WorldBuffer();

	
	//Create Vertex Data
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexTextureColorNormal[vertexCount];

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				BYTE pixel = ((heightMap[index] & 0x00FF0000) >> 16);

				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = (float)pixel / 7.5f;
				vertices[index].Position.z = (float)z;

				vertices[index].Color = D3DXCOLOR(1, 1, 1, 1);

				vertices[index].Uv = D3DXVECTOR2((float)x, (float)z);
			}
		}//for(z)
	}

	//Create Index Data
	{
		indexCount = width * height * 6;
		indices = new UINT[indexCount];

		UINT i = 0;
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = (width + 1) * z + x;

				indices[i + 0] = (width + 1) * z + x; //0
				indices[i + 1] = (width + 1) * (z + 1) + x; //1
				indices[i + 2] = (width + 1) * z + x + 1; //2

				indices[i + 3] = (width + 1) * z + x + 1; //2
				indices[i + 4] = (width + 1) * (z + 1) + x; //1
				indices[i + 5] = (width + 1) * (z + 1) + x + 1; //3

				i += 6;
			}
		}//for(z)
	}

	//Create Normal Data
	{
		for (UINT i = 0; i < indexCount / 3; i++)
		{
			UINT index0 = indices[i * 3 + 0];
			UINT index1 = indices[i * 3 + 1];
			UINT index2 = indices[i * 3 + 2];

			VertexTextureColorNormal v0 = vertices[index0];
			VertexTextureColorNormal v1 = vertices[index1];
			VertexTextureColorNormal v2 = vertices[index2];

			D3DXVECTOR3 d1 = v1.Position - v0.Position;
			D3DXVECTOR3 d2 = v2.Position - v0.Position;

			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &d1, &d2);

			vertices[index0].Normal += normal;
			vertices[index1].Normal += normal;
			vertices[index2].Normal += normal;
		}

		for (UINT i = 0; i < vertexCount; i++)
			D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
	}


	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureColorNormal) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	colorBuffer = new ColorBuffer();

	rState[0] = new RasterizerState();
	rState[1] = new RasterizerState();
	rState[1]->FillMode(D3D11_FILL_WIREFRAME);
	
	
	// 삼각형

	triShader = new Shader(Shaders + L"010_Triangle.hlsl");
	triPosition = D3DXVECTOR3(0, 0, 0);

	Vertex* triVertices = new Vertex[3];
	triVertices[0].Position = D3DXVECTOR3(-0.5f, 1.0f, 0.0f);
	triVertices[1].Position = D3DXVECTOR3( 0.5f, 1.0f, 0.0f);
	triVertices[2].Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = triVertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &triVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//큐브 만들기

	CubeShader = new Shader(Shaders + L"010_Triangle.hlsl");
	CubePosition = D3DXVECTOR3(0, 0, 0);

	// 점 6 * 4 필요
	float Pos[24][3] =
	{
		{ -0.5f,-0.5f,-0.5f },{ -0.5f,0.5f,-0.5f },{ 0.5f,-0.5f,-0.5f },{ 0.5f,0.5f,-0.5f }, // 정면
	{ -0.5f,-0.5f,0.5f },{ -0.5f,0.5f,0.5f },{ 0.5f,-0.5f,0.5f },{ 0.5f,0.5f,0.5f },// 뒤
	{ -0.5f,-0.5f,0.5f },{ -0.5f,0.5f,0.5f },{ -0.5f,-0.5f,-0.5f },{ -0.5f,0.5f,-0.5f }, // 좌
	{ 0.5f,-0.5f,0.5f },{ 0.5f, 0.5f,0.5f },{ 0.5f,-0.5f,-0.5f },{ 0.5f,0.5f,-0.5f }, // 우
	{ -0.5f,0.5f,-0.5f },{ -0.5f,0.5f,0.5f },{ 0.5f,0.5f,-0.5f },{ 0.5f,0.5f,0.5f }, //상
	{ -0.5f,-0.5f,-0.5f, },{ -0.5f,-0.5f,0.5f },{ 0.5f,-0.5f,-0.5f },{ 0.5f,-0.5f,0.5f }//하

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

DrawGrid::~DrawGrid()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(colorBuffer);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void DrawGrid::Update()
{
	

	if (Mouse::Get()->Press(0))
	{
		CubePositionDirection = GetPickingMouseY();
		// 클릭값을 원하는 방향이 넣어놈
	}
	// 포지션을 그쪽 방향으로 천천히 이동
	// x축 y축 비율 가져와서 이동하게.
	if (CubePosition.x - CubePositionDirection.x > 0.3f || 
		CubePosition.x - CubePositionDirection.x < -0.3f)
		MovePoint(&CubePosition, &CubePositionDirection, 50);
				// 현재 포지션,		마우스클릭포지션,    속도





}

void DrawGrid::Render()
{
	ImGui::SliderFloat3("Direction", (float *)&colorBuffer->Data.Direction, -1, 1);
	ImGui::LabelText("FPS", "%0.0f", ImGui::GetIO().Framerate);

	UINT stride = sizeof(VertexTextureColorNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	worldBuffer->SetMatrix(W);

	worldBuffer->SetVSBuffer(1);
	shader->Render();

	colorBuffer->SetPSBuffer(0);

	//rState[1]->RSSetState();
	{
		D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	}
	//rState[0]->RSSetState();


	//삼각형 그리기
	/*ImGui::SliderFloat("PositionX", &triPosition.x, 0, width - 1);
	ImGui::SliderFloat("PositionZ", &triPosition.z, 0, height - 1);

	stride = sizeof(Vertex);
	offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &triVertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/

	//D3DXMATRIX T;
	////D3DXMatrixTranslation(&T, triPosition.x, GetY(), triPosition.z);
	////D3DXMatrixTranslation(&T, triPosition.x, GetPickingY(), triPosition.z);

	//triPosition = GetPickingMouseY();
	//D3DXMatrixTranslation(&T, triPosition.x, triPosition.y, triPosition.z);
	//worldBuffer->SetMatrix(T);

	/*values->ViewProjection->SetVSBuffer(0);
	worldBuffer->SetVSBuffer(1);
	triShader->Render();

	D3D::GetDC()->Draw(3, 0);*/



	// 육면체 그리기

	ImGui::SliderFloat("PositionX", &CubePosition.x, 0, width - 1);
	ImGui::SliderFloat("PositionZ", &CubePosition.z, 0, height - 1);


	stride = sizeof(VertexTextureColorNormal);
	offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &CubeVertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(CubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3DXMATRIX T;
	//CubePosition = GetPickingMouseY();
	D3DXMatrixTranslation(&T, CubePosition.x, GetPickingY() + 1, CubePosition.z);
	worldBuffer->SetMatrix(T);




	worldBuffer->SetVSBuffer(1);
	CubeShader->Render();

	colorBuffer->SetPSBuffer(0);

	D3D::GetDC()->DrawIndexed(36, 0, 0);

}

void DrawGrid::ReadPixels(wstring file, UINT** heightMap)
{
	HRESULT hr;

	ID3D11Texture2D* src;
	hr = D3DX11CreateTextureFromFile
	(
		D3D::GetDevice(), file.c_str(), NULL, NULL, (ID3D11Resource **)&src, NULL
	);
	assert(SUCCEEDED(hr));

	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	
	D3D11_TEXTURE2D_DESC destDesc;
	ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
	destDesc.Width = srcDesc.Width;
	destDesc.Height = srcDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = srcDesc.Format;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	destDesc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D* dest;
	hr = D3D::GetDevice()->CreateTexture2D(&destDesc, NULL, &dest);
	assert(SUCCEEDED(hr));

	hr = D3DX11LoadTextureFromTexture
	(
		D3D::GetDC(), src, NULL, dest
	);
	assert(SUCCEEDED(hr));

	
	/*hr = D3DX11SaveTextureToFile
	(
		D3D::GetDC(), dest, D3DX11_IFF_PNG, L"Test.png"
	);
	assert(SUCCEEDED(hr));*/


	*heightMap = new UINT[destDesc.Width * destDesc.Height];

	D3D11_MAPPED_SUBRESOURCE map;
	D3D::GetDC()->Map(dest, 0, D3D11_MAP_READ, NULL, &map);
	{
		width = destDesc.Width;
		height = destDesc.Height;

		memcpy(*heightMap, map.pData, sizeof(UINT) * width * height);
	}
	D3D::GetDC()->Unmap(dest, 0);

	width--;
	height--;

	SAFE_RELEASE(src);
	SAFE_RELEASE(dest);
}

float DrawGrid::GetY()
{
	UINT x = (UINT)CubePosition.x;
	UINT z = (UINT)CubePosition.z;

	if (x < 0 || x >= width) return 0.0f;
	if (z < 0 || z >= height) return 0.0f;

	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + (x + 1);
	index[3] = (width + 1) * (z + 1) + x + 1;

	D3DXVECTOR3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;


	float ddx = (x - v[0].x) / 1.0f;
	float ddz = (z - v[0].z) / 1.0f;

	
	D3DXVECTOR3 temp;
	if (ddx + ddz <= 1)
	{
		temp = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	}
	else
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;

		temp = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}


	return temp.y;
}

float DrawGrid::GetPickingY()
{
	D3DXVECTOR3 start = D3DXVECTOR3(CubePosition.x, 1000, CubePosition.z);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, -1, 0);

	UINT x = (UINT)CubePosition.x;
	UINT z = (UINT)CubePosition.z;

	if (x < 0 || x >= width) return 0.0f;
	if (z < 0 || z >= height) return 0.0f;


	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + (x + 1);
	index[3] = (width + 1) * (z + 1) + x + 1;

	D3DXVECTOR3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	float y = 0.0f;
	float u, v, distance;
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
	{
		D3DXVECTOR3 result;
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
		
		y = result.y;
	}

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
	{
		D3DXVECTOR3 result;
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

		y = result.y;
	}

	return y;
}

D3DXVECTOR3 DrawGrid::GetPickingMouseY()
{
	Viewport* vp = values->Viewport;
	
	D3DXVECTOR3 start;
	values->MainCamera->Position(&start);

	D3DXMATRIX V, P;
	values->MainCamera->Matrix(&V);
	values->Perspective->GetMatrix(&P);

	D3DXVECTOR3 direction = vp->GetDirection(V, P);

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index[4];
			index[0] = (width + 1) * z + x;
			index[1] = (width + 1) * (z + 1) + x;
			index[2] = (width + 1) * z + (x + 1);
			index[3] = (width + 1) * (z + 1) + x + 1;

			D3DXVECTOR3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;


			D3DXVECTOR3 out;
			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
			{
				out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

				return out;
			}

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
			{
				out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

				return out;
			}
		}
	}

	return D3DXVECTOR3(0, 0, 0);
}

void DrawGrid::MovePoint(D3DXVECTOR3*Origin, D3DXVECTOR3* Direction, float Speed)
{
	
	D3DXVECTOR3 Normal(0, 0, 0);

	Normal.x = (*Direction).x - (*Origin).x;
	Normal.z = (*Direction).z - (*Origin).z;

	D3DXVec3Normalize(&Normal, &Normal); // 노말벡터화



	if ((*Origin).x != (*Direction).x) // 두 값이 다르면 이동
	{
		(*Origin).x += Normal.x * Speed * Time::Delta();
	}

	if ((*Origin).z != (*Direction).z) // 두 값이 다르면 이동
	{
		(*Origin).z += Normal.z * Speed * Time::Delta();
	}


}
