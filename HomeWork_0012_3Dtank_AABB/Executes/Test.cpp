#include "stdafx.h"
#include "Test.h"
#include "./Environment/Terrain.h"

#include "./Enemy/Enemy.h"
#include "./Missile/Missile.h"
#include "./Boundings/Collision.h"

Test::Test()
	:tankPosition(0, 0, 0), tankPositionDirection(0, 0, 0)
	, tankRotation(0, 0, 0), tankSpeed(25.0f), MyHP(3), MissileSize(0)
	, DirectionNormal(0,0,0)
{
	
	terrainMaterial = new Material(Shaders + L"016_TerrainBrush.fx");

	terrainMaterial->SetDiffuseMap(Textures + L"Dirt.png");
	
	terrain = new Terrain(terrainMaterial, Textures + L"HeightMap256.png");


	position = D3DXVECTOR3(0, 0, 0);

	//

	shader = new Shader( Shaders + L"023_Lighting.fx");

	MyTank = new GameModel
	(
		shader,
		Models + L"Tank/", L"Tank.material",
		Models + L"Tank/", L"Tank.mesh"
	);


	// �� ��ũ ����

	

	for (int i = 0; i < 4; i++)
	{
		Enemy* EnemyTank = new Enemy(terrain, shader);
		EnemyTanks.push_back(EnemyTank);
	}

	// �̻���
	MissileMaterial = new Material(Shaders + L"015_Mesh.fx");
	MissileMaterial->SetDiffuseMap(Textures + L"Yellow.png");

	// �浹
	texture = new Texture(Textures + L"Yellow.png");
	MyCube = new MeshCube(NULL, 3, 3, 3, texture);




}

Test::~Test()
{
	//SAFE_DELETE(terrain);
	if (MyTank != NULL)
		SAFE_DELETE(MyTank);

	for (Enemy* model : EnemyTanks)
		SAFE_DELETE(model);

}

void Test::Update()
{
	D3DXVECTOR3 position;
	if (terrain->GetHeight(position) == true)
		this->position = position;

	terrain->Update(); // ���� ����

	// My��ũ
	{
		// �����̴���
		if (tankPosition.x - tankPositionDirection.x > 0.1f ||
			tankPosition.x - tankPositionDirection.x < -0.1f)
		{
			terrain->MovePoint(&tankPosition, &tankPositionDirection, tankSpeed, tankRotation);

			//Wheel(lFront, rFront, lBack, rBack);

		}


		if (Mouse::Get()->Press(0))
		{
			tankPositionDirection = terrain->GetPickingMouseY();
			// Ŭ������ ���ϴ� ������ �־��

			// �븻���� ���� ���������� �ٲ�

			DirectionNormal.x = tankPositionDirection.x - tankPosition.x;
			DirectionNormal.z = tankPositionDirection.z - tankPosition.z;

			D3DXVec3Normalize(&DirectionNormal, &DirectionNormal); // �븻����ȭ, �̰����� �̻��Ϲ��⼳��
		}

		tankPosition.y = terrain->GetHeightY(tankPosition);

		MyTank->Rotation(tankRotation);
		MyTank->Position(tankPosition);


		MyTank->Update();




		// �̻��� �����
		if (Keyboard::Get()->Down(VK_SPACE))
		{
									// �浹�ʵ�, �̻��ϸ��͸���(���� �Ⱦ�),�� ��ũ, ��ũ����븻����, �ؽ���
			Missile* MyMissile = new Missile(terrain, MissileMaterial, MyTank, DirectionNormal, texture);
			MyMissiles.push_back(MyMissile);

		}
		// �̻��� ������
		for (Missile* Missile : MyMissiles)
		{
			
			if (Missile->GetDelete())
			{
				// ���� �̰� �Ⱦ�, ������ �� // �״� ó���� bool������ ����
				Missile->GetMeshCube()->Position(0, 0, 0); // �ϴ��� �����������̷���
			}
			else
			{
				Missile->Update();
			}




		}
		
		

	}
	


	// �浹ó�� ( �� ��ũ�� �� �̻���) // ���߿� �浹ó�� Ŭ���� �ϳ� ������
	{
		for (Enemy* model : EnemyTanks)
		{
			for (Missile* missile : MyMissiles)
			{

				if (Collision::Intersect
				(
					model->GetTankBox(),
					missile->GetMeshCube()
				))
				{
					missile->SetDelete(true);
					model->SetisDeath(true);

				}
				
				
			}
		}

	}

	// �� �̻��ϰ� �� ��ũ�� �浹ó��
	for (Enemy* model : EnemyTanks)
	{
		for (Missile* missile : model->GetMissiles())
		{
			if (Collision::Intersect
			(
				MyCube,
				missile->GetMeshCube()
			))
			{
				missile->SetDelete(true);
				MyHP -= 1;
			}

		}
	}




	// �� ��ũ������

	for (Enemy* model : EnemyTanks)
	{
		if (!model->GetisDeath())
			model->Update();
		else
		{
			D3DXVECTOR3 DeathPos(0, 0, 0);
			model->GetTank()->Position(DeathPos);
		}
	}


	// �� ��ũ ���ε�
	MyCube->Position(tankPosition);
	MyCube->Rotation(tankRotation);


	// ���� ��ũ����ó��
	iterEnemyTanks = EnemyTanks.begin();
	for (iterEnemyTanks; iterEnemyTanks != EnemyTanks.end(); )
	{
		if ((*iterEnemyTanks)->GetisDeath()) // ���� �׾�����
		{
			iterEnemyTanks = EnemyTanks.erase(iterEnemyTanks);
		}
		else iterEnemyTanks++;
	}
	
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

void Test::Render()
{
	ImGui::SliderInt("HP", &MyHP, 0, 3);
	MissileSize = MyMissiles.size();
	ImGui::SliderInt("MissileSize", &MissileSize, 0, 100);
	ImGui::Separator();
	ImGui::SliderFloat3("MyTank Pos", (float*)&tankPosition, 0, 256);
	ImGui::Separator();
	for (int i = 0; i < EnemyTanks.size(); i++)
	{
		ImGui::SliderFloat3("Emeny Pos" - i, (float*)&EnemyTanks.at(i)->GetTankPos(), 0, 256);
	}

	ImGui::Separator();

	
	
	if (MyMissiles.size() > 0)
	{
		// �̻��� ��ġ �ޱ�
		for (int i = 0; i < MyMissiles.size(); i++)
		{
			ImGui::SliderFloat3("Missile Pos" - i, (float*)&MyMissiles.at(i)->GetMissilePos(), 0, 256);
		}
		
	}
	// test
	if (EnemyTanks.size() > 0)
	{
		int cooltime = EnemyTanks.at(0)->GetCoolTime();
		ImGui::SliderInt("Cooltime", &cooltime, 0, 1000);

		int EnemyMissile = EnemyTanks.at(0)->GetMissiles().size();
		ImGui::SliderInt("EnemyMissile", &EnemyMissile, 0, 100);
	}


	terrain->Render();

	MyTank->Render(); //?

	for (Enemy* model : EnemyTanks)
		model->Render();

	//�̻���
	for (Missile* Missile : MyMissiles)
	{
		Missile->Render();
	}
	

	//��ũ ���ε��ڽ�

	MyCube->Render();


}

