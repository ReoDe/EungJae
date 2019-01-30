#pragma once

class Enemy
{
public:
	friend class EnemyState;
	friend class EstateIdle;
	friend class EstateRun;
	friend class EstateHitted;
public:
	Enemy(class Terrain* terrain);
	~Enemy();

	void Initialize();
	void Ready();
	void Update();
	void Destroy();
	void PreRender();
	void Render();


	void setplayerAddressLink(class Player* pl)
	{
		player = pl;
	}

	GameAnimator* GetModel()
	{
		return gameModel;
	}
	/////// 임시
	void TrueCheckBox()
	{
		checkbox = true;
	}



	vector<MeshCube*> & GetBox() { return collisionBox; }
	MeshCube* & GetBindingBox() { return bodyBox; }

	void SetPosition(D3DXVECTOR3 vac)
	{
		modelPosition = vac;
	}

	// 외부 접근가능

	void Hitted();
private:
	void Moving();
	void Kicking();
	void NoKicing();

	void ToIdel();

	void SetFrameCount(int val) { gameModel->SetKeyFrame(val); }

	// 임시
	void SetBox();
private:

	GameAnimator* gameModel;
	// 
	class Terrain* terrain;
	// 속성

	D3DXVECTOR3 modelPosition;
	D3DXVECTOR3 modelRotation;
	D3DXVECTOR3 modelScale;

	D3DXVECTOR3 modelPositionDirection;
	
	float modelSpeed;


	bool bRun;
	bool bKicking;
	bool bHitted;


	int Keyframe;
	int MaxKeyFrameCount;


	// 캐릭터와의 상호작용
	MeshSphere* eyeSphere;
	float eyeRange;

	MeshSphere* attackSphere;
	float attackRange;


private:
	// 플레이어와의 상호작용
	class Player* player;

	float dist;

private:
	// 상태
	class EnemyState* _enemyState;
	class EnemyState* _preEnemyState;

private:
	// 애니메이션 클립

	enum ClipName
	{
		clipName_Idle,
		clipName_Run,
		clipName_Kicking,
		clipName_Hitted

	};

private:
	// 임시
	bool checkbox;
	vector<MeshCube*> collisionBox;
	MeshCubeMinMax* mainBox;

private:
		// 충돌처리용 박스를 새로 만들자

		// 전체 몸통
		MeshCube* bodyBox;
};