#pragma once

class TestAnimation : public Execute
{
public:
	TestAnimation();
	~TestAnimation();

	void Update() override;
	
	void PreRender() override;
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	GameAnimator * gameModel;

	enum state
	{
		STATE_Idle,
		STATE_Running
	};
	state charactorState;

	Material * terrainMaterial;
	class Terrain* terrain;

	//
	D3DXVECTOR3 position; // 마우스 포지션

	D3DXVECTOR3 charactorPosition;
	D3DXVECTOR3 charactorPositionDirection;
	D3DXVECTOR3 charactorRotation;
	float charactorSpeed;
	D3DXVECTOR3 charactorScale;

};