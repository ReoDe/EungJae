#include "stdafx.h"
#include "Cube.h"
#include "../Model/ModelMesh.h"

Cube::Cube(wstring name, Vertex * vertices, int width, D3DXMATRIX & mat)
{	
	assert(vertices);
	objectName = name;
	arrWidth = width;
	object = new D3DXVECTOR3[arrWidth];

	for (int i = 0; i < arrWidth; i++)
	{
		object[i] = vertices[i].Position;
	}
	
	Initialize();
	SetWorldMatrix(mat);
}

Cube::Cube(wstring name, VertexTexture * vertices, int width, D3DXMATRIX & mat)
{
	assert(vertices);
	objectName = name;
	arrWidth = width;
	object = new D3DXVECTOR3[arrWidth];

	for (int i = 0; i < arrWidth; i++)
	{
		object[i] = vertices[i].Position;
	}

	Initialize();
	SetWorldMatrix(mat);
}

Cube::Cube(wstring name, VertexTextureNormal * vertices, int width, D3DXMATRIX & mat)
{
	assert(vertices);
	objectName = name;
	arrWidth = width;
	object = new D3DXVECTOR3[arrWidth];

	for (int i = 0; i < arrWidth; i++)
	{
		object[i] = vertices[i].Position;
	}

	Initialize();
	SetWorldMatrix(mat);
}

Cube::~Cube()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE(object);
}

void Cube::Update()
{
	

}

void Cube::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	worldBuffer->SetMatrix(matWorld);
	worldBuffer->SetVSBuffer(1);
	
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	shader->Render();



	rState[1]->RSSetState();
	D3D::GetDC()->DrawIndexed(36, 0, 0);
	rState[0]->RSSetState();

}

void Cube::Binding()
{
	//Create Vertex Buffers
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 8;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		for (int i = 0; i < 6; i++)
		{
			D3D11_SUBRESOURCE_DATA data = { 0 };
			data.pSysMem = vertices;

			HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
			assert(SUCCEEDED(hr));
		}
	}

	//Create Index Buffers
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * 36;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		for (int i = 0; i < 6; i++)
		{
			D3D11_SUBRESOURCE_DATA data = { 0 };
			data.pSysMem = indices;

			HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
			assert(SUCCEEDED(hr));
		}
	}
}

void Cube::GetVertexPosition(D3DXVECTOR3 ** arr)
{
	D3DXVECTOR3* temp = new D3DXVECTOR3[8];

	for (int i = 0; i < 8; i++)
	{
		temp[i] = vertices[i].Position;
	}
	
	*arr = temp;
}

void Cube::GetIndices(UINT ** arr)
{
	*arr = indices;
}

void Cube::Initialize()
{
	boneIndex = 0;
	bVisible = false;

	shader = new Shader(Shaders + L"011_BoundingBox.hlsl");
	worldBuffer = new WorldBuffer();

	rState[0] = new RasterizerState();
	rState[1] = new RasterizerState();
	rState[1]->FillMode(D3D11_FILL_WIREFRAME);
		
	SetVertices();
	Binding();
}

void Cube::SetWorldMatrix(D3DXMATRIX & mat)
{	
	matWorld = mat;
	worldBuffer->SetMatrix(matWorld);
}

void Cube::SetVertices()
{
	float MinX, MaxX;
	float MinY, MaxY;
	float MinZ, MaxZ;

	MinX = MaxX = object[0].x;
	MinY = MaxY = object[0].y;
	MinZ = MaxZ = object[0].z;

	for (int i = 0; i < arrWidth; i++)
	{
		if (object[i].x < MinX) MinX = object[i].x;
		if (object[i].x > MaxX) MaxX = object[i].x;

		if (object[i].y < MinY) MinY = object[i].y;
		if (object[i].y > MaxY) MaxY = object[i].y;

		if (object[i].z < MinZ) MinZ = object[i].z;
		if (object[i].z > MaxZ) MaxZ = object[i].z;
	}


	vertices = new Vertex[8];
	vertices[0].Position = D3DXVECTOR3(MinX, MinY, MinZ);
	vertices[1].Position = D3DXVECTOR3(MinX, MaxY, MinZ);
	vertices[2].Position = D3DXVECTOR3(MaxX, MinY, MinZ);
	vertices[3].Position = D3DXVECTOR3(MaxX, MaxY, MinZ);

	vertices[4].Position = D3DXVECTOR3(MinX, MinY, MaxZ);
	vertices[5].Position = D3DXVECTOR3(MinX, MaxY, MaxZ);
	vertices[6].Position = D3DXVECTOR3(MaxX, MinY, MaxZ);
	vertices[7].Position = D3DXVECTOR3(MaxX, MaxY, MaxZ);

	indices = new UINT[36]
	{
		0,1,2,2,1,3,
		2,3,6,6,3,7,
		6,7,4,4,7,5,
		4,5,0,0,5,1,
		1,5,3,3,5,7,
		4,0,6,6,0,2
	};
}

