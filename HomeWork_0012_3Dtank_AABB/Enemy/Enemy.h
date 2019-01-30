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

	// 미사일

	Material* MissileMaterial;
	vector<class Missile*> MyMissiles;
	vector<class Missile*>::iterator iterMyMissiles;
	// 상태
	bool isDeath;

	// 바인딩박스
	class MeshCube* EnemyBox;
	class Texture* texture;
	

	// 방향 노말
	D3DXVECTOR3 DirectionNormal;

	// 미사일 쿨타임
	int CoolTime;

};