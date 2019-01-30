#include "stdafx.h"
#include "TestPlay.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"


void TestPlay::Initialize()
{
	globalCheck = false;
	localCheck = false;

}

void TestPlay::Ready() // gpu복사
{
	// 지형
	terrainMaterial = new Material(Shaders + L"028_Terrain.fx");
	terrainMaterial->SetDiffuseMap(Textures + L"Dirt2.png");
	terrain = new Terrain(terrainMaterial, Textures + L"HeightMap256.png");

	// 플레이어
	player = new class Player(terrain);
	player->Initialize();
	player->Ready();

	// 적군
	enemy1 = new Enemy(terrain);
	enemy1->setplayerAddressLink(player); // 링크
	enemy1->Initialize();
	enemy1->Ready();



}

void TestPlay::Destroy()
{
	SAFE_DELETE(terrain);


	player->Destroy();

	enemy1->Destroy();

}

void TestPlay::Update()
{
	// 지형
	terrain->Update();

	player->Update();

	
	enemy1->Update();


	// 충돌처리

	//////////////////////////////
	// AABB
	//////////////////////////////
	
	PlayerToEnemy();
	PlayerPunchToEnemy();

	//////////////////////////////
	// OBB
	//////////////////////////////

	PlayerToEnemyOBB();
	PlayerPunchToEnemyOBB();
	


}

void TestPlay::PreRender()
{
	player->PreRender();
	
	enemy1->PreRender();

	ImGui::End();

	ImGui::Begin("AABB Collision");
	ImGui::Checkbox("PlayerToEnemy", &globalCheck);
	ImGui::Checkbox("PlayerPunchToEnemy", &localCheck);
	ImGui::End();


	ImGui::Begin("OBB Collision");
	ImGui::Checkbox("CheckOBB", &bCheckOBB);
	ImGui::Checkbox("bPunch1", &bPunch1);
	ImGui::Checkbox("bPunch2", &bPunch2);
}

void TestPlay::Render()
{

	terrain->Render();

	player->Render();

	enemy1->Render();


}

void TestPlay::PlayerToEnemy()
{
	globalCheck = Collision::IntersectAABB(player->GetBindingBox(), enemy1->GetBindingBox());
}

void TestPlay::PlayerPunchToEnemy()
{


	for (int i = 0; i < player->GetBox().size(); i++)
	{
		MeshCube* cube = player->GetBox().at(i)->collisionBox;
		D3DXMATRIX mat = player->GetModel()->GetMeshCube().at(i)->World;

		bool enable = player->GetBox().at(i)->enable;

		cube->Matrix(mat);


		if (enable == true)
			localCheck = Collision::IntersectAABB(cube, enemy1->GetBindingBox(), mat);
		

		if (localCheck == true && preLocalCheck == false)
			// 몬스터 함수 콜
		{
			enemy1->Hitted();

		}

		preLocalCheck = localCheck;


	}
}

void TestPlay::PlayerToEnemyOBB()
{
	// 플레이어 메인 사각형
	{
		D3DXVECTOR3 pos;
		player->GetBindingBox()->Position(&pos);

		D3DXMATRIX mat;
		player->GetBindingBox()->Matrix(&mat);

		D3DXVECTOR3 length, Max, Min;
		Max = player->GetBindingBox()->GetMax();
		Min = player->GetBindingBox()->GetMin();

		length = (Max - Min) / 2;

		SetOBB(OBB1, pos, mat, length);
	}

	// 몬스터 메인 사각형
	{
		D3DXVECTOR3 pos;
		enemy1->GetBindingBox()->Position(&pos);

		D3DXMATRIX mat;
		enemy1->GetBindingBox()->Matrix(&mat);

		D3DXVECTOR3 length, Max, Min;
		Max = player->GetBindingBox()->GetMax();
		Min = player->GetBindingBox()->GetMin();

		length = (Max - Min) / 2;

		SetOBB(OBB2, pos, mat, length);
	}

	bCheckOBB = CheckOBB(OBB1, OBB2);
}

