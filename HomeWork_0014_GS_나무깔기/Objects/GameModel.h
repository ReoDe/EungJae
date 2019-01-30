#pragma once
#include "GameRender.h"
#include "../Model/Model.h"
#include "../Model/ModelBone.h"
#include "../Model/ModelMesh.h"

class GameModel : public GameRender
{
public:
	GameModel(wstring shaderFile, wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile);
	virtual ~GameModel();

	void Update();
	void Render();

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
	void UpdateWorld();

private:
	Model* model;

	vector<D3DXMATRIX> transforms;
};