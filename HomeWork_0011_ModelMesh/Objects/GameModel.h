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

	void SRT(wstring name);
public:
	void UpdateWorld();

private:
	Shader* shader;
	Model* model;

	vector<D3DXMATRIX> transforms;

	// ����

	bool BindingBoxVisible;
	RasterizerState* rState[2];


	D3DXVECTOR3 scale, rotation, position;

private:
	class Buffer : public ShaderBuffer
	{
	public:
		Buffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Index = 0;
		}

		struct Struct
		{
			int Index;
			float Padding[3];
		} Data;
	};

	Buffer* buffer;
};