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
	/////// �ӽ�
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

	// �ܺ� ���ٰ���

	void Hitted();
private:
	void Moving();
	void Kicking();
	void NoKicing();

	void ToIdel();

	void SetFrameCount(int val) { gameModel->SetKeyFrame(val); }

	// �ӽ�
	void SetBox();
private:

	GameAnimator* gameModel;
	// 
	class Terrain* terrain;
	// �Ӽ�

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


	// ĳ���Ϳ��� ��ȣ�ۿ�
	MeshSphere* eyeSphere;
	float eyeRange;

	MeshSphere* attackSphere;
	float attackRange;


private:
	// �÷��̾���� ��ȣ�ۿ�
	class Player* player;

	float dist;

private:
	// ����
	class EnemyState* _enemyState;
	class EnemyState* _preEnemyState;

private:
	// �ִϸ��̼� Ŭ��

	enum ClipName
	{
		clipName_Idle,
		clipName_Run,
		clipName_Kicking,
		clipName_Hitted

	};

private:
	// �ӽ�
	bool checkbox;
	vector<MeshCube*> collisionBox;
	MeshCubeMinMax* mainBox;

private:
		// �浹ó���� �ڽ��� ���� ������

		// ��ü ����
		MeshCube* bodyBox;
};