#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
#include "Fbx/FbxLoader.h"

Player::Player(Terrain * terrain)
	: terrain(terrain)
{
}

void Player::Initialize()
{
	// �Ӽ�
	mousePosition = D3DXVECTOR3(0, 0, 0);

	modelPosition = D3DXVECTOR3(5, 0, 5);
	modelPositionDirection = D3DXVECTOR3(5, 0, 5);
	modelRotation = D3DXVECTOR3(0, 0, 0);
	modelScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	modelSpeed = 5.0f;

	// ����
	_playerState = new PlayerState();
	_prePlayerState = new PlayerState();

	_playerState->setplayerAddressLink(this);
	_prePlayerState->setplayerAddressLink(this);

	//
	bRun = false;
	PunchCount = 0;

	// ����

	swordPos = D3DXVECTOR3(0, 0, 0);
	swordRotation = D3DXVECTOR3(0, 0, 0);
	swordScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

}

void Player::Ready()
{
	gameModel = new GameAnimator
	(
		Shaders + L"046_Model.fx",
		Models + L"Kachujin/Kachujin.material",
		Models + L"Kachujin/Kachujin.mesh"
	);

	// Ŭ�� �߰�
	{
		gameModel->AddClip(Models + L"Motion/Punching_Idle.animation");
		gameModel->AddClip(Models + L"Kachujin/Running.animation");

		gameModel->AddClip(Models + L"Motion/Punching_1.animation");
		gameModel->AddClip(Models + L"Motion/Punching_2.animation");
		gameModel->AddClip(Models + L"Motion/Punching_3.animation");

	}
	
	gameModel->Ready();
	gameModel->Scale(0.01f, 0.01f, 0.01f);
	gameModel->Position(5, 0, 5);
	gameModel->ClipNumber(clipName_Idle);

	// ���� ���ε� �ڽ�
	{
		gameModel->SetMeshCube(NULL, 30, 30, 30, 10); // �޼�
		gameModel->SetMeshCube(NULL, 30, 30, 30, 34); // ������

		gameModel->GetMeshCube().at(0)->enable = false;
		gameModel->GetMeshCube().at(1)->enable = false;
	}

	////////////////
	// �浹 �ڽ�
	////////////////
	SetBox();


	///////////////////
	// ����
	//////////////////
	sword = new GameModel
	(
		Shaders + L"046_Model.fx",
		Models + L"Weapon/greatsword_of_fn.material",
		Models + L"Weapon/greatsword_of_fn.mesh"
	);
	sword->Ready();

	
}

void Player::Destroy()
{
	SAFE_DELETE(gameModel);

}

void Player::Update()
{
	// �ƽ� Ű �����Ӱ�
	Keyframe = gameModel->GetKeyframe();
	MaxKeyFrameCount = gameModel->GetMaxFrameCount();


	// ���� ��ü
	_playerState->handleInput();
	_playerState->StateUpdate();

	_prePlayerState = _playerState;

	// �̺�Ʈ

	EventPlayer();

	// �ڽ� ������
	UpdateBox();

	gameModel->Update();

	// ����

	
	
	SetSRTsword();

	SetRotation(0, 90, -90, swordRotation);

	swordPos.x += 0.05f;


	sword->Position(swordPos);
	sword->RotationDegree(swordRotation);
	sword->Scale(swordScale);
	// 

	//sword->Rotation(modelRotation);
	
	
	sword->Update();


}



void Player::PreRender()
{
	ImGui::SliderFloat("Speed", &modelSpeed, 0, 25);
	ImGui::SliderFloat3("Scale", (float*)&modelScale, 0, 1);
	ImGui::SliderInt("PunchCount", &PunchCount, 0, 4);

	ImGui::SliderInt("frame", &Keyframe, 0, 100);
	ImGui::SliderInt("Max Frame", &MaxKeyFrameCount, 0, 100);
	ImGui::Separator();

	D3DXVECTOR3 scale, rotation, trans;
	bodyBox->Scale(&scale);
	bodyBox->Rotation(&rotation);
	bodyBox->Position(&trans);

	ImGui::SliderFloat3("bodyBox Scale", (float*)&scale, 0, 10);
	ImGui::SliderFloat3("bodyBox rotation", (float*)&scale, 0, 180);
	ImGui::SliderFloat3("bodyBox position", (float*)&scale, 0, 100);

	//////////////////////////////////////////////////////////////////////

	MaxVer = localBox.at(0)->collisionBox->GetMax();
	MinVer = localBox.at(0)->collisionBox->GetMin();

	D3DXMATRIX world;
	world = gameModel->GetMeshCube().at(1)->World;

	D3DXVec3TransformCoord(&MaxVer, &MaxVer, &world);
	D3DXVec3TransformCoord(&MinVer, &MinVer, &world);


	ImGui::SliderFloat3("Number 0 Max", (float*)&MaxVer, 0, 100);
	ImGui::SliderFloat3("Number 0 Min", (float*)&MinVer, 0, 100);

	/////////////////////////////////////////////////////////////////////////



	ImGui::SliderFloat3("sword Pos", (float*)&swordRotation, -180, 180);
	
}

void Player::Render()
{
	gameModel->Render();

	bodyBox->Render();

	for (LocalBindingBoxes* cube : localBox)
	{
		if (cube->enable == true)
			cube->collisionBox->Render();
	}

	sword->Render();
}


