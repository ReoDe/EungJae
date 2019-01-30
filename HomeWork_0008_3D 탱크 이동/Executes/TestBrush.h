#pragma once

class TestBrush : public Execute
{
public:
	TestBrush(ExecuteValues* values);
	~TestBrush();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}
public:
	void Wheel(D3DXMATRIX& lFront, D3DXMATRIX& rFront, D3DXMATRIX& lBack, D3DXMATRIX& rBack);
private:
	Material * terrainMaterial;
	class Terrain* terrain;

	D3DXVECTOR3 position;

	Shader* shader;
	class GameModel* tank;

	D3DXVECTOR3 tankPosition;
	D3DXVECTOR3 tankPositionDirection;
	D3DXVECTOR3 tankRotation;
	D3DXVECTOR3 tankRotationtest;
	// 

	float tankSpeed;

	//로컬 가져오기

	D3DXMATRIX lFront, rFront, lBack, rBack;
};