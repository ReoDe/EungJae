#pragma once
#include "framework.h"

struct Collision
{
	static bool IntersectAABB(MeshCubeMinMax* Cube_1, MeshCubeMinMax* Cube_2)
	{
		D3DXVECTOR3 Cube_1_Position, Cube_2_Position;

		Cube_1->Position(&Cube_1_Position);
		Cube_2->Position(&Cube_2_Position);

		// 정점의 min max 값에 현재 포지션값을 더해준다.
		D3DXVECTOR3 Cube_1_Max = Cube_1->GetMax() + Cube_1_Position;
		D3DXVECTOR3 Cube_1_Min = Cube_1->GetMin() + Cube_1_Position;

		D3DXVECTOR3 Cube_2_Max = Cube_2->GetMax() + Cube_2_Position;
		D3DXVECTOR3 Cube_2_Min = Cube_2->GetMin() + Cube_2_Position;


		// 충돌판정

		if (Cube_1_Min.x <= Cube_2_Max.x && Cube_1_Max.x >= Cube_2_Min.x &&
			Cube_1_Min.y <= Cube_2_Max.y && Cube_1_Max.y >= Cube_2_Min.y &&
			Cube_1_Min.z <= Cube_2_Max.z && Cube_1_Max.z >= Cube_2_Min.z)
			return true;
		else
			return false;
	}

	static bool IntersectAABB(MeshCubeMinMax* Cube_1, MeshCube* Cube_2)
	{
		D3DXVECTOR3 Cube_1_Position, Cube_2_Position;

		Cube_1->Position(&Cube_1_Position);
		Cube_2->Position(&Cube_2_Position);

		// 정점의 min max 값에 현재 포지션값을 더해준다.
		D3DXVECTOR3 Cube_1_Max = Cube_1->GetMax() + Cube_1_Position;
		D3DXVECTOR3 Cube_1_Min = Cube_1->GetMin() + Cube_1_Position;

		D3DXVECTOR3 Cube_2_Max = Cube_2->GetMax() + Cube_2_Position;
		D3DXVECTOR3 Cube_2_Min = Cube_2->GetMin() + Cube_2_Position;


		// 충돌판정

		if (Cube_1_Min.x <= Cube_2_Max.x && Cube_1_Max.x >= Cube_2_Min.x &&
			Cube_1_Min.y <= Cube_2_Max.y && Cube_1_Max.y >= Cube_2_Min.y &&
			Cube_1_Min.z <= Cube_2_Max.z && Cube_1_Max.z >= Cube_2_Min.z)
			return true;
		else
			return false;
	}

	static bool IntersectAABB(MeshCube* Cube_1, MeshCube* Cube_2)
	{
		D3DXVECTOR3 Cube_1_Position, Cube_2_Position;

		Cube_1->Position(&Cube_1_Position);
		Cube_2->Position(&Cube_2_Position);

		// 정점의 min max 값에 현재 포지션값을 더해준다.
		D3DXVECTOR3 Cube_1_Max = Cube_1->GetMax() + Cube_1_Position;
		D3DXVECTOR3 Cube_1_Min = Cube_1->GetMin() + Cube_1_Position;

		D3DXVECTOR3 Cube_2_Max = Cube_2->GetMax() + Cube_2_Position;
		D3DXVECTOR3 Cube_2_Min = Cube_2->GetMin() + Cube_2_Position;


		// 충돌판정

		if (Cube_1_Min.x <= Cube_2_Max.x && Cube_1_Max.x >= Cube_2_Min.x &&
			Cube_1_Min.y <= Cube_2_Max.y && Cube_1_Max.y >= Cube_2_Min.y &&
			Cube_1_Min.z <= Cube_2_Max.z && Cube_1_Max.z >= Cube_2_Min.z)
			return true;
		else
			return false;
	}

	// 첫번째만 월드변환 필요할 때
	static bool IntersectAABB(MeshCube* Cube_1, MeshCube* Cube_2, D3DXMATRIX Cube_1_mat)
	{
		D3DXVECTOR3 Cube_1_Position, Cube_2_Position;

		D3DXVECTOR3 Cube_1_Max, Cube_1_Min, Cube_2_Max, Cube_2_Min;

		Cube_1->Position(&Cube_1_Position);
		Cube_2->Position(&Cube_2_Position);

		Cube_1_Max = Cube_1->GetMax();
		Cube_1_Min = Cube_1->GetMin();
		Cube_2_Max = Cube_2->GetMax();
		Cube_2_Min = Cube_2->GetMin();

		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		// 웓드변환
		if (world != Cube_1_mat)
		{
			D3DXVec3TransformCoord(&Cube_1_Max, &Cube_1_Max, &Cube_1_mat);
			D3DXVec3TransformCoord(&Cube_1_Min, &Cube_1_Min, &Cube_1_mat);
		}

		Cube_1_Max;
		Cube_1_Min;
		Cube_2_Max += Cube_2_Position;
		Cube_2_Min += Cube_2_Position;


		// 충돌판정

		if (Cube_1_Min.x <= Cube_2_Max.x && Cube_1_Max.x >= Cube_2_Min.x &&
			Cube_1_Min.y <= Cube_2_Max.y && Cube_1_Max.y >= Cube_2_Min.y &&
			Cube_1_Min.z <= Cube_2_Max.z && Cube_1_Max.z >= Cube_2_Min.z)
			return true;
		else
			return false;
	}

	/*

	static bool IntersectAABB(MeshCube* Cube_1, MeshCube* Cube_2, D3DXMATRIX world_1, D3DXMATRIX world_2 )
	{
		D3DXVECTOR3 Cube_1_Max, Cube_1_Min , Cube_2_Max, Cube_2_Min;

		Cube_1_Max = Cube_1->GetMax();
		Cube_1_Min = Cube_1->GetMin();
		Cube_2_Max = Cube_2->GetMax();
		Cube_2_Min = Cube_2->GetMin();

		// 웓드변환
		D3DXVec3TransformCoord(&Cube_1_Max, &Cube_1_Max, &world_1);
		D3DXVec3TransformCoord(&Cube_1_Min, &Cube_1_Min, &world_1);

		D3DXVec3TransformCoord(&Cube_2_Max, &Cube_2_Max, &world_2);
		D3DXVec3TransformCoord(&Cube_2_Min, &Cube_2_Min, &world_2);

		
		// 충돌판정
		if (Cube_1_Min.x > Cube_2_Max.x || Cube_1_Max.x < Cube_2_Min.x)
			return false;
		if (Cube_1_Min.y > Cube_2_Max.y || Cube_1_Max.y < Cube_2_Min.y)
			return false;
		if (Cube_1_Min.z > Cube_2_Max.z || Cube_1_Max.z < Cube_2_Min.z)
			return false;
		return true;
	}

	*/
};