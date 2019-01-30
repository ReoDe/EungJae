#pragma once

class Test2 : public Execute
{
public:
	Test2();
	~Test2();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override;

	void ResizeScreen() override {}

private:

	class Sky* sky;

	//


	Material * terrainMaterial;
	class Terrain* terrain;

	D3DXVECTOR3 position; // 마우스 포지션

	Shader* shader;
	GameModel* MyTank;
	D3DXVECTOR2 MyPos; // 탱크 ID 위치

	D3DXVECTOR3 tankPosition;
	D3DXVECTOR3 tankPositionDirection;
	D3DXVECTOR3 tankRotation;
	float tankSpeed;
	int MyHP;

	// 미사일 만들기

	Material* MissileMaterial;
	vector<class Missile*> MyMissiles;
	vector<class Missile*>::iterator iterMyMissiles;
	int MissileSize;



	// 이제 적 생성

	vector<class Enemy*> EnemyTanks;
	vector<class Enemy*>::iterator iterEnemyTanks;
	bool asdf = false;

	// 충돌을 위한 내 탱크 바인딩박스
	Material* MyCubeMaterial;
	MeshCube* MyCube;
	//Texture* texture;

	// 미사일 방향 설정을위한 노말벡터
	D3DXVECTOR3 DirectionNormal;


};