void TestPlay::PlayerPunchToEnemyOBB()
{
	// 플레이어 주먹 1
	{
		D3DXMATRIX mat = player->GetModel()->GetMeshCube().at(0)->World;

		D3DXVECTOR3 pos;
		pos.x = mat._41;
		pos.y = mat._42;
		pos.z = mat._43;

		D3DXVECTOR3 length, Max, Min;
		Max = player->GetBox().at(0)->collisionBox->GetMax();
		Min = player->GetBox().at(0)->collisionBox->GetMin();

		D3DXVec3TransformCoord(&Max, &Max, &mat);
		D3DXVec3TransformCoord(&Min, &Min, &mat);

		length.x = abs((Max.x - Min.x) / 2);
		length.y = abs((Max.y - Min.y) / 2);
		length.z = abs((Max.z - Min.z) / 2);

		SetOBB(Punch1, pos, mat, length);
	}

	// 플레이어 주먹 2
	{
		D3DXMATRIX mat = player->GetModel()->GetMeshCube().at(1)->World;

		D3DXVECTOR3 pos;
		pos.x = mat._41;
		pos.y = mat._42;
		pos.z = mat._43;

		D3DXVECTOR3 length, Max, Min;
		Max = player->GetBox().at(1)->collisionBox->GetMax();
		Min = player->GetBox().at(1)->collisionBox->GetMin();

		D3DXVec3TransformCoord(&Max, &Max, &mat);
		D3DXVec3TransformCoord(&Min, &Min, &mat);

		length.x = abs((Max.x - Min.x) / 2);
		length.y = abs((Max.y - Min.y) / 2);
		length.z = abs((Max.z - Min.z) / 2);

		SetOBB(Punch2, pos, mat, length);
	}

	// 몬스터 메인 사각형
	{
		D3DXVECTOR3 pos;
		enemy1->GetBindingBox()->Position(&pos);

		D3DXMATRIX mat;
		enemy1->GetBindingBox()->Matrix(&mat);

		D3DXVECTOR3 length, Max, Min;
		Max = player->GetBindingBox()->GetMax();
		Min = player->GetBindingBox()->GetMin();

		length = (Max - Min) / 2;

		SetOBB(OBB2, pos, mat, length);
	}



	// 왼손 체크
	{
		bool enable = player->GetBox().at(0)->enable;

		if (enable == true) // 왼손
			bPunch1 = CheckOBB(Punch1, OBB2);
		else bPunch1 == false;

		if (bPunch1 == true && prebPunch1 == false) // 주먹이 완전히 빠진 뒤에
												 // 몬스터 함수 콜
		{
			enemy1->Hitted();

		}

		prebPunch1 = bPunch1;
	}
	
	// 오른손 체크
	{
		bool enable = player->GetBox().at(1)->enable;

		if (enable == true) // 왼손
			bPunch2 = CheckOBB(Punch2, OBB2);
		else bPunch2 == false;

		if (bPunch2 == true && prebPunch2 == false) // 주먹이 완전히 빠진 뒤에
													// 몬스터 함수 콜
		{
			enemy1->Hitted();

		}

		prebPunch2 = bPunch2;
	}



}

void TestPlay::ClearCheckBox()
{
}

void TestPlay::SetOBB(OUT ST_OBB & Box, D3DXVECTOR3 & position, D3DXMATRIX & world, D3DXVECTOR3 & length)
{
	// 중점
	Box.centerPos = position;


	// xyz 각각의 방향벡터
	Box.axis[0] = D3DXVECTOR3(world._11, world._12, world._13);
	Box.axis[1] = D3DXVECTOR3(world._21, world._22, world._23);
	Box.axis[2] = D3DXVECTOR3(world._31, world._32, world._33);
	
	// 를 정규화(안해도 똑같지않나)
	D3DXVec3Normalize(&Box.axis[0], &Box.axis[0]);
	D3DXVec3Normalize(&Box.axis[1], &Box.axis[1]);
	D3DXVec3Normalize(&Box.axis[2], &Box.axis[2]);


	// 중점에서 xyz 각 평면까지의 길이 (반지름)
	Box.axisLen[0] = length.x;
	Box.axisLen[1] = length.y;
	Box.axisLen[2] = length.z;
}

