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
	// ����
	Material * terrainMaterial;
	class Terrain* terrain;

	// �÷��̾�
	class Player * player;

	// ����
	class Enemy* enemy1;

private:
	void PlayerToEnemy();
	void PlayerPunchToEnemy();

	void PlayerToEnemyOBB();
	void PlayerPunchToEnemyOBB();

	void ClearCheckBox();
private:
	///////////////////////////////////
	// �浹üũ
	///////////////////////////////////

	// AABB

	bool globalCheck;


	bool localCheck;
	bool preLocalCheck;

	
private:
	struct ST_OBB // OBB����ü
	{
		D3DXVECTOR3 centerPos; // ���� �߾��� ��ǥ
		D3DXVECTOR3 axis[3]; //���ڿ� ������ �� ���� ��������
		float  axisLen[3]; // ������ ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	};

	void SetOBB(OUT ST_OBB& Box, D3DXVECTOR3 & position, D3DXMATRIX & world, D3DXVECTOR3 & length);

	bool CheckOBB(ST_OBB& Box1, ST_OBB& Box2);

	

	ST_OBB OBB1; // �÷��̾� �ٵ�
	ST_OBB OBB2; // ���� �ٵ�

	ST_OBB Punch1; // �޼�
	ST_OBB Punch2; // ������

	bool bCheckOBB = false;

	bool bPunch1 = false;
	bool bPunch2 = false;

	bool prebPunch1 = false;
	bool prebPunch2 = false;

};