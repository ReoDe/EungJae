#include "framework.h"
#include "Freedom.h"

Freedom::Freedom(float moveSpeed, float rotationSpeed)
	: moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{

}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	D3DXVECTOR3 F = Forward();
	D3DXVECTOR3 R = Right();
	D3DXVECTOR3 U = Up();

	//捞悼贸府
	{
		D3DXVECTOR3 position;
		Position(&position);

		if (Mouse::Get()->Press(1))
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

			Position(position);
		}
	}

	//雀傈贸府
	{
		D3DXVECTOR2 R;
		Rotation(&R);

		if (Mouse::Get()->Press(1))
		{
			D3DXVECTOR3 val = Mouse::Get()->GetMoveValue();

			R.x += val.y * rotationSpeed * Time::Delta();
			R.y += val.x * rotationSpeed * Time::Delta();

			Rotation(R);
		}
	}
}
