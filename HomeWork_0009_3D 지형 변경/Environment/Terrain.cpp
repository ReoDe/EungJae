#include "stdafx.h"
#include "Terrain.h"
#

Terrain::Terrain(ExecuteValues * values, Material * material, wstring heightMap)
	: values(values), material(material), capacity(10.0f), BrushType(0)
{
	this->heightMap = new Texture(heightMap);
	this->heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);
	// 높이조정

	colorMap = new Texture(Textures + L"Floor.png");
	// 텍스쳐 2개

	colorMap2 = new Texture(Textures + L"Dirt2.png");
	colorMap3 = new Texture(Textures + L"Dirt3.png");


	alphaMap = new Texture(Textures + L"AlphaMap256.png");


	tile = Tile_A;

	worldBuffer = new WorldBuffer();
	brushBuffer = new BrushBuffer();

	CreateData();
	CreateNormalData();
	CreateBuffer();

}

Terrain::~Terrain()
{
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(indexBuffer);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE(heightMap);
	SAFE_DELETE(worldBuffer);

}

void Terrain::Update()
{
	if (D3DXVec3Length(&brushBuffer->Data.Location) > 0.0f)
	{
		if (BrushType == 0) // y축
		{
			UINT type = brushBuffer->Data.Type;
			if (type == Rect && Mouse::Get()->Press(0))
				AdjustHeight(brushBuffer->Data.Location);
			if (type == Circle && Mouse::Get()->Press(0))
				circleHeight(brushBuffer->Data.Location);
			if (type == Cone && Mouse::Get()->Press(0))
				coneHeight(brushBuffer->Data.Location);
		}
		else // 색 변경
		{
			UINT type = brushBuffer->Data.Type;

			
			if (type == Rect && Mouse::Get()->Press(0))
			{
				switch (tile)
				{
				case Tile_A:
					AdjustColorMapA(brushBuffer->Data.Location);
					break;
				case Tile_B:
					AdjustColorMapB(brushBuffer->Data.Location);
					break;
				case Tile_C:
					AdjustColorMapC(brushBuffer->Data.Location);
					break;
				case Tile_D:
					AdjustColorMapD(brushBuffer->Data.Location);
					break;
				}
			} 

			if (type == Circle || type == Cone && Mouse::Get()->Press(0))
			{
				switch (tile)
				{
				case Tile_A:
					circleAdjustColorMapA(brushBuffer->Data.Location);
					break;
				case Tile_B:
					circleAdjustColorMapB(brushBuffer->Data.Location);
					break;
				case Tile_C:
					circleAdjustColorMapC(brushBuffer->Data.Location);
					break;
				case Tile_D:
					circleAdjustColorMapD(brushBuffer->Data.Location);
					break;
				}
			}
			
			

		}
		
	}




}

void Terrain::Render()
{

	
	ImGui::RadioButton("Y brush",&BrushType, 0); ImGui::SameLine(120);
	ImGui::RadioButton("tarrian brush", &BrushType, 1);
	ImGui::SliderInt("Brush Type", &brushBuffer->Data.Type, 1, 3);
	if (BrushType == 1)
	{
		ImGui::SliderInt("What Tile", (int*)&tile, 0, 3);
	}
	ImGui::SliderInt("Brush Range", &brushBuffer->Data.Range, 1, 10);
	ImGui::ColorEdit3("Brush Color", (float*)&brushBuffer->Data.Color);
	
	//ImGui::DragFloat("Capacity", &capacity, 0.5f);

	if (ImGui::Button("save Botton")) SaveData();
	ImGui::Separator();
	ImGui::Text("test");


	

	UINT stride = sizeof(VertexColorTextureNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	colorMap->SetShaderResource(5);
	alphaMap->SetShaderResource(6);
	colorMap2->SetShaderResource(7);
	colorMap3->SetShaderResource(8);

	// 이걸로 다 해결
	heightMap->SetShaderResource(9);

	brushBuffer->SetVSBuffer(2);
	worldBuffer->SetVSBuffer(1);
	material->PSSetBuffer();

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

bool Terrain::GetHeight(OUT D3DXVECTOR3 & picked)
{
	Ray ray;
	
	D3DXVECTOR3 S; // 카메라 위치
	D3DXMATRIX V, P; 
	D3DXMATRIX W;
	values->MainCamera->Position(&S);
	values->MainCamera->Matrix(&V);
	values->Perspective->GetMatrix(&P);

	D3DXMatrixIdentity(&W);
	values->Viewport->GetRay(&ray, S, W, V, P);

	brushBuffer->Data.Location = D3DXVECTOR3(0, 0, 0); // 0 초기화

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
				brushBuffer->Data.Location = picked;
				// 원래는 왜부에서 값을 받아와서 줘야함
				return true;
			}

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &ray.Position, &ray.Direction, &u, &v, &distance))
			{
		
				picked = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				brushBuffer->Data.Location = picked;
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
	
	// 여기 확인좀하자



	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	// CreateVertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexColorTextureNormal[vertexCount];

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

				//vertices[index].Color.g = heights[index].g;

				//heights[index].g = 0;
				//heights[index].b = 0; 
				//heights[index].a = 0;
				// gba 초기화
			}
		}
	}

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);

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

			VertexColorTextureNormal v0 = vertices[index0];
			VertexColorTextureNormal v1 = vertices[index1];
			VertexColorTextureNormal v2 = vertices[index2];

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
		desc.ByteWidth = sizeof(VertexColorTextureNormal) * vertexCount;
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

