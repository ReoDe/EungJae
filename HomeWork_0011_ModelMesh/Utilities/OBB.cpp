#include "stdafx.h"
#include "OBB.h"




OBB::OBB(Ray * ray, MeshCubeParts * meshCubeParts)
	:tMinX(0.0f), tMaxX(100000.0f), tMinY(0.0f), tMaxY(100000.0f)
	, tMinZ(0.0f), tMaxZ(100000.0f), rray(ray)
{
	
	dz1 = meshCubeParts->GetMin().z;
	dz2 = meshCubeParts->GetMax().z;
	dx1 = meshCubeParts->GetMin().x;
	dx2 = meshCubeParts->GetMax().x;
	dy1 = meshCubeParts->GetMax().y;
	dy2 = meshCubeParts->GetMin().y;

	MakePlane();

	Position = rray->Position; // 카메라 포지션점

	tMinZ = DistancePlane(planeZ1, dz1);
	tMaxZ = DistancePlane(planeZ2, dz2);
	tMinX = DistancePlane(planeX1, dx1);
	tMaxX = DistancePlane(planeX2, dx2);
	tMinY = DistancePlane(planeY1, dy1);
	tMaxY = DistancePlane(planeY2, dy2);

}

OBB::~OBB()
{


}



void OBB::MakePlane()
{
							
	planeZ1 = D3DXPLANE(0,0,-1, dz1); // 정면
	planeZ2 = D3DXPLANE(0, 0, 1, dz2); // 후면
	planeX1 = D3DXPLANE(-1, 0, 0, dx1); // 왼쪽
	planeX2 = D3DXPLANE(1, 0, 0, dx2); // 오른쪽
	planeY1 = D3DXPLANE(0, 1, 0, dy1); // 위
	planeY2 = D3DXPLANE(0, -1, 0, dy2); // 아래
	
}

float OBB::DistancePlane(D3DXPLANE plane, float d)
{
	float dist;
	dist = fabsf(plane.a * Position.x + plane.b * Position.y + plane.c * Position.z + d)
		/ sqrtf(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

	return dist;
}

bool OBB::InterSectRay()
{

	float tMin = tMinX;
	float tMax = tMaxX;

	tMin = min(tMin, tMinY);
	tMin = min(tMin, tMinZ);

	tMax = max(tMax, tMaxY);
	tMax = max(tMax, tMaxZ);

	if (tMin < tMax) return true;
	else return false;
}
