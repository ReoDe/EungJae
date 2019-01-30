#include "framework.h"
#include "GameModel.h"

GameModel::GameModel(wstring shaderFile, wstring matFile, wstring meshFile)
	: shaderFile(shaderFile), matFile(matFile), meshFile(meshFile)
	, pass(0)
{
	shader = new Shader(Shaders + L"046_CsModel.fx", true);
}

GameModel::~GameModel()
{
	SAFE_RELEASE(boneBuffer);
	SAFE_RELEASE(boneSrv);

	SAFE_DELETE_ARRAY(boneTransforms);
	SAFE_DELETE_ARRAY(renderTransforms);

	SAFE_DELETE(model);
	SAFE_DELETE(shader);
}

void GameModel::Ready()
{
	model = new Model();
	model->ReadMaterial(matFile);
	model->ReadMesh(meshFile);
	SetShader(shaderFile);

	boneTransforms = new D3DXMATRIX[model->BoneCount()];
	renderTransforms = new D3DXMATRIX[model->BoneCount()];



	CsResource::CreateStructuredBuffer(sizeof(D3DXMATRIX), model->BoneCount(), NULL, &boneBuffer);
	CsResource::CreateSrv(boneBuffer, &boneSrv);

	UpdateWorld();

	model->SetBindingBox();
}

void GameModel::UpdateWorld()
{
	__super::UpdateWorld();


	D3DXMATRIX world;
	World(&world);

	for (Material* material : model->Materials())
		material->GetShader()->AsMatrix("World")->SetMatrix(world);
		
	UpdateTransforms();
	MappedBoneBuffer();

	UpdateVertex();


	for (CubeStruct* box : model->BindingBox())
	{
		box->meshCubeMinMax->Matrix(world);
	}
}

void GameModel::UpdateVertex()
{
	for (ModelMesh* mesh : model->Meshes())
	{
		VertexTextureNormalBlend* vertices = mesh->Vertices();

		
		ID3D11Buffer* vertexBuffer;
		ID3D11ShaderResourceView* vertexSrv;
		ID3D11UnorderedAccessView* outputUav;

		CsResource::CreateRawBuffer
		(
			sizeof(VertexTextureNormalBlend) * mesh->VertexCount(),
			vertices, &vertexBuffer
		);

		CsResource::CreateSrv(vertexBuffer, &vertexSrv);
		CsResource::CreateUav(mesh->VertexBuffer(), &outputUav);

		shader->AsScalar("BoneIndex")->SetInt(mesh->ParentBoneIndex());
		shader->AsShaderResource("Vertex")->SetResource(vertexSrv);
		shader->AsUAV("Output")->SetUnorderedAccessView(outputUav);


		float x = ceilf((float)mesh->VertexCount() / 256.0f);
		shader->Dispatch(0, pass, (UINT)x, 1, 1);

		
		SAFE_RELEASE(vertexBuffer);
		SAFE_RELEASE(vertexSrv);
		SAFE_RELEASE(outputUav);
	}
}

void GameModel::UpdateTransforms()
{
	for (UINT i = 0; i < model->BoneCount(); i++)
	{
		ModelBone* bone = model->BoneByIndex(i);


		D3DXMATRIX transform;
		D3DXMATRIX parentTransform;

		int parentIndex = bone->ParentIndex();
		if (parentIndex < 0)
			D3DXMatrixIdentity(&parentTransform);
		else
			parentTransform = boneTransforms[parentIndex];


		D3DXMATRIX inv = bone->Global();

		boneTransforms[i] = parentTransform;
		renderTransforms[i] = inv * boneTransforms[i];
	}
}

void GameModel::MappedBoneBuffer()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(boneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, renderTransforms, sizeof(D3DXMATRIX) * model->BoneCount());
	}
	D3D::GetDC()->Unmap(boneBuffer, 0);

	shader->AsShaderResource("BoneBuffer")->SetResource(boneSrv);
}

void GameModel::Update()
{
	
}

void GameModel::Render()
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Render();

	for (CubeStruct* cube : model->BindingBox())
	{
		cube->meshCubeMinMax->Render();
	}
}

Model * GameModel::GetModel()
{
	return model;
}

void GameModel::SetShader(wstring shaderFile)
{
	for (Material* material : model->Materials())
		material->SetShader(shaderFile);
}

void GameModel::SetDiffuse(float r, float g, float b, float a)
{
	SetDiffuse(D3DXCOLOR(r, g, b, a));
}

void GameModel::SetDiffuse(D3DXCOLOR & color)
{
	for (Material* material : model->Materials())
		material->SetDiffuse(color);
}

void GameModel::SetDiffuseMap(wstring file)
{
	for (Material* material : model->Materials())
		material->SetDiffuseMap(file);
}

void GameModel::SetSpecular(float r, float g, float b, float a)
{
	SetSpecular(D3DXCOLOR(r, g, b, a));
}

void GameModel::SetSpecular(D3DXCOLOR & color)
{
	for (Material* material : model->Materials())
		material->SetSpecular(color);
}

void GameModel::SetSpecularMap(wstring file)
{
	for (Material* material : model->Materials())
		material->SetSpecularMap(file);
}

void GameModel::SetShininess(float val)
{
	for (Material* material : model->Materials())
		material->SetShininess(val);
}

void GameModel::SetNormalMap(wstring file)
{
	for (Material* material : model->Materials())
		material->SetNormalMap(file);
}