void Terrain::AdjustHeight(D3DXVECTOR3 & location)
{
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;
	

	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			vertices[index].Position.y += 10.0f * Time::Delta();
		}
	}

	CreateNormalData();

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer, 0, NULL, &vertices[0], sizeof(VertexColorTextureNormal), vertexCount
	);
}

void Terrain::circleHeight(D3DXVECTOR3 & location)
{
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			// 만약 정점위치가 반지름보다 크면 브레이크
			// 중점은 location.x, location.z

			// 마우스 중점과 버텍스 사이의 거리의 제곱
			float distance2 = (vertices[index].Position.x - location.x) * (vertices[index].Position.x - location.x)
						+ (vertices[index].Position.z - location.z) * (vertices[index].Position.z - location.z);

			float distance = sqrtf(distance2);
			float Radius = size;

			// 만약 반지름보다 두점사이의 거리가 가깝거나 같다면 표시
			if (Radius > distance)
			{
				// 중앙으로부터 거리에 비례해서 증가해야함
				vertices[index].Position.y += 10.0f * Time::Delta() * (0.5f * cosf(D3DX_PI * distance / (float)size) + 0.5f);
			}
				
		}
	}

	CreateNormalData();

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer, 0, NULL, &vertices[0], sizeof(VertexColorTextureNormal), vertexCount
	);

}

void Terrain::coneHeight(D3DXVECTOR3 & location)
{
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			// 만약 정점위치가 반지름보다 크면 브레이크
			// 중점은 location.x, location.z

			// 마우스 중점과 버텍스 사이의 거리의 제곱
			float distance2 = (vertices[index].Position.x - location.x) * (vertices[index].Position.x - location.x)
				+ (vertices[index].Position.z - location.z) * (vertices[index].Position.z - location.z);

			float distance = sqrtf(distance2);
			float Radius = size;

			// 만약 반지름보다 두점사이의 거리가 가깝거나 같다면 표시
			if (Radius > distance)
			{
				// 중앙으로부터 거리에 비례해서 증가해야함
				vertices[index].Position.y += 10.0f * Time::Delta();
			}

		}
	}

	CreateNormalData();

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer, 0, NULL, &vertices[0], sizeof(VertexColorTextureNormal), vertexCount
	);
}

void Terrain::AdjustColorMapA(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			// 전부다 그려짐. 이거 바꺼야되는데
			//if (heights[index].g != 0 && heights[index].b != 0 && heights[index].a != 0) // g가 그려지려면 셋다 0일때
			{

				heights[index].g = 0;
				heights[index].b = 0;
				heights[index].a = 0;
			}
		}
	}



	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);


}

void Terrain::AdjustColorMapB(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			// 전부다 그려짐. 이거 바꺼야되는데
			//if (heights[index].g != 1) // g가 그려지려면 셋다 0일때
			{

				heights[index].g = 1;
				heights[index].b = 0;
				heights[index].a = 0;
			}
		}
	}


	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);

}

void Terrain::AdjustColorMapC(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			// 전부다 그려짐. 이거 바꺼야되는데
			//if (heights[index].g != 0 && heights[index].b != 1 && heights[index].a != 0) // g가 그려지려면 셋다 0일때
			{

				heights[index].g = 0;
				heights[index].b = 1;
				heights[index].a = 0;
			}
		}
	}


	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);

}

