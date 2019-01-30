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


	// 적 탱크 생성

	

	for (int i = 0; i < 4; i++)
	{
		Enemy* EnemyTank = new Enemy(terrain, shader);
		EnemyTanks.push_back(EnemyTank);
	}

	// 미사일
	MissileMaterial = new Material(Shaders + L"015_Mesh.fx");
	MissileMaterial->SetDiffuseMap(Textures + L"Yellow.png");

	// 충돌
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

	terrain->Update(); // 지형 생성

	// My탱크
	{
		// 움직이는중
		if (tankPosition.x - tankPositionDirection.x > 0.1f ||
			tankPosition.x - tankPositionDirection.x < -0.1f)
		{
			terrain->MovePoint(&tankPosition, &tankPositionDirection, tankSpeed, tankRotation);

			//Wheel(lFront, rFront, lBack, rBack);

		}


		if (Mouse::Get()->Press(0))
		{
			tankPositionDirection = terrain->GetPickingMouseY();
			// 클릭값을 원하는 방향이 넣어놈

			// 노말벡터 방향 꺾을때마다 바뀜

			DirectionNormal.x = tankPositionDirection.x - tankPosition.x;
			DirectionNormal.z = tankPositionDirection.z - tankPosition.z;

			D3DXVec3Normalize(&DirectionNormal, &DirectionNormal); // 노말벡터화, 이것으로 미사일방향설정
		}

		tankPosition.y = terrain->GetHeightY(tankPosition);

		MyTank->Rotation(tankRotation);
		MyTank->Position(tankPosition);


		MyTank->Update();




		// 미사일 만들기
		if (Keyboard::Get()->Down(VK_SPACE))
		{
									// 충돌필드, 미사일매터리얼(현재 안씀),내 탱크, 탱크정면노말벡터, 텍스쳐
			Missile* MyMissile = new Missile(terrain, MissileMaterial, MyTank, DirectionNormal, texture);
			MyMissiles.push_back(MyMissile);

		}
		// 미사일 움직임
		for (Missile* Missile : MyMissiles)
		{
			
			if (Missile->GetDelete())
			{
				// 이제 이거 안씀, 지워도 됨 // 죽는 처리는 bool값으로 결정
				Missile->GetMeshCube()->Position(0, 0, 0); // 일단은 지우지말고이렇게
			}
			else
			{
				Missile->Update();
			}




		}
		
		

	}
	


	// 충돌처리 ( 적 탱크와 내 미사일) // 나중에 충돌처리 클래스 하나 만들자
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

	// 적 미사일과 내 탱크의 충돌처리
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




	// 적 탱크움직임

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


	// 내 탱크 바인딩
	MyCube->Position(tankPosition);
	MyCube->Rotation(tankRotation);


	// 죽은 탱크삭제처리
	iterEnemyTanks = EnemyTanks.begin();
	for (iterEnemyTanks; iterEnemyTanks != EnemyTanks.end(); )
	{
		if ((*iterEnemyTanks)->GetisDeath()) // 만약 죽었으면
		{
			iterEnemyTanks = EnemyTanks.erase(iterEnemyTanks);
		}
		else iterEnemyTanks++;
	}
	
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
		// 미사일 위치 받기
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

	//미사일
	for (Missile* Missile : MyMissiles)
	{
		Missile->Render();
	}
	

	//탱크 바인딩박스

	MyCube->Render();


}

