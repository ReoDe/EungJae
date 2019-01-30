#include "stdafx.h"
#include "GameAnimator.h"
#include "Model\ModelClip.h"

GameAnimator::GameAnimator(wstring shaderFile, wstring matFile, wstring meshFile)
	:currentClip(0), currentKeyframe(0), nextKeyframe(0), frameFactor(0), maxCount(1)
	, frameTime(0.0f), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1)
{
	model = new Model();
	model->ReadMaterial(matFile);
	model->ReadMesh(meshFile);

	for (Material* material : model->Materials())
		material->SetShader(shaderFile);

	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(1);


	boneTransforms.assign(model->BoneCount(), D3DXMATRIX());
	renderTransforms.assign(model->BoneCount(), D3DXMATRIX());
}

GameAnimator::~GameAnimator()
{
	for (ModelClip* clip : clips)
		SAFE_DELETE(clip);

	SAFE_DELETE(model);
}

void GameAnimator::AddClip(wstring clipFile)
{
	clips.push_back(new ModelClip(clipFile));
}

void GameAnimator::ClipNumber(UINT index)
{
	currentClip = index;
}

void GameAnimator::Update()
{
	frameTime += Time::Delta();

	ModelClip* clip = clips[currentClip];
	float invFrameRate = 1.0f / clip->FrameRate(); // ������ 1/30 ��, 30������

	if (frameTime >= invFrameRate)
	{
		frameTime = 0.0f;

		currentKeyframe = (currentKeyframe + 1) % clip->FrameCount();// ����
		nextKeyframe = (currentKeyframe + 1) % clip->FrameCount();// ����
																  // ������������ �־�� ����

		UpdateBone();
	}



}



void GameAnimator::Render()
{
	for (Material* material : model->Materials())
	{
		const float* data = renderTransforms[0];
		UINT count = renderTransforms.size();

		material->GetShader()->AsMatrix("Bones")->SetMatrixArray(data, 0, count);
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Render();
}



void GameAnimator::UpdateBone()
{
	for (UINT i = 0; i < model->BoneCount(); i++)
	{
		ModelBone* bone = model->BoneByIndex(i);



		// �ش� ���� �ִϸ��̼��� �������� ����
		ModelKeyframe* frame = clips[currentClip]->Keyframe(bone->Name());
		if (frame == NULL) continue;

		D3DXMATRIX S, R, T;

		ModelKeyframeData current = frame->Transforms[currentKeyframe];
		ModelKeyframeData next = frame->Transforms[nextKeyframe];

		// ������ ���� ����
		D3DXVECTOR3 s1 = current.Scale;
		D3DXVECTOR3 s2 = next.Scale;

		D3DXVECTOR3 s;
		// ���� ����
		D3DXVec3Lerp(&s, &s1, &s2, frameFactor);
		D3DXMatrixScaling(&S, s.x, s.y, s.z);

		D3DXQUATERNION q1 = current.Rotation;
		D3DXQUATERNION q2 = current.Rotation;

		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &q1, &q2, frameFactor);
		D3DXMatrixRotationQuaternion(&R, &q);

		// ������ ���� ����
		D3DXVECTOR3 t1 = current.Translation;
		D3DXVECTOR3 t2 = next.Translation;

		D3DXVECTOR3 t;
		// ���� ����
		D3DXVec3Lerp(&t, &t1, &t2, frameFactor);
		D3DXMatrixTranslation(&T, t.x, t.y, t.z);

		
		D3DXMATRIX animation = S * R * T;

		D3DXMATRIX transform;
		D3DXMATRIX parentTransform;

		int parentIndex = bone->ParentIndex();
		if (parentIndex < 0) // �Ѹ����, �������
		{
			D3DXMatrixIdentity(&parentTransform);


			World(&parentTransform);

		}

		else // �� ��
			parentTransform = boneTransforms[parentIndex];
		// ������� �θ� ��



		// �̵� ���带 bone�� �־����?

		// �ڱ��� �۷ι� �����
		D3DXMATRIX inv = bone->Global();
	
		// �۷ι� inv = ����
		D3DXMatrixInverse(&inv, NULL, &inv);
	



		//�ִϸ��̼� * �θ� = ���������
		boneTransforms[i] = animation * parentTransform;
		// inv * animation * parentTransform << ��������ǥ��, �޼��� �ݴ�
		renderTransforms[i] = inv * boneTransforms[i];


	}
}

