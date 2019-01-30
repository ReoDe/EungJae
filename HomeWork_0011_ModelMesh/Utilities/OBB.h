#pragma once

class OBB
{

public:
	OBB(struct Ray* ray, class MeshCubeParts* meshCubeParts);
	~OBB();
	
	void MakePlane();
	float DistancePlane(D3DXPLANE plane, float d);

	bool InterSectRay();
	void SetRay(Ray* RRay)
	{
		rray = RRay;
	}


private:
	float tMinX; // ���� �߰ߵ� ���� ����� ������, x
	float tMaxX; // ���� �߰ߵ� ���� �� ������, x
	float tMinY;
	float tMaxY;
	float tMinZ;
	float tMaxZ;

private:

	D3DXPLANE planeX1;
	D3DXPLANE planeX2;
	D3DXPLANE planeY1;
	D3DXPLANE planeY2;
	D3DXPLANE planeZ1;
	D3DXPLANE planeZ2;

	float dz1;
	float dz2;
	float dx1;
	float dx2;
	float dy1;
	float dy2;

	D3DXVECTOR3 Position; // ī�޶�������

	struct Ray* rray;

		  
};