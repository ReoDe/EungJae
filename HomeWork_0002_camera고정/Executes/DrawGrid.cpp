#include "stdafx.h"
#include "DrawGrid.h"

DrawGrid::DrawGrid(ExecuteValues*values)
	:Execute(values), width(20), height(20)
{

	shader = new Shader(Shaders + L"009_Grid.hlsl");
	worldBuffer = new WorldBuffer();

	//Create Vertex Data
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexTextureColor[vertexCount];

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = (float)0;
				vertices[index].Position.z = (float)z;

				vertices[index].Color = D3DXCOLOR(1, 1, 1, 1);

				vertices[index].Uv = D3DXVECTOR2((float)x, (float)z);
			}
		}//for(z)
	}

	//Create Index Data
	{
		indexCount = width * height * 6; // 칸에 6개씩
		indices = new UINT[indexCount];

		UINT i = 0;


		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = (width + 1) * z + x;

				indices[i + 0] = (width + 1) * z + x;
				indices[i + 1] = (width + 1) * (z + 1) + x;
				indices[i + 2] = (width + 1) * z + (x + 1);

				indices[i + 3] = (width + 1) * z + (x + 1);
				indices[i + 4] = (width + 1) * (z + 1) + x;
				indices[i + 5] = (width + 1) * (z + 1) + (x + 1);
			
				i += 6;
			}
		}//for(z)
	}





	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC 디스크립터 무조건 붙는 규칙
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureColor) * vertexCount; //용량
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
		desc.ByteWidth = sizeof(indices) * indexCount; //용량
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //만드는용도

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices; //pSysMem의p는 포인터

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	colorBuffer = new ColorBuffer();
	
	
	// ???
	rState[0] = new RasterizerState();

	rState[1] = new RasterizerState(); 
	rState[1]->FillMode(D3D11_FILL_WIREFRAME);




}


DrawGrid::~DrawGrid()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(rState[0]);
	SAFE_DELETE(rState[1]);

	SAFE_RELEASE(vertexBuffer);//I가붙는애들은 릴리즈로지워야됨
	SAFE_RELEASE(indexBuffer);//I가붙는애들은 릴리즈로지워야됨
	

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}


void DrawGrid::Update()
{
	
}

void DrawGrid::Render()
{
	

	


	//세팅부분
	UINT stride = sizeof(VertexTextureColor);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();

	colorBuffer->SetPSBuffer(0);



	//실제그리는부분
	//D3D::GetDC()->Draw(6, 0);

	rState[1]->RSSetState(); // 상태바꿔서 그리고
	{
		D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	}
	rState[0]->RSSetState(); // 원상복구


}

