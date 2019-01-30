#include "stdafx.h"
#include "ModelMeshPart.h"
#include "ModelMesh.h"

void ModelMeshPart::Pass(UINT pass)
{
	this->pass = pass;
}

ModelMeshPart::ModelMeshPart()
{

}

ModelMeshPart::~ModelMeshPart()
{
}

void ModelMeshPart::Copy(ModelMeshPart ** clone)
{
	ModelMeshPart* part = new ModelMeshPart();
	part->materialName = materialName;
	part->vertices.assign(vertices.begin(), vertices.end());
	part->indices.assign(indices.begin(), indices.end());

	*clone = part;
}

void ModelMeshPart::Binding()
{
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureNormal) * vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &vertices[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//2. Index Buffer
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &indices[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void ModelMeshPart::Render()
{
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;


	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	UINT boneIndex = parent->ParentBoneIndex();

	material->GetShader()->AsScalar("BoneIndex")->SetInt(int(boneIndex));
	material->GetShader()->DrawIndexed(0, pass, indices.size());



	//D3D::GetDC()->DrawIndexed(indices.size(), 0, 0);;
}