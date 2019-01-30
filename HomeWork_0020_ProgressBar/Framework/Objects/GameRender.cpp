#include "framework.h"
#include "GameRender.h"

GameRender::GameRender()
	: position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
	, direction(0, 0, 1), up(0, 1, 0), right(1, 0, 0)
{
	D3DXMatrixIdentity(&world);
}

GameRender::~GameRender()
{
}

void GameRender::World(D3DXMATRIX * mat)
{
	*mat = world;
}

void GameRender::Position(float x, float y, float z)
{
	Position(D3DXVECTOR3(x, y, z));
}

void GameRender::Position(D3DXVECTOR3 & vec)
{
	position = vec;

	UpdateWorld();
}

void GameRender::Position(D3DXVECTOR3 * vec)
{
	*vec = position;
}

void GameRender::Scale(float x, float y, float z)
{
	Scale(D3DXVECTOR3(x, y, z));
}

void GameRender::Scale(D3DXVECTOR3 & vec)
{
	scale = vec;

	UpdateWorld();
}

void GameRender::Scale(D3DXVECTOR3 * vec)
{
	*vec = scale;
}

void GameRender::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void GameRender::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;

	UpdateWorld();
}

void GameRender::Rotation(D3DXVECTOR3 * vec)
{
	*vec = rotation;
}

void GameRender::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void GameRender::RotationDegree(D3DXVECTOR3 & vec)
{
	D3DXVECTOR3 temp;

	temp.x = Math::ToRadian(vec.x);
	temp.y = Math::ToRadian(vec.y);
	temp.z = Math::ToRadian(vec.z);

	Rotation(temp);
}

void GameRender::RotationDegree(D3DXVECTOR3 * vec)
{
	D3DXVECTOR3 temp;

	temp.x = Math::ToDegree(rotation.x);
	temp.y = Math::ToDegree(rotation.y);
	temp.z = Math::ToDegree(rotation.z);

	*vec = temp;
}

void GameRender::Direction(D3DXVECTOR3 * vec)
{
	*vec = direction;
}

void GameRender::Up(D3DXVECTOR3 * vec)
{
	*vec = up;
}

void GameRender::Right(D3DXVECTOR3 * vec)
{
	*vec = right;
}

void GameRender::UpdateWorld()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * R * T;

	direction = D3DXVECTOR3(world._31, world._32, world._33);
	up = D3DXVECTOR3(world._21, world._22, world._23);
	right = D3DXVECTOR3(world._11, world._12, world._13);
}
