#include "stdafx.h"
#include "Terrain.h"


#include "./Viewer/Freedom.h"
#include "./Viewer/Viewport.h"
#include "./Viewer/Perspective.h"

Terrain::Terrain(Material * material, wstring heightMap)
	: material(material)
{

	this->heightMap = new Texture(heightMap);

	CreateData();
	CreateNormalData();
	CreateBuffer();

}

Terrain::~Terrain()
{


	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE(heightMap);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(indexBuffer);

	Context::Delete();

}

void Terrain::Update()
{

}

void Terrain::Render()
{
	//ImGui::SliderFloat("y axis", &Angle, -179, 180);
	

	//ImGui::SliderInt("Brush Type", &brushBuffer->Data.Type, 0, 2);
	//ImGui::SliderInt("Brush Range", &brushBuffer->Data.Range, 1, 5);
	//ImGui::ColorEdit3("Brush Color", (float*)&brushBuffer->Data.Color);

	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);


	material->GetShader()->AsMatrix("World")->SetMatrix(W);
	material->GetShader()->DrawIndexed(0, 0, indexCount);



	/*brushBuffer->SetVSBuffer(2);
	worldBuffer->SetVSBuffer(1);
	material->PSSetBuffer();

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);*/
}

bool Terrain::GetHeight(OUT D3DXVECTOR3 & picked)
{
	Ray ray;
	
	D3DXVECTOR3 S; // 카메라 위치
	D3DXMATRIX V, P; 
	D3DXMATRIX W;

	Context::Get()->GetMainCamera()->Position(&S);
	Context::Get()->GetMainCamera()->Matrix(&V);
	Context::Get()->GetPerspective()->GetMatrix(&P);

	D3DXMatrixIdentity(&W);
	Context::Get()->GetViewport()->GetRay(&ray, S, W, V, P);


	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index[4];

			index[0] = (width + 1) * z + x;
			index[1] = (width + 1) * (z + 1) + x;
			index[2] = (width + 1) * z + x + 1;
			index[3] = (width + 1) * (z + 1) + x + 1;

			D3DXVECTOR3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;

			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance))
			{
				picked = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				//brushBuffer->Data.Location = picked;
				// 원래는 왜부에서 값을 받아와서 줘야함
				return true;
			}

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance))
			{
		
				picked = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				//brushBuffer->Data.Location = picked;
				return true;
			}
		}
	}



	return false;
}

float Terrain::GetHeightY(D3DXVECTOR3& position)
{
	D3DXVECTOR3 start = D3DXVECTOR3(position.x, 1000, position.z);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, -1, 0);

	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

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

void Terrain::CreateData()
{
	vector<D3DXCOLOR> heights;
	heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	// CreateVertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexTextureNormal[vertexCount];

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = (float)(heights[index].r * 255.0f) / 10.0f;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width;
				vertices[index].Uv.y = (float)z / height;
			}
		}
	}

	//CreateIndexData
	{
		indexCount = width * height * 6;
		indices = new UINT[indexCount];

		UINT index = 0;
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{

				indices[index + 0] = (width + 1) * z + x;
				indices[index + 1] = (width + 1) * (z + 1) + x;
				indices[index + 2] = (width + 1) * z + (x + 1);

				indices[index + 3] = (width + 1) * z + (x + 1);
				indices[index + 4] = (width + 1) * (z + 1) + x;
				indices[index + 5] = (width + 1) * (z + 1) + (x + 1);

				index += 6;
			}
		}
	}

}

void Terrain::CreateNormalData()
{

	{
		for (UINT i = 0; i < indexCount / 3; i++)
		{
			UINT index0 = indices[i * 3 + 0];
			UINT index1 = indices[i * 3 + 1];
			UINT index2 = indices[i * 3 + 2];

			VertexTextureNormal v0 = vertices[index0];
			VertexTextureNormal v1 = vertices[index1];
			VertexTextureNormal v2 = vertices[index2];

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

}

void Terrain::CreateBuffer()
{
	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureNormal) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//CreateVertexBuffer
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
}


D3DXVECTOR3 Terrain::GetPickingMouseY()
{
	//Viewport* vp = values->Viewport;

	//D3DXVECTOR3 start;
	//values->MainCamera->Position(&start);

	//D3DXMATRIX V, P;
	//values->MainCamera->Matrix(&V);
	//values->Perspective->GetMatrix(&P);

	//D3DXVECTOR3 direction = vp->GetDirection(V, P);
	//
	Ray ray;

	D3DXVECTOR3 S; // 카메라 위치
	D3DXMATRIX V, P;
	D3DXMATRIX W;

	Context::Get()->GetMainCamera()->Position(&S);
	Context::Get()->GetMainCamera()->Matrix(&V);
	Context::Get()->GetPerspective()->GetMatrix(&P);

	D3DXMatrixIdentity(&W);
	Context::Get()->GetViewport()->GetRay(&ray, S, W, V, P);



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
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance) == TRUE)
			{
				out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

				return out;
			}

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance) == TRUE)
			{
				out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

				return out;
			}
		}
	}

	return D3DXVECTOR3(0, 0, 0);
}