void Terrain::AdjustColorMapD(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{
			UINT index = (width + 1) * z + x;

			// 전부다 그려짐. 이거 바꺼야되는데
			//if (heights[index].g != 0 && heights[index].b != 0 && heights[index].a != 1) // g가 그려지려면 셋다 0일때
			{

				heights[index].g = 0;
				heights[index].b = 0;
				heights[index].a = 1;
			}
		}
	}




	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);

}

void Terrain::circleAdjustColorMapA(D3DXVECTOR3 & location)
{

	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{

			UINT index = (width + 1) * z + x;

			float distance2 = (vertices[index].Position.x - location.x) * (vertices[index].Position.x - location.x)
				+ (vertices[index].Position.z - location.z) * (vertices[index].Position.z - location.z);

			float distance = sqrtf(distance2);
			float Radius = size;


			if (Radius > distance)
			{
				heights[index].g = 0;
				heights[index].b = 0;
				heights[index].a = 0;
			}

		}
	}



	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);
}

void Terrain::circleAdjustColorMapB(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{

			UINT index = (width + 1) * z + x;

			float distance2 = (vertices[index].Position.x - location.x) * (vertices[index].Position.x - location.x)
				+ (vertices[index].Position.z - location.z) * (vertices[index].Position.z - location.z);

			float distance = sqrtf(distance2);
			float Radius = size;


			if (Radius > distance)
			{
				heights[index].g = 1;
				heights[index].b = 0;
				heights[index].a = 0;
			}

		}
	}



	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);
}

void Terrain::circleAdjustColorMapC(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{

			UINT index = (width + 1) * z + x;

			float distance2 = (vertices[index].Position.x - location.x) * (vertices[index].Position.x - location.x)
				+ (vertices[index].Position.z - location.z) * (vertices[index].Position.z - location.z);

			float distance = sqrtf(distance2);
			float Radius = size;


			if (Radius > distance)
			{
				heights[index].g = 0;
				heights[index].b = 1;
				heights[index].a = 0;
			}

		}
	}



	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);
}

void Terrain::circleAdjustColorMapD(D3DXVECTOR3 & location)
{
	// 범위
	UINT size = (UINT)brushBuffer->Data.Range;

	D3D11_BOX box;


	box.left = (UINT)location.x - size;
	box.top = (UINT)location.z + size;
	box.right = (UINT)location.x + size;
	box.bottom = (UINT)location.z - size;

	for (UINT z = box.bottom; z <= box.top; z++)
	{
		for (UINT x = box.left; x <= box.right; x++)
		{

			UINT index = (width + 1) * z + x;

			float distance2 = (vertices[index].Position.x - location.x) * (vertices[index].Position.x - location.x)
				+ (vertices[index].Position.z - location.z) * (vertices[index].Position.z - location.z);

			float distance = sqrtf(distance2);
			float Radius = size;


			if (Radius > distance)
			{
				heights[index].g = 0;
				heights[index].b = 0;
				heights[index].a = 1;
			}

		}
	}



	CreateNormalData();

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);
}


void Terrain::SaveData()
{
	// heightMap파일에 asdf파일이 덮어씌워짐
	//asdf->SaveFile(heightMap->GetFile());
	
	// asdf 파일에 heightMap파일이 저장됨
	//heightMap->SaveFile(asdf->GetFile());

	/*
	일단 지금 heightMap에 있는 a값을 읽어서 D3DXCOLOR벡터에 저장해 놓았음.

	그리고 그 D3DXCOLOR벡터에 있는 값들을 기반으로 정점을 만듬

	맵 에딧으로 수정을하면 정점데이터가 수정됨.

	===

	이것을 다시 저장하기 위해선 정점데이터를 다시 얻어옴.
	*/



	// a는 높이
	// 여기 확인좀하자
	//ModifiedData.assign(vertexCount, D3DXCOLOR());
	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	// CreateVertexData
	{
		vertexCount = (width + 1) * (height + 1);
		

		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				UINT index = (width + 1) * z + x;

				heights[index].r = vertices[index].Position.y * 10.0f / 255.0f;
			}
		}
	}

	heightMap->ModifiedPixels(DXGI_FORMAT_R8G8B8A8_UNORM, heights);

	heightMap->SaveFile(heightMap->GetFile());
	
}


D3DXVECTOR3 Terrain::GetPickingMouseY()
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
	if (ddx + ddz <= 1) // 안쪽삼각형
	{
		temp = (m[0] + m[1] + m[2]) / 3; // 안쪽삼각형의 법선벡터


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

		Angle = temp.x;
		Rotation.z = Angle;
		//D3DXMatrixRotationX(&(*Rx), angle);


		Angle = temp.z;
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