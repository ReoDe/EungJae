#include "stdafx.h"
#include "Enemy.h"
#include "EnemyState.h"
#include "./Player/Player.h"

Enemy::Enemy(Terrain * terrain)
	: terrain(terrain)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	modelPosition = D3DXVECTOR3(10, 0, 10);
	modelPosition.y = terrain->GetHeightY(modelPosition);
	modelPositionDirection = D3DXVECTOR3(0, 0, 0);
	modelRotation = D3DXVECTOR3(0, 0, 0);
	modelScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	modelSpeed = 1.5f;

	bRun = false;
	bKicking = false;
	bHitted = false;

	////////////////////////////////
	// ����
	////////////////////////////////

	_enemyState = new EnemyState();
	_preEnemyState = new EnemyState();

	_enemyState->SetEnemyAddressLink(this);
	_preEnemyState->SetEnemyAddressLink(this);



	////////////////////////////////
	// ��ƿ
	////////////////////////////////

	// �þ�
	{
		eyeRange = 6.0f;
		eyeSphere = new MeshSphere(NULL, eyeRange);
		D3DXCOLOR color = D3DXCOLOR(1, 0, 0, 1);
		eyeSphere->Color(color);
	}

	// ���ݻ�Ÿ�
	{
		attackRange = 1.5f;
		attackSphere = new MeshSphere(NULL, attackRange);
		D3DXCOLOR color = D3DXCOLOR(0, 0, 1, 1);
		attackSphere->Color(color);
	}

	dist = 0;

	// �ӽ�
	checkbox = false;
}

void Enemy::Ready()
{
	gameModel = new GameAnimator
	(
		Shaders + L"046_Model.fx",
		Models + L"Starkie/Starkie.material",
		Models + L"Starkie/Starkie.mesh"
	);
	// Ŭ�� �߰�
	{
		gameModel->AddClip(Models + L"Kachujin/Idle.animation");
		gameModel->AddClip(Models + L"Motion/Zombie_Walking.animation");
		gameModel->AddClip(Models + L"Motion/Zombie_Kicking.animation");

		gameModel->AddClip(Models + L"Motion/Head_Hit.animation");

	}


	gameModel->Ready();

	gameModel->ClipNumber(clipName_Idle);

	// �ڽ�
	{
		gameModel->SetMeshCube(NULL, 30, 30, 30, 62);
	
	}

	gameModel->Scale(modelScale);
	gameModel->Position(modelPosition);
	////////////////////////////////
	// �浹 �ڽ�
	////////////////////////////////
	SetBox();

}

void Enemy::Update()
{
	// �ƽ� Ű �����Ӱ�
	Keyframe = gameModel->GetKeyframe();
	MaxKeyFrameCount = gameModel->GetMaxFrameCount();

	////////////////////////////////
	// ��ƿ
	////////////////////////////////
	eyeSphere->Position(modelPosition);
	attackSphere->Position(modelPosition);

	////////////////////////////////
	// ����
	////////////////////////////////

	_enemyState->handleInput();
	_enemyState->StateUpdate();

	_preEnemyState = _enemyState;




	gameModel->Update();
}

void Enemy::Destroy()
{
	SAFE_DELETE(gameModel);
	SAFE_DELETE(eyeSphere);
	SAFE_DELETE(attackSphere);
}

void Enemy::PreRender()
{
	ImGui::Separator();
	ImGui::SliderInt("frame", &Keyframe, 0, 100);
	ImGui::SliderInt("Max Frame", &MaxKeyFrameCount, 0, 100);
	ImGui::Checkbox("checkBox", &checkbox);
}

void Enemy::Render()
{

	////////////////////////////////
	// ��ƿ
	////////////////////////////////

	attackSphere->Render();
	eyeSphere->Render();



	gameModel->Render();
	bodyBox->Render();
}

void Enemy::Moving()
{

	

	dist = Math::Distance(player->GetPosition().x, player->GetPosition().z, modelPosition.x, modelPosition.z);


	// ������ �÷��̾� �������� ������
	if (dist < eyeRange)
	{
		if (modelPosition.x - modelPositionDirection.x > 0.1f ||
			modelPosition.x - modelPositionDirection.x < -0.1f)
			// �����̴���
		{
			terrain->MovePoint(&modelPosition, &modelPositionDirection, modelSpeed, modelRotation);
		
			gameModel->ClipNumber(clipName_Run);
			bRun = true;
		}


		modelPositionDirection = player->GetPosition();
		modelPositionDirection.y = 0;

	}
	else
	{
		gameModel->ClipNumber(clipName_Idle);
		bRun = false;
	}


	modelPosition.y = terrain->GetHeightY(modelPosition);


	gameModel->Scale(modelScale);
	gameModel->Rotation(modelRotation);
	gameModel->Position(modelPosition);

	// �ڽ�
	{
		bodyBox->Rotation(modelRotation);
		bodyBox->Position(modelPosition.x, modelPosition.y + 1, modelPosition.z);
	}

}

void Enemy::Kicking()
{
	dist = Math::Distance(player->GetPosition().x, player->GetPosition().z, modelPosition.x, modelPosition.z);

	if (dist < attackRange)
	{
		gameModel->ClipNumber(clipName_Kicking);
		SetFrameCount(0);

		bKicking = true;
	}

}

void Enemy::NoKicing()
{
	if (Keyframe == MaxKeyFrameCount)
	{
		bKicking = false;
	}
}

void Enemy::ToIdel()
{
	if (Keyframe == MaxKeyFrameCount)
	{
		bHitted = false;
	}
}

// �ǰݽ�
void Enemy::Hitted()
{
	gameModel->ClipNumber(clipName_Hitted);
	SetFrameCount(0);

	bHitted = true;
}



void Enemy::SetBox()
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
