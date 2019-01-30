#pragma once
#include "stdafx.h"

struct Collision
{
	static bool Intersect(class MeshCube* Cube_1, class MeshCube* Cube_2)
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

};