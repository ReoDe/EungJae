#include "stdafx.h"
#include "Missile.h"
#include "./Environment/Terrain.h"

Missile::Missile(Terrain * terrain, Material * material,
	GameModel * gameModel, D3DXVECTOR3 DirectionNormal, Texture* texture)
	: terrain(terrain), material(material), gameModel(gameModel)
	, bDeleteMissile(false), DirectionNormal(DirectionNormal)
{
	MissileCube = new MeshCube(NULL, 2, 2, 2, texture);

	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	MissileCube->Matrix(W);

	gameModel->Position(&MissilePosition); // �����ǰ��� ��������
	gameModel->Position(&StartPosition); // ó����
	
	//gameModel->Position(&MissileRotation); // ȸ����
}

Missile::~Missile()
{
	SAFE_DELETE(MissileCube);
}

void Missile::Update()
{
	MoveToMissile(); // �̵�

	DeleteMissile(); // ����


	// �̻����� �׶��� ���� �ѱ�� ����
	if (MissilePosition.x < 0 || MissilePosition.x > 256
		|| MissilePosition.z < 0 || MissilePosition.z > 256)
	{
		bDeleteMissile = true;
	}

}

void Missile::Render()
{
	MissileCube->Render();
}

void Missile::MoveToMissile()
{
	//MissileCube->Rotation(MissilePosition);

	MissilePosition.x += DirectionNormal.x * 50.f * Time::Delta();
	MissilePosition.z += DirectionNormal.z * 50.f * Time::Delta();
	MissilePosition.y = terrain->GetHeightY(MissilePosition) + 1;

	MissileCube->Position(MissilePosition);
}

void Missile::DeleteMissile()
{
	float MaxDist = 200;
	float Distance = 0;

	Distance = sqrtf((StartPosition.x - MissilePosition.x) * (StartPosition.x - MissilePosition.x)
		+ (StartPosition.y - MissilePosition.y) * (StartPosition.y - MissilePosition.y)
		+ (StartPosition.z - MissilePosition.z) * (StartPosition.z - MissilePosition.z));

	if (Distance > MaxDist)
	{
		bDeleteMissile = true;
	}
		
}
