#pragma once
#include "GameRender.h"

class GameAnimator : public GameRender
{
public:
	GameAnimator(wstring shaderFile, wstring matFile, wstring meshFile);
	~GameAnimator();

	void AddClip(wstring clipFile);

	void Update();
	void Render();

private:
	void UpdateBone();

private:
	class Model* model;

	UINT currentClip;
	int currentKeyframe;
	int nextKeyframe;

	float frameTime;
	float frameFactor;

	vector<D3DXMATRIX> boneTransforms;
	vector<D3DXMATRIX> renderTransforms;

	vector<class ModelClip *> clips;
};