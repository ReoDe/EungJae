#pragma once
// 박스
struct LocalBindingBoxes
{
	MeshCube* collisionBox;
	bool enable;
	D3DXMATRIX world;
};


class Player
{
public:
	friend class PlayerState;
	friend class stateIdle;
	friend class stateRun;
public:
	Player(class Terrain* terrain);
	~Player() {};

	void Initialize();
	void Ready();
	void Update();
	void Destroy();
	void PreRender();
	void Render();

	D3DXVECTOR3 GetPosition() { return modelPosition; }

	GameAnimator* GetModel()
	{
		return gameModel;
	}

	vector<LocalBindingBoxes*>& GetBox() { return localBox; }
	MeshCube* & GetBindingBox() { return bodyBox; }
private:

	void Moving();
	void Punching();
	void NoPunching();

	// 유틸

	void SetFrameCount(int val) { gameModel->SetKeyFrame(val); }

	void ClearEnable()
	{
		for (UINT i = 0; i < gameModel->GetMeshCube().size(); i++)
		{
			localBox.at(i)->enable = false;
		}
	}
	// 이벤트

	void EventPlayer();

	void Punch_1();
	void Punch_2();
	void Punch_3();
	
	// 박스
	void SetBox();

	void UpdateBox();

private:
	GameAnimator* gameModel; // 모델
							 //
	class Terrain* terrain;
	
	// 속성

	D3DXVECTOR3 mousePosition;

	D3DXVECTOR3 modelPosition;
	D3DXVECTOR3 modelPositionDirection;
	D3DXVECTOR3 modelRotation;
	D3DXVECTOR3 modelScale;

	float modelSpeed;



	MeshCubeMinMax* mainBox;

private:
	// 충돌처리용 박스를 새로 만들자

	// 전체 몸통
	MeshCube* bodyBox;
	// 로컬
	vector<LocalBindingBoxes*> localBox;
private:
	// 상태
	class PlayerState* _playerState;
	class PlayerState* _prePlayerState;


	bool bRun;
	int PunchCount;

	int Keyframe;
	int MaxKeyFrameCount;

private:
	// 애니메이션 클립
	
	enum ClipName
	{
		clipName_Idle,
		clipName_Run,
		clipName_Punch_1,
		clipName_Punch_2,
		clipName_punch_3
	};

private:

	D3DXVECTOR3 MaxVer, MinVer;

private:
	// 무기
	GameModel* sword;
	
	void QuatToPitchYawRoll(const D3DXQUATERNION& q,FLOAT& pitch, FLOAT& yaw, FLOAT& roll, BOOL bToDegree)
	{
		FLOAT sqw = q.w * q.w;
		FLOAT sqx = q.x * q.x;
		FLOAT sqy = q.y * q.y;
		FLOAT sqz = q.z * q.z;

		// rotation about x-axis
		pitch = asinf(2.0f * (q.w * q.x - q.y * q.z));
		// rotation about y-axis
		yaw = atan2f(2.0f * (q.x * q.z + q.w * q.y), (-sqx - sqy + sqz + sqw));
		// rotation about z-axis
		roll = atan2f(2.0f * (q.x * q.y + q.w * q.z), (-sqx + sqy - sqz + sqw));

		if (bToDegree)

		{
			pitch = D3DXToDegree(pitch);
			yaw = D3DXToDegree(yaw);
			roll = D3DXToDegree(roll);
		}

	}

	void SetSRTsword()
	{
		D3DXMATRIX world;
		world = gameModel->GetMeshCube().at(1)->World;


		D3DXVECTOR3 pos, scale;
		D3DXQUATERNION rot;

		D3DXMatrixDecompose(&scale, &rot, &pos, &world);
		// 회전값 분해하고


		D3DXVECTOR3 rotation;
		QuatToPitchYawRoll(rot, rotation.x, rotation.y, rotation.z, true);
		// 오일러각으로 바꾸고


		// 얻은 회전값을 대입
		swordRotation = rotation;
		// 얻은 포지션값
		swordPos = pos;

	}
	
	void SetRotation(float x, float y, float z,OUT D3DXVECTOR3& rotation)
	{
		rotation.x += x;
		if (rotation.x > 180) rotation.x -= 360;
		if (rotation.x < -180) rotation.x += 360;

		rotation.y += y;
		if (rotation.y > 180) rotation.y -= 360;
		if (rotation.y < -180) rotation.y += 360;

		rotation.z += z;
		if (rotation.z > 180) rotation.z -= 360;
		if (rotation.z < -180) rotation.z += 360;
	}
	
	D3DXVECTOR3 swordPos, swordRotation, swordScale;


};
