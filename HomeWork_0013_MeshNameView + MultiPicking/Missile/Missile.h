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
	
	MeshCube* GetMeshCube() { return MissileCube;}// 충돌처리용
private:
	void MoveToMissile();
	void DeleteMissile();

private:
	class Terrain* terrain;
	class Material* material;
	class GameModel* gameModel;

	class MeshCube* MissileCube;

	D3DXVECTOR3 StartPosition; // 처음 미사일 포지션
	D3DXVECTOR3 StartRotation; // 

	D3DXVECTOR3 MissilePosition; // 모델 포지션, 미사일 포지션
	D3DXVECTOR3 MissileRotation; // 모델 포지션

	D3DXVECTOR3 DirectionNormal;

	bool bDeleteMissile;
	
};