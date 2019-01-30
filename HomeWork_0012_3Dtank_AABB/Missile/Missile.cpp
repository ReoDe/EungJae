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

	gameModel->Position(&MissilePosition); // 포지션값을 가져오고
	gameModel->Position(&StartPosition); // 처음값
	
	//gameModel->Position(&MissileRotation); // 회전값
}

Missile::~Missile()
{
	SAFE_DELETE(MissileCube);
}

void Missile::Update()
{
	MoveToMissile(); // 이동

	DeleteMissile(); // 삭제


	// 미사일이 테라인 범위 넘기면 삭제
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