bool TestPlay::CheckOBB(ST_OBB& Box1, ST_OBB& Box2)
{
	double c[3][3];
	double absC[3][3];
	double d[3];
	double r0, r1, r;
	int i;
	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	D3DXVECTOR3 diff = Box1.centerPos - Box2.centerPos;


	for (i = 0; i < 3; ++i)
	{
		c[0][i] = D3DXVec3Dot(&Box1.axis[0], &Box2.axis[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}


	d[0] = D3DXVec3Dot(&diff, &Box1.axis[0]);
	r = abs(d[0]);
	r0 = Box1.axisLen[0];
	r1 = Box2.axisLen[0] * absC[0][0] + Box2.axisLen[1] * absC[0][1] + Box2.axisLen[2] * absC[0][2];
	if (r > r0 + r1)
	{
		return false;
	}
	for (i = 0; i < 3; ++i)
	{
		c[1][i] = D3DXVec3Dot(&Box1.axis[1], &Box2.axis[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = D3DXVec3Dot(&diff, &Box1.axis[1]);
	r = abs(d[1]);
	r0 = Box1.axisLen[1];
	r1 = Box2.axisLen[0] * absC[1][0] + Box2.axisLen[1] * absC[1][1] + Box2.axisLen[2] * absC[1][2];
	if (r > r0 + r1)
	{
		return false;

	}
	for (i = 0; i < 3; ++i)
	{
		c[2][i] = D3DXVec3Dot(&Box1.axis[2], &Box2.axis[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = D3DXVec3Dot(&diff, &Box1.axis[2]);
	r = abs(d[2]);
	r0 = Box1.axisLen[2];
	r1 = Box2.axisLen[0] * absC[2][0] + Box2.axisLen[1] * absC[2][1] + Box2.axisLen[2] * absC[2][2];
	if (r > r0 + r1)
	{
		return false;

	}
	r = abs(D3DXVec3Dot(&diff, &Box2.axis[0]));
	r0 = Box1.axisLen[0] * absC[0][0] + Box1.axisLen[1] * absC[1][0] + Box1.axisLen[2] * absC[2][0];
	r1 = Box2.axisLen[0];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(D3DXVec3Dot(&diff, &Box2.axis[1]));
	r0 = Box1.axisLen[0] * absC[0][1] + Box1.axisLen[1] * absC[1][1] + Box1.axisLen[2] * absC[2][1];
	r1 = Box2.axisLen[1];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(D3DXVec3Dot(&diff, &Box2.axis[2]));
	r0 = Box1.axisLen[0] * absC[0][2] + Box1.axisLen[1] * absC[1][2] + Box1.axisLen[2] * absC[2][2];
	r1 = Box2.axisLen[2];
	if (r > r0 + r1)
	{
		return false;

	}


	if (existsParallelPair == true)
	{
		return true;

	}


	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = Box1.axisLen[1] * absC[2][0] + Box1.axisLen[2] * absC[1][0];
	r1 = Box2.axisLen[1] * absC[0][2] + Box2.axisLen[2] * absC[0][1];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = Box1.axisLen[1] * absC[2][1] + Box1.axisLen[2] * absC[1][1];
	r1 = Box2.axisLen[0] * absC[0][2] + Box2.axisLen[2] * absC[0][0];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = Box1.axisLen[1] * absC[2][2] + Box1.axisLen[2] * absC[1][2];
	r1 = Box2.axisLen[0] * absC[0][1] + Box2.axisLen[1] * absC[0][0];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = Box1.axisLen[0] * absC[2][0] + Box1.axisLen[2] * absC[0][0];
	r1 = Box2.axisLen[1] * absC[1][2] + Box2.axisLen[2] * absC[1][1];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = Box1.axisLen[0] * absC[2][1] + Box1.axisLen[2] * absC[0][1];
	r1 = Box2.axisLen[0] * absC[1][2] + Box2.axisLen[2] * absC[1][0];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = Box1.axisLen[0] * absC[2][2] + Box1.axisLen[2] * absC[0][2];
	r1 = Box2.axisLen[0] * absC[1][1] + Box2.axisLen[1] * absC[1][0];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = Box1.axisLen[0] * absC[1][0] + Box1.axisLen[1] * absC[0][0];
	r1 = Box2.axisLen[1] * absC[2][2] + Box2.axisLen[2] * absC[2][1];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = Box1.axisLen[0] * absC[1][1] + Box1.axisLen[1] * absC[0][1];
	r1 = Box2.axisLen[0] * absC[2][2] + Box2.axisLen[2] * absC[2][0];
	if (r > r0 + r1)
	{
		return false;

	}


	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = Box1.axisLen[0] * absC[1][2] + Box1.axisLen[1] * absC[0][2];
	r1 = Box2.axisLen[0] * absC[2][1] + Box2.axisLen[1] * absC[2][0];
	if (r > r0 + r1)
	{
		return false;

	}

	return true;

}

