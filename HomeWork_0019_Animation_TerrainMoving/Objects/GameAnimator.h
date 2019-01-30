#pragma once
#include "GameRender.h"

class GameAnimator : public GameRender
{
public:

	GameAnimator(wstring shaderFile, wstring matFile, wstring meshFile);
	virtual ~GameAnimator();

	void AddClip(wstring clipFile);
	void ClipNumber(UINT index);

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

	UINT maxCount;

	vector<D3DXMATRIX> boneTransforms;

	// ½¦ÀÌ´õ·Î ³Ñ¾î°¥ ³ðµé
	vector<D3DXMATRIX> renderTransforms;

	vector<class ModelClip*> clips;

	//
	D3DXVECTOR3 scale;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;


};