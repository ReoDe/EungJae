#include "stdafx.h"
#include "Enemy.h"
#include "./Environment/Terrain.h"
#include "./Missile/Missile.h"

Enemy::Enemy(Terrain* terrain, Shader* shader)
	: terrain(terrain), shader(shader)
	, tankPosition(0, 0, 0), tankPositionDirection(0, 0, 0)
	, tankRotation(0, 0, 0), tankSpeed(10.0f), isMove(false)
	, isDeath(false), CoolTime(0)
{
	EnemyTank = new GameModel
	(
		shader,
		Models + L"Tank/", L"Tank.material",
		Models + L"Tank/", L"Tank.mesh"
	);

	//초기값 


	D3DXVECTOR3 position(0,0,0);
	position.x = Math::Random(100.f, 200.0f);
	position.z = Math::Random(100.f, 200.0f);
	position.y = terrain->GetHeightY(position);

	tankPosition = position;
	tankPositionDirection.x = Math::Random(0.0f, 250.0f);
	tankPositionDirection.z = Math::Random(0.0f, 250.0f);
	tankPositionDirection.y = terrain->GetHeightY(tankPositionDirection);

	EnemyTank->Position(position);

	// 미사일값
	MissileMaterial = new Material(Shaders + L"015_Mesh.fx");
	MissileMaterial->SetDiffuseMap(Textures + L"Red.png");




	// 박스 크기 정확히 해보자
	D3DXMATRIX Global;
	Global = EnemyTank->GetModel()->BoneByIndex(0)->Global();
	D3DXVECTOR3 BoxMax, BoxMin;
	BoxMax = EnemyTank->GetModel()->Max;
	BoxMin = EnemyTank->GetModel()->Min;



	D3DXVECTOR3 BoxSize = BoxMax - BoxMin; // 매트릭스 곱하기전

	D3DXVECTOR3 result;

	result.x = BoxSize.x * Global._11;
	result.y = BoxSize.y * Global._22;
	result.z = BoxSize.z * Global._33;


	// 박스
	texture = new Texture(Textures + L"Red.png");
	EnemyBox = new MeshCube(NULL, result.x, result.y, result.z, texture);

	
}

Enemy::~Enemy()
{
	//SAFE_DELETE(terrain);
	SAFE_DELETE(EnemyTank);
}

void Enemy::Update()
{

	// EnemyTank탱크
	{
		// 움직이는중
		if (tankPosition.x - tankPositionDirection.x > 0.1f ||
			tankPosition.x - tankPositionDirection.x < -0.1f)
		{
			terrain->MovePoint(&tankPosition, &tankPositionDirection, tankSpeed, tankRotation);

			//Wheel(lFront, rFront, lBack, rBack);


			DirectionNormal.x = tankPositionDirection.x - tankPosition.x;
			DirectionNormal.z = tankPositionDirection.z - tankPosition.z;

			D3DXVec3Normalize(&DirectionNormal, &DirectionNormal); // 노말벡터화


			isMove = true;

			CoolTime += 5;
		}
		else
		{
			isMove = false;


		}
		


		if (!isMove) // 안움직일때
		{
			tankPositionDirection.x = Math::Random(0.0f, 250.0f);
			tankPositionDirection.z = Math::Random(0.0f, 250.0f);
			tankPositionDirection.y = terrain->GetHeightY(tankPositionDirection);
			// 클릭값을 원하는 방향이 넣어놈

			// 노말벡터 방향 꺾을때마다 바뀜



		}

		tankPosition.y = terrain->GetHeightY(tankPosition);

		EnemyTank->Rotation(tankRotation);
		EnemyTank->Position(tankPosition);


	}

	EnemyTank->Update();



	// 미사일 만들기



	if (CoolTime > 1000) // 조건문 수정하기
	{
		Missile* MyMissile = new Missile(terrain, MissileMaterial, EnemyTank, DirectionNormal, texture);
		MyMissiles.push_back(MyMissile);

		CoolTime = 0;
	}
	// 미사일 움직임
	for (Missile* Missile : MyMissiles)
	{
		if (Missile->GetDelete())
		{
			Missile->GetMeshCube()->Position(0, 0, 0); // 일단은 지우지말고이렇게
		}
		else
		{
			Missile->Update();
		}
	}


	// 박스 위치 조정
	EnemyBox->Position(tankPosition.x, terrain->GetHeightY(tankPosition) + 2, tankPosition.z);
	EnemyBox->Rotation(tankRotation);



	// 미사일 삭제처리
	iterMyMissiles = MyMissiles.begin();
	for (iterMyMissiles; iterMyMissiles != MyMissiles.end(); )
	{
		if ((*iterMyMissiles)->GetDelete()) // 만약 원점이면
		{
			iterMyMissiles = MyMissiles.erase(iterMyMissiles);
		}
		else iterMyMissiles++;
	}


}

void Enemy::Render()
{

	EnemyTank->Render();

	for (Missile* Missile : MyMissiles)
	{
		Missile->Render();
	}

	// 박스 조정
	EnemyBox->Render();
	
}
