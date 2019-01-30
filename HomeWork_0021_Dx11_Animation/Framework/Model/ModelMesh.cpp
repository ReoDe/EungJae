#include "framework.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "ModelBone.h"

ModelMesh::ModelMesh()
{
	
}

ModelMesh::~ModelMesh()
{
	for (ModelMeshPart* part : meshParts)
		SAFE_DELETE(part);
}

void ModelMesh::Render()
{
	UINT stride = sizeof(VertexTextureNormalBlend);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (ModelMeshPart* part : meshParts)
		part->Render();
}

void ModelMesh::Copy(ModelMesh ** clone)
{
	ModelMesh* mesh = new ModelMesh();
	mesh->name = name;
	mesh->parentBoneIndex = parentBoneIndex;

	mesh->vertexCount = this->vertexCount;
	mesh->vertices = new VertexTextureNormalBlend[this->vertexCount];
	memcpy(mesh->vertices, this->vertices, sizeof(VertexTextureNormalBlend) * vertexCount);

	for (ModelMeshPart* part : meshParts)
	{
		ModelMeshPart* temp = NULL;
		part->Copy(&temp);

		temp->parent = mesh;
		mesh->meshParts.push_back(temp);
	}
	*clone = mesh;
}

void ModelMesh::Pass(UINT pass)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(pass);
}

void ModelMesh::Binding()
{
	//D3D11_BUFFER_DESC desc;
	//D3D11_SUBRESOURCE_DATA data;

	////1. Vertex Buffer
	//{
	//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	//	desc.Usage = D3D11_USAGE_DEFAULT;
	//	desc.ByteWidth = sizeof(VertexTextureNormalBlend) * vertexCount;
	//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	//	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	//	data.pSysMem = vertices;

	//	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	//	assert(SUCCEEDED(hr));
	//}

	CsResource::CreateRawBuffer(sizeof(VertexTextureNormalBlend) * vertexCount, vertices, &vertexBuffer);
}
