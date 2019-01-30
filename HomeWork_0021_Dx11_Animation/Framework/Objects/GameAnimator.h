#pragma once
#include "GameRender.h"

struct LocalBindingBox
{
	MeshCube* cube;
	int Index;
	bool enable;
	wstring boneName;
	D3DXMATRIX World;

	D3DXVECTOR3 Rotation;
};

class GameAnimator : public GameModel
{
public:
	GameAnimator(wstring shaderFile, wstring matFile, wstring meshFile);
	~GameAnimator();

	void AddClip(wstring clipFile);
	void ClipNumber(UINT index);
	UINT GetClipNumber() { return currentClip; }

	void Update();
	void Render();


	// 클립 정보

	int GetKeyframe() { return currentKeyframe; }
	void SetKeyFrame(int val) { currentKeyframe = val; }

	int GetMaxFrameCount() { return MaxFrameCount - 1; }



	// 지역 바인딩 박스 만들기
	vector<LocalBindingBox*>& GetMeshCube() { return Cubes; }

	void SetMeshCube(Material* mat, float width, float height, float depth, int index)
	{
		LocalBindingBox* localBindingBox = new LocalBindingBox();

		MeshCube* cube = new MeshCube(mat, width, height, depth);

		localBindingBox->cube = cube;
		localBindingBox->Index = index;
		localBindingBox->enable = true;
		localBindingBox->boneName = model->BoneByIndex(index)->Name();

		Cubes.push_back(localBindingBox);
	}


private:
	void UpdateWorld() override;
	void UpdateTransforms() override;

private:

	UINT currentClip;
	int currentKeyframe;
	int nextKeyframe;

	float frameTime;
	float frameFactor;

	vector<class ModelClip *> clips;

	//

	int MaxFrameCount;

	vector<LocalBindingBox*> Cubes;
};