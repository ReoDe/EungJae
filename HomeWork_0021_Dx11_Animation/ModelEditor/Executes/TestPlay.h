#pragma once
#include "Systems/IExecute.h"

class TestPlay : public IExecute
{
public:
	void Initialize() override;
	void Ready() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override;

	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	// 지형
	Material * terrainMaterial;
	class Terrain* terrain;

	// 플레이어
	class Player * player;

	// 적군
	class Enemy* enemy1;

private:
	void PlayerToEnemy();
	void PlayerPunchToEnemy();

	void PlayerToEnemyOBB();
	void PlayerPunchToEnemyOBB();

	void ClearCheckBox();
private:
	///////////////////////////////////
	// 충돌체크
	///////////////////////////////////

	// AABB

	bool globalCheck;


	bool localCheck;
	bool preLocalCheck;

	
private:
	struct ST_OBB // OBB구조체
	{
		D3DXVECTOR3 centerPos; // 상자 중앙의 좌표
		D3DXVECTOR3 axis[3]; //상자에 평행한 세 축의 단위벡터
		float  axisLen[3]; // 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.
	};

	void SetOBB(OUT ST_OBB& Box, D3DXVECTOR3 & position, D3DXMATRIX & world, D3DXVECTOR3 & length);

	bool CheckOBB(ST_OBB& Box1, ST_OBB& Box2);

	

	ST_OBB OBB1; // 플레이어 바디
	ST_OBB OBB2; // 몬스터 바디

	ST_OBB Punch1; // 왼손
	ST_OBB Punch2; // 오른손

	bool bCheckOBB = false;

	bool bPunch1 = false;
	bool bPunch2 = false;

	bool prebPunch1 = false;
	bool prebPunch2 = false;

};