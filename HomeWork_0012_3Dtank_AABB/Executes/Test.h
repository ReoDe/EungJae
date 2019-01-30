#pragma once

class Enemy;
class Missile;

class Test : public Execute
{
public:
	Test();
	~Test();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	Material * terrainMaterial;
	class Terrain* terrain;

	D3DXVECTOR3 position; // ���콺 ������

	Shader* shader;
	GameModel* MyTank;

	D3DXVECTOR3 tankPosition;
	D3DXVECTOR3 tankPositionDirection;
	D3DXVECTOR3 tankRotation;
	float tankSpeed;
	int MyHP;
	
	// �̻��� �����

	Material* MissileMaterial;
	vector<Missile*> MyMissiles;
	vector<Missile*>::iterator iterMyMissiles;
	int MissileSize;



	// ���� �� ����

	vector<Enemy*> EnemyTanks;
	vector<Enemy*>::iterator iterEnemyTanks;
	bool asdf = false;

	// �浹�� ���� �� ��ũ ���ε��ڽ�
	MeshCube* MyCube;
	Texture* texture;
	
	// �̻��� ���� ���������� �븻����
	D3DXVECTOR3 DirectionNormal;
};