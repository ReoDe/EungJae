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

	D3DXVECTOR3 position; // ���콺 ������

	Shader* shader;
	GameModel* MyTank;
	D3DXVECTOR2 MyPos; // ��ũ ID ��ġ

	D3DXVECTOR3 tankPosition;
	D3DXVECTOR3 tankPositionDirection;
	D3DXVECTOR3 tankRotation;
	float tankSpeed;
	int MyHP;

	// �̻��� �����

	Material* MissileMaterial;
	vector<class Missile*> MyMissiles;
	vector<class Missile*>::iterator iterMyMissiles;
	int MissileSize;



	// ���� �� ����

	vector<class Enemy*> EnemyTanks;
	vector<class Enemy*>::iterator iterEnemyTanks;
	bool asdf = false;

	// �浹�� ���� �� ��ũ ���ε��ڽ�
	Material* MyCubeMaterial;
	MeshCube* MyCube;
	//Texture* texture;

	// �̻��� ���� ���������� �븻����
	D3DXVECTOR3 DirectionNormal;


};