void Player::Moving()
{

	D3DXVECTOR3 position;
	if (terrain->GetHeight(position) == true)
		this->mousePosition = position;


	// ĳ���� ������
	{

		if (modelPosition.x - modelPositionDirection.x > 0.1f ||
			modelPosition.x - modelPositionDirection.x < -0.1f)
			// �����̴���
		{
			terrain->MovePoint(&modelPosition, &modelPositionDirection, modelSpeed, modelRotation);

			gameModel->ClipNumber(clipName_Run);
			bRun = true;

		}
		else
		{
			gameModel->ClipNumber(clipName_Idle);
			bRun = false;
		}

		if (Mouse::Get()->Press(0))
		{
			modelPositionDirection = terrain->GetPickingMouseY();
			// Ŭ������ ���ϴ� ������
		}

		modelPosition.y = terrain->GetHeightY(modelPosition);

		gameModel->Scale(modelScale);
		gameModel->Rotation(modelRotation);
		gameModel->Position(modelPosition);

	}


	
}

void Player::Punching()
{
	if (PunchCount == 0)
	{
		gameModel->ClipNumber(clipName_Punch_1);
		SetFrameCount(0);

		PunchCount = 1;
		return;
	}

	if (PunchCount == 1)
	{
		gameModel->ClipNumber(clipName_Punch_2);
		SetFrameCount(0);

		PunchCount = 2;
		return;
	}

	if (PunchCount == 2)
	{
		gameModel->ClipNumber(clipName_punch_3);
		SetFrameCount(5);

		PunchCount = 3;
		return;
	}

	
}

void Player::NoPunching()
{
	if (Keyframe == MaxKeyFrameCount)
	{
		PunchCount = 0;

		modelPositionDirection = modelPosition; // ����
	}
}

void Player::EventPlayer()
{
	switch (gameModel->GetClipNumber())
	{
	case clipName_Punch_1: Punch_1(); break;
	case clipName_Punch_2: Punch_2(); break;
	case clipName_punch_3: Punch_3(); break;
	default: break;
	}
}

void Player::Punch_1()
{
	if (Keyframe == 12)
	{
		localBox.at(0)->enable = true;
	}

	if (Keyframe == 27)
	{
		localBox.at(0)->enable = false;
	}
}

void Player::Punch_2()
{
	if (Keyframe == 10)
	{
		localBox.at(1)->enable = true;
	}

	if (Keyframe == 25)
	{
		localBox.at(1)->enable = false;
	}
}

void Player::Punch_3()
{
	if (Keyframe == 12)
	{
		localBox.at(0)->enable = true;
	}

	if (Keyframe == 35)
	{
		localBox.at(0)->enable = false;
	}
}

void Player::SetBox()
{

	// ���� �ٵ�
	{
		mainBox = gameModel->GetModel()->BindingBox().at(0)->meshCubeMinMax;

		// �� ����
		D3DXMATRIX world;
		gameModel->World(&world);

		// ���� �ٵ��� xyz��
		D3DXVECTOR3 WHD; // widht, height , depth
		WHD.x = mainBox->GetWidth();
		WHD.y = mainBox->GetHeight();
		WHD.z = mainBox->GetDepth();

		D3DXVec3TransformNormal(&WHD, &WHD, &world);


		bodyBox = new MeshCube(NULL, WHD.x, WHD.y, WHD.z);
		bodyBox->Color(D3DXCOLOR(0, 1, 1, 1));


		// ���⼭ ���鶧���� ���庯ȯ���Ѽ� ������
		int a = 0;
	}
	


	// ���� �浹�ڽ�
	{
	

		for (UINT i = 0; i < gameModel->GetMeshCube().size(); i++)
		{
			LocalBindingBoxes* boxes = new LocalBindingBoxes();

			// ���� �ٵ��� xyz��
			D3DXVECTOR3 WHD; // widht, height , depth
			WHD = D3DXVECTOR3(30, 30, 30);

			MeshCube* box = new MeshCube(NULL, WHD.x, WHD.y, WHD.z);


			// �� ����
			D3DXMATRIX world;
			world = gameModel->GetMeshCube().at(i)->World;


			boxes->collisionBox = box;
			boxes->enable = false;
			boxes->world = world;

			localBox.push_back(boxes);
		}




		int a = 0;
	}
}

void Player::UpdateBox()
{
	// �ڽ� ������
	{
		for (UINT i = 0; i < gameModel->GetMeshCube().size(); i++)
		{
			// �� ����
			D3DXMATRIX world;
			world = gameModel->GetMeshCube().at(i)->World;

			localBox.at(i)->collisionBox->Matrix(world);

		}


		//bodyBox->Scale(modelScale);
		bodyBox->Rotation(modelRotation);
		bodyBox->Position(modelPosition.x, modelPosition.y + 1, modelPosition.z);

		D3DXVECTOR3 rot, pos;
		bodyBox->Rotation(&rot);
		bodyBox->Position(&pos);
		int a = 0;
	}
}

/*
���⼭ ���� �Ѱ��� ������ ���庯ȯ �� �� -> �ʵ��� �����ǰ��� ���ؼ� �ѱ�

�װ��� �浹üũ

*/