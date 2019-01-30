#include "framework.h"
#include "GameAnimator.h"
#include "Model/ModelClip.h"

GameAnimator::GameAnimator(wstring shaderFile, wstring matFile, wstring meshFile)
	: GameModel(shaderFile, matFile, meshFile),
	currentClip(0), currentKeyframe(0), nextKeyframe(0)
	, frameTime(0.0f), frameFactor(0.0f)
{
	pass = 1;

}

GameAnimator::~GameAnimator()
{
	for (ModelClip* clip : clips)
		SAFE_DELETE(clip);

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
	__super::Update();

	frameTime += Time::Delta();

	ModelClip* clip = clips[currentClip];
	float invFrameRate = 1.0f / clip->FrameRate();
	if (frameTime >= invFrameRate)
	{
		frameTime = 0.0f;

		UINT temp = clip->FrameCount();
		currentKeyframe = (currentKeyframe + 1) % clip->FrameCount();
		nextKeyframe = (currentKeyframe + 1) % clip->FrameCount();
		
		// 프레임 바뀔때마다 갱신됨
		UpdateTransforms();
		MappedBoneBuffer();
		UpdateVertex();


	}

	MaxFrameCount = clip->FrameCount();
}

void GameAnimator::Render()
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Render();

	// 바인딩 박스
	{
		for (CubeStruct* cube : model->BindingBox())
		{
			//cube->meshCubeMinMax->Render();
		}

		
		for (UINT i = 0; i < Cubes.size(); i++)
		{	
			//if (Cubes.at(i)->enable == true)
				//Cubes.at(i)->cube->Render();
		}
	}

}

void GameAnimator::UpdateWorld()
{
	GameRender::UpdateWorld();

	D3DXMATRIX world;
	World(&world);

	for (Material* material : model->Materials())
		material->GetShader()->AsMatrix("World")->SetMatrix(world);


	// 전체 바인딩 박스
	for (CubeStruct* box : model->BindingBox())
	{
		box->meshCubeMinMax->Matrix(world);
	}

	


}

void GameAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < model->BoneCount(); i++)
	{
		ModelBone* bone = model->BoneByIndex(i);

		ModelKeyframe* frame = clips[currentClip]->Keyframe(bone->Name());
		if (frame == NULL) continue;


		D3DXMATRIX S, R, T;

		ModelKeyframeData current = frame->Transforms[currentKeyframe];
		ModelKeyframeData next = frame->Transforms[nextKeyframe];

		
		D3DXVECTOR3 s1 = current.Scale;
		D3DXVECTOR3 s2 = next.Scale;

		D3DXVECTOR3 s;
		D3DXVec3Lerp(&s, &s1, &s2, frameFactor);
		D3DXMatrixScaling(&S, s.x, s.y, s.z);


		D3DXQUATERNION q1 = current.Rotation;
		D3DXQUATERNION q2 = next.Rotation;

		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &q1, &q2, frameFactor);
		D3DXMatrixRotationQuaternion(&R, &q);

		
		D3DXVECTOR3 t1 = current.Translation;
		D3DXVECTOR3 t2 = next.Translation;

		D3DXVECTOR3 t;
		D3DXVec3Lerp(&t, &t1, &t2, frameFactor);
		D3DXMatrixTranslation(&T, t.x, t.y, t.z);

		
		D3DXMATRIX animation = S * R * T;

		
		D3DXMATRIX transform;
		D3DXMATRIX parentTransform;

		int parentIndex = bone->ParentIndex();
		if (parentIndex < 0)
		{
			D3DXMatrixIdentity(&parentTransform);
		}
		else
		{
			parentTransform = boneTransforms[parentIndex];
		}



		D3DXMATRIX inv = bone->Global();
		D3DXMatrixInverse(&inv, NULL, &inv);


		boneTransforms[i] = animation * parentTransform;
		renderTransforms[i] = inv * boneTransforms[i];




		// 로컬 바인딩 박스
		for (UINT j = 0; j < Cubes.size(); j++)
		{

			if (Cubes.at(j)->Index == i)
			{
				D3DXMATRIX world;
				World(&world);

				Cubes.at(j)->cube->Matrix(boneTransforms[i] * world);
				Cubes.at(j)->World = boneTransforms[i] * world;

				//Cubes.at(j)->Rotation = 
			}

		}

	

	}
}
