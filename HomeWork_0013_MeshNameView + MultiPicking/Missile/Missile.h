#pragma once

class Missile
{
public:
	Missile(class Terrain* terrain, class Material* material,
		class GameModel* gameModel, D3DXVECTOR3 DirectionNormal);
	~Missile();

	void Update();
	void Render();
	bool GetDelete() { return bDeleteMissile; }
	void SetDelete(bool deleteMissile)
	{
		bDeleteMissile = deleteMissile;
	}


	D3DXVECTOR3 GetMissilePos() { return MissilePosition; }
	
	MeshCube* GetMeshCube() { return MissileCube;}// �浹ó����
private:
	void MoveToMissile();
	void DeleteMissile();

private:
	class Terrain* terrain;
	class Material* material;
	class GameModel* gameModel;

	class MeshCube* MissileCube;

	D3DXVECTOR3 StartPosition; // ó�� �̻��� ������
	D3DXVECTOR3 StartRotation; // 

	D3DXVECTOR3 MissilePosition; // �� ������, �̻��� ������
	D3DXVECTOR3 MissileRotation; // �� ������

	D3DXVECTOR3 DirectionNormal;

	bool bDeleteMissile;
	
};