#include "stdafx.h"
#include "DrawGrid.h"

DrawGrid::DrawGrid(ExecuteValues*values)
	:Execute(values), width(256), height(256)
{
	UINT* heightMap = NULL;
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

				BYTE pixel = ((heightMap[index] & 0x00ff0000) >> 16); // R���������



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
		indexCount = width * height * 6; // ĭ�� 6����
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

			vertices[index0].Noraml += normal;
			vertices[index1].Noraml += normal;
			vertices[index2].Noraml += normal;
		}

		for (UINT i = 0; i < vertexCount; i++)
			D3DXVec3Normalize(&vertices[i].Noraml, &vertices[i].Noraml);


	}



	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureColorNormal) * vertexCount; //�뷮
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
		desc.ByteWidth = sizeof(indices) * indexCount; //�ϴ� ����
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









	//�ﰢ��

	triShader = new Shader(Shaders + L"010_Triangle.hlsl");
	triPosition = D3DXVECTOR3(0, 0, 0);

	Vertex* triVertices = new Vertex[3];
	triVertices[0].Position = D3DXVECTOR3(-0.5f, 1.0f, 0);
	triVertices[1].Position = D3DXVECTOR3(0.5f, 1.0f, 0);
	triVertices[2].Position = D3DXVECTOR3(0, 0, 0);



	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 3; //�뷮
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = triVertices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &triVertexBuffer);
		assert(SUCCEEDED(hr));
	}





	// �븻���͸����
	{
		shader2 = new Shader(Shaders + L"010_HeightColor.hlsl");
		vertexCount2 = vertexCount * 2;
		normalVertices = new Vertex[vertexCount2]; // 2��

		for (UINT i = 0; i < vertexCount; i++)
		{
			if (i % 2 == 1) // Ȧ�� , vertex ����Ʈ
			{
				normalVertices[2 * i - 1].Position = vertices[i].Position;
			}
			else // ¦�� , vertexNormal ����Ʈ
			{
				normalVertices[2 * i].Position = vertices[i].Position + vertices[i].Noraml;
			}
		}
	}
	
	//Create Index Data
	{
		indexCount2 = vertexCount2 * 2;
		normalIndices = new UINT[indexCount2];

		for (UINT i = 0; i < vertexCount2; i++)
		{
			normalIndices[i] = i;
		}
	}




	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * vertexCount2; //�뷮
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = normalVertices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &normalVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 }; //DESC ��ũ���� ������ �ٴ� ��Ģ
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(normalIndices) * indexCount2; //�뷮
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //����¿뵵

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = normalIndices; //pSysMem��p�� ������

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &normalIndexBuffer);
		assert(SUCCEEDED(hr));
	}






}


DrawGrid::~DrawGrid()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(rState[0]);
	SAFE_DELETE(rState[1]);

	SAFE_RELEASE(vertexBuffer);//I���ٴ¾ֵ��� ������������ߵ�
	SAFE_RELEASE(indexBuffer);//I���ٴ¾ֵ��� ������������ߵ�
	

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}


void DrawGrid::Update()
{
	
}

void DrawGrid::Render()
{
	
	ImGui::SliderFloat3("Direction", (float*)&colorBuffer->Data.Direction, -1, 1);
	
	//���úκ�
	UINT stride = sizeof(VertexTextureColorNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//������ 3���� ��� �׷���

	//D3DXMATRIX W;
	//D3DXMatrixIdentity(&W);
	//worldBuffer->SetMatrix(W);

	worldBuffer->SetVSBuffer(1);
	shader->Render();
	colorBuffer->SetPSBuffer(0);

	rState[1]->RSSetState(); // ���¹ٲ㼭 �׸���
	{
		D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	}
	rState[0]->RSSetState(); // ���󺹱�



	// �븻����

	//���úκ�
	stride = sizeof(Vertex);
	offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &normalVertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(normalIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	worldBuffer->SetVSBuffer(1);
	shader2->Render();

	colorBuffer->SetPSBuffer(0);

	rState[1]->RSSetState(); // ���¹ٲ㼭 �׸���
	{
		D3D::GetDC()->DrawIndexed(indexCount2, 0, 0);
	}
	rState[0]->RSSetState(); // ���󺹱�



}
// 2�������ͷ� �� ����, �� �Լ����� �����ؼ� �������ٰ��̱� ����
void DrawGrid::ReadPixels(wstring file, UINT ** heightMap)
{
	HRESULT hr;

	ID3D11Texture2D* src;
	hr = D3DX11CreateTextureFromFile
	(
		D3D::GetDevice(), (file).c_str(), NULL, NULL, (ID3D11Resource **)&src, NULL
	);
	assert(SUCCEEDED(hr)); // ���⼭ ���� ����





	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc); // �극��ũ����Ʈ

	// ���� �ؽ�Ʈ �����Ұ����� �����ϱ����� �ؽ��ĸ� �ϳ� �� �����.

	D3D11_TEXTURE2D_DESC destDesc;
	ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
	destDesc.Width = srcDesc.Width;
	destDesc.Height = srcDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = srcDesc.Format;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//cpu���� �����ؼ� ���� �� �ֵ��� �����ش�.( ������ gpu)
	destDesc.Usage = D3D11_USAGE_STAGING;
	// ������� ��������


	ID3D11Texture2D* dest;
	hr = D3D::GetDevice()->CreateTexture2D(&destDesc, NULL, &dest);
	assert(SUCCEEDED(hr));

	//�����ϴ� �ؽ���
	hr = D3DX11LoadTextureFromTexture
	(
		// vram���� dc
		D3D::GetDC(), src, NULL, dest
	);
	assert(SUCCEEDED(hr));

	//������ �ؽ��ĸ� ���Ϸ� �ٽ� ����
	
	/*hr = D3DX11SaveTextureToFile
	(
		D3D::GetDC(), dest, D3DX11_IFF_PNG, L"Test.png"
	);*/

	
	
	*heightMap = new UINT[width * height];

	
	// ���ҽ��� ���� �����ö��� 

	D3D11_MAPPED_SUBRESOURCE map;
	D3D::GetDC()->Map(dest, 0, D3D11_MAP_READ, NULL, &map);
	{
		width = destDesc.Width;
		height = destDesc.Height;

		memcpy(*heightMap, map.pData, sizeof(UINT) * width * height);

	}
	D3D::GetDC()->Unmap(dest, 0);
	//map�� �ϸ� �Ѱ�� ��ü�� ������ Ǯ������Ѵ�. dest

	width--;
	height--;

	SAFE_RELEASE(src);
	SAFE_RELEASE(dest);
	


}

float DrawGrid::GetY()
{
	UINT x = (UINT)triPosition.x;
	UINT z = (UINT)triPosition.z;

	if (x < 0 || x >= width) return 0.0f; // ���� ���� ������
	if (z < 0 || z >= height) return 0.0f;

	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + (x + 1);
	index[3] = (width + 1) * (z + 1) + x + 1;
	// 4���� ������ ��ġ 


	D3DXVECTOR3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;


	float ddx = (x - v[0].x) / 1.0f; // ������ ĭ�� ũ��
	float ddz = (z - v[0].z) / 1.0f; // ������ ĭ�� ũ��

	D3DXVECTOR3 temp;
	if ((ddx + ddz) <= 1)
	{
		temp = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0])* ddz;
	} // �簢���� �ؿ� �ﰢ�� interpolation
	else
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;

		temp = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3])* ddz;
	}// �簢���� ���� �簢�� interpolation


	return temp.y;
}

