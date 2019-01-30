#pragma once
#include "GameRender.h"
#include "../Model/Model.h"
#include "../Model/ModelBone.h"
#include "../Model/ModelMesh.h"

class GameModel : public GameRender
{
public:
	GameModel(Shader* shader, wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile);
	virtual ~GameModel();

	void Update();
	void Render();

	Model* GetModel();

public:
	void UpdateWorld();

private:
	Shader* shader;
	Model* model;

	vector<D3DXMATRIX> transforms;



};