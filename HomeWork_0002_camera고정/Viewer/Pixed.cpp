#include "stdafx.h"
#include "Pixed.h"

Pixed::Pixed(float moveSpeed, float rotationSpeed)
	:moveSpeed(moveSpeed), rotationSpeed(rotationSpeed), angle(Math::PI * 0)
{

}

Pixed::~Pixed()
{
}

void Pixed::Update()
{
	



	D3DXVECTOR3 F = Forward();
	D3DXVECTOR3 R = Right();
	D3DXVECTOR3 U = Up();

	// 이동처리
	{
		D3DXVECTOR3 position;
		Position(&position);

		D3DXVECTOR3 at;
		At(&at);


		if (Mouse::Get()->Press(0) == false)
		{
			// at바꾸자 바꿈.

			if (Keyboard::Get()->Press('W'))
			{
				at.z += 1.0f * Time::Delta();
			}
			else if (Keyboard::Get()->Press('S'))
			{
				at.z -= 1.0f * Time::Delta();
			}

			if (Keyboard::Get()->Press('A'))
			{
				at.x -= 1.0f * Time::Delta();
			}
			else if (Keyboard::Get()->Press('D'))
			{
				at.x += 1.0f * Time::Delta();
			}

			At(at);

			// Position바꾸자 바꿈.

			if (Keyboard::Get()->Press('W'))
			{
				position.z += 1.0f * Time::Delta();
			}
			else if (Keyboard::Get()->Press('S'))
			{
				position.z -= 1.0f * Time::Delta();
			}

			if (Keyboard::Get()->Press('A'))
			{
				position.x -= 1.0f * Time::Delta();
			}
			else if (Keyboard::Get()->Press('D'))
			{
				position.x += 1.0f * Time::Delta();
			}

			Position(position);
		}
		if (Mouse::Get()->Press(0)) // 좌클릭
		{
			
			if (Keyboard::Get()->Press('W'))
				position += F * moveSpeed * Time::Delta();

			else if (Keyboard::Get()->Press('S'))
				position += -F * moveSpeed * Time::Delta();

			if (Keyboard::Get()->Press('A'))
				position += -R * moveSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('D'))
				position += R * moveSpeed * Time::Delta();

			if (Keyboard::Get()->Press('E'))
				position += U * moveSpeed * Time::Delta();
			else if (Keyboard::Get()->Press('Q'))
				position += -U * moveSpeed * Time::Delta();

			Position3(position);
		}


	}


	// 회전처리
	{
		D3DXVECTOR2 R;
		Rotation(&R);


		if (Mouse::Get()->Press(0))
		{
			D3DXVECTOR3 val = Mouse::Get()->GetMoveValue();

			R.x += val.y * rotationSpeed * Time::Delta();
			R.y += val.x * rotationSpeed * Time::Delta();

			Rotation(R);
		}
		

	}

}

void Pixed::Render()
{

	D3DXVECTOR3 at;
	At(&at);

	D3DXVECTOR3 position;
	Position(&position);

	float RR = sqrt(
		(position.x - at.x)*(position.x - at.x) +
		(position.y - at.y)*(position.y - at.y) +
		(position.z - at.z)*(position.z - at.z)); // 두 점사이의 거리




	ImGui::SliderFloat3("at", at, -5, 5);
	ImGui::SliderFloat("RR", &RR, -50, 50);
	ImGui::SliderFloat("angle", &angle, -50, 50);
}