void Terrain::MovePoint(D3DXVECTOR3*Origin, D3DXVECTOR3* Direction, float Speed, D3DXVECTOR3& Rotation)
{

	D3DXVECTOR3 Normal(0, 0, 0);

	Normal.x = (*Direction).x - (*Origin).x;
	Normal.z = (*Direction).z - (*Origin).z;

	D3DXVec3Normalize(&Normal, &Normal); // 노말벡터화



										 // y회전


	Angle = atan2f(-Normal.z, Normal.x);
	


	Rotation.y = Angle - Math::PI * 0.5;




	UINT x = (UINT)(*Origin).x;
	UINT z = (UINT)(*Origin).z;


	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + (x + 1);
	index[3] = (width + 1) * (z + 1) + x + 1;

	D3DXVECTOR3 v[4]; // 포지션
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;


	float ddx = (x - v[0].x) / 1.0f;
	float ddz = (z - v[0].z) / 1.0f;



	D3DXVECTOR3 m[4]; // 노말벡터
	for (int i = 0; i < 4; i++)
		m[i] = vertices[index[i]].Normal;

	D3DXVECTOR3 temp;

	/*D3DXVECTOR3 tempTriIn_1;
	D3DXVECTOR3 tempTriIn_2;
	D3DXVECTOR3 tempTriInCross;

	D3DXVECTOR3 tempTriOut_1;
	D3DXVECTOR3 tempTriOut_2;
	D3DXVECTOR3 tempTriOutCross;*/
	if (ddx + ddz <= 1) // 안쪽삼각형
	{
		temp = (m[0] + m[1] + m[2]) / 3; // 안쪽삼각형의 법선벡터

		//tempTriIn_1 = m[1] - m[0]; // 첫번째 직선
		//tempTriIn_2 = m[2] - m[0]; // 두번째 직선

		//D3DXVec3Cross(&tempTriInCross, &tempTriIn_1, &tempTriIn_2);
		//D3DXVec3Normalize(&tempTriInCross, &tempTriInCross);


		// 원래는 직선두개로 법선벡터 구해서 쓴다.


										 // z축 회전
		Angle = temp.x;
		Rotation.z = Angle;
		//D3DXMatrixRotationX(&(*Rx), angle);

		// x축 회전
		Angle = -temp.z;
		Rotation.x = Angle;
		//D3DXMatrixRotationX(&(*Rz), angle);

	}
	else // 위쪽
	{
		temp = (m[1] + m[2] + m[3]) / 3; // 위쪽삼각형의 법선벡터


		//tempTriOut_1 = m[1] - m[3]; // 첫번째 직선
		//tempTriOut_2 = m[2] - m[3]; // 두번째 직선

		//D3DXVec3Cross(&tempTriOutCross, &tempTriOut_1, &tempTriOut_2);
		//D3DXVec3Normalize(&tempTriOutCross, &tempTriOutCross);


		Angle = temp.x;
		Rotation.z = Angle;
		//D3DXMatrixRotationX(&(*Rx), angle);


		Angle = -temp.z;
		Rotation.x = Angle;
		//D3DXMatrixRotationX(&(*Rz), angle);

	}







	// 이동

	if ((*Origin).x != (*Direction).x) // 두 값이 다르면 이동
	{
		(*Origin).x += Normal.x * Speed * Time::Delta();
	}

	if ((*Origin).z != (*Direction).z) // 두 값이 다르면 이동
	{
		(*Origin).z += Normal.z * Speed * Time::Delta();
	}


}