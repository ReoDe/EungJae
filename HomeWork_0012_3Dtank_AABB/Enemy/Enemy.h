#pragma once

class Enemy
{
public:
	Enemy(class Terrain* terrain, class Shader* shader);
	~Enemy();

	void Update();
	void Render();
	GameModel* GetTank() { return EnemyTank; }
	void SetisDeath(bool isdeath) 
	{
		isDeath = isdeath;
	}
	bool GetisDeath()
	{
		return isDeath;
	}

	D3DXVECTOR3 GetTankPos()
	{
		return tankPosition;
	}

	MeshCube* GetTankBox()
	{
		return EnemyBox;
	}

	int GetCoolTime()
	{
		return CoolTime;
	}
	vector<class Missile*> GetMissiles()
	{
		return MyMissiles;
	}

private:
	class Terrain* terrain;
	class Shader* shader;
	GameModel * EnemyTank;
	
	D3DXVECTOR3 tankPosition;
	D3DXVECTOR3 tankPositionDirection;
	D3DXVECTOR3 tankRotation;
	float tankSpeed;

	bool isMove;

	// �̻���

	Material* MissileMaterial;
	vector<class Missile*> MyMissiles;
	vector<class Missile*>::iterator iterMyMissiles;
	// ����
	bool isDeath;

	// ���ε��ڽ�
	class MeshCube* EnemyBox;
	class Texture* texture;
	

	// ���� �븻
	D3DXVECTOR3 DirectionNormal;

	// �̻��� ��Ÿ��
	int CoolTime;

};