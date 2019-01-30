#pragma once
#include "GameRender.h"
#include "../Model/Model.h"
#include "../Model/ModelBone.h"
#include "../Model/ModelMesh.h"

class GameModel : public GameRender
{
public:
	GameModel(wstring shaderFile, wstring matFile, wstring meshFile);
	virtual ~GameModel();

	void Ready();

	virtual void Update();
	virtual void Render();

	Model* GetModel();

	void SetShader(wstring shaderFile);

	void SetDiffuse(float r, float g, float b, float a = 1.0f);
	void SetDiffuse(D3DXCOLOR& color);
	void SetDiffuseMap(wstring file);

	void SetSpecular(float r, float g, float b, float a = 1.0f);
	void SetSpecular(D3DXCOLOR& color);
	void SetSpecularMap(wstring file);
	void SetShininess(float val);

	void SetNormalMap(wstring file);

public:
	virtual void UpdateWorld() override;
	void UpdateVertex();

	virtual void UpdateTransforms();
	void MappedBoneBuffer();

protected:
	wstring shaderFile;
	wstring matFile;
	wstring meshFile;

	Shader* shader;
	UINT pass;

	class Model* model;

	D3DXMATRIX* boneTransforms;
	D3DXMATRIX* renderTransforms;

	ID3D11Buffer* boneBuffer;
	ID3D11ShaderResourceView* boneSrv;



};