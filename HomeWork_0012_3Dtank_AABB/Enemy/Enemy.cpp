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

	//�ʱⰪ 


	D3DXVECTOR3 position(0,0,0);
	position.x = Math::Random(100.f, 200.0f);
	position.z = Math::Random(100.f, 200.0f);
	position.y = terrain->GetHeightY(position);

	tankPosition = position;
	tankPositionDirection.x = Math::Random(0.0f, 250.0f);
	tankPositionDirection.z = Math::Random(0.0f, 250.0f);
	tankPositionDirection.y = terrain->GetHeightY(tankPositionDirection);

	EnemyTank->Position(position);

	// �̻��ϰ�
	MissileMaterial = new Material(Shaders + L"015_Mesh.fx");
	MissileMaterial->SetDiffuseMap(Textures + L"Red.png");




	// �ڽ� ũ�� ��Ȯ�� �غ���
	D3DXMATRIX Global;
	Global = EnemyTank->GetModel()->BoneByIndex(0)->Global();
	D3DXVECTOR3 BoxMax, BoxMin;
	BoxMax = EnemyTank->GetModel()->Max;
	BoxMin = EnemyTank->GetModel()->Min;



	D3DXVECTOR3 BoxSize = BoxMax - BoxMin; // ��Ʈ���� ���ϱ���

	D3DXVECTOR3 result;

	result.x = BoxSize.x * Global._11;
	result.y = BoxSize.y * Global._22;
	result.z = BoxSize.z * Global._33;


	// �ڽ�
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

	// EnemyTank��ũ
	{
		// �����̴���
		if (tankPosition.x - tankPositionDirection.x > 0.1f ||
			tankPosition.x - tankPositionDirection.x < -0.1f)
		{
			terrain->MovePoint(&tankPosition, &tankPositionDirection, tankSpeed, tankRotation);

			//Wheel(lFront, rFront, lBack, rBack);


			DirectionNormal.x = tankPositionDirection.x - tankPosition.x;
			DirectionNormal.z = tankPositionDirection.z - tankPosition.z;

			D3DXVec3Normalize(&DirectionNormal, &DirectionNormal); // �븻����ȭ


			isMove = true;

			CoolTime += 5;
		}
		else
		{
			isMove = false;


		}
		


		if (!isMove) // �ȿ����϶�
		{
			tankPositionDirection.x = Math::Random(0.0f, 250.0f);
			tankPositionDirection.z = Math::Random(0.0f, 250.0f);
			tankPositionDirection.y = terrain->GetHeightY(tankPositionDirection);
			// Ŭ������ ���ϴ� ������ �־��

			// �븻���� ���� ���������� �ٲ�



		}

		tankPosition.y = terrain->GetHeightY(tankPosition);

		EnemyTank->Rotation(tankRotation);
		EnemyTank->Position(tankPosition);


	}

	EnemyTank->Update();



	// �̻��� �����



	if (CoolTime > 1000) // ���ǹ� �����ϱ�
	{
		Missile* MyMissile = new Missile(terrain, MissileMaterial, EnemyTank, DirectionNormal, texture);
		MyMissiles.push_back(MyMissile);

		CoolTime = 0;
	}
	// �̻��� ������
	for (Missile* Missile : MyMissiles)
	{
		if (Missile->GetDelete())
		{
			Missile->GetMeshCube()->Position(0, 0, 0); // �ϴ��� �����������̷���
		}
		else
		{
			Missile->Update();
		}
	}


	// �ڽ� ��ġ ����
	EnemyBox->Position(tankPosition.x, terrain->GetHeightY(tankPosition) + 2, tankPosition.z);
	EnemyBox->Rotation(tankRotation);



	// �̻��� ����ó��
	iterMyMissiles = MyMissiles.begin();
	for (iterMyMissiles; iterMyMissiles != MyMissiles.end(); )
	{
		if ((*iterMyMissiles)->GetDelete()) // ���� �����̸�
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

	// �ڽ� ����
	EnemyBox->Render();
	
}
