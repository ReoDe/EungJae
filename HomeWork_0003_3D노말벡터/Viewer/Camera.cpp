#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	:position(0,0,-5), rotation(0.5f,0.5f)
	, forward(0,0,0), right(0,0,0), up(0,0,0)
{
	D3DXMatrixIdentity(&matRotation);
	//D3DXMatrixIdentity(&matView); // 행렬 초기화 정방행렬


	Rotation();
	Move();
	
}

Camera::~Camera()
{
}

void Camera::Position(float x, float y, float z)
{
	Position(D3DXVECTOR3(x, y, z));
}

void Camera::Position(D3DXVECTOR3 & vec)
{
	position = vec;
	View();
}

void Camera::Position(D3DXVECTOR3 * vec)
{
	*vec = position;
}

void Camera::Rotation(float x, float y)
{
	Rotation(D3DXVECTOR2(x, y));
}

void Camera::Rotation(D3DXVECTOR2 & vec)
{
	rotation = vec;

	Rotation();
}

void Camera::Rotation(D3DXVECTOR2 * vec)
{
	*vec = rotation;
}

void Camera::RotationDegree(float x, float y)
{
	RotationDegree(D3DXVECTOR2 (x, y));
}

void Camera::RotationDegree(D3DXVECTOR2 & vec)
{
	rotation = vec * 0.017453281f;

	Rotation();
}

void Camera::RotationDegree(D3DXVECTOR2 * vec)
{
	*vec = rotation * 180.0f / Math::PI;
}

void Camera::Matrix(D3DXMATRIX * mat)
{
	*mat = matView;
}

void Camera::Move()
{
	View();
}

void Camera::Rotation()
{
	D3DXMATRIX X, Y;
	D3DXMatrixRotationX(&X, rotation.x); // 초기화 하고 함 (위아래)회전
	D3DXMatrixRotationY(&Y, rotation.y); // 초기화 하고 y에 적용 (좌우)회전

	matRotation = X * Y;

	//노말이 방향 , 대부분 맨 앞이 리턴
	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0,0,1), &matRotation);
	D3DXVec3TransformNormal(&right, &D3DXVECTOR3(1,0,0), &matRotation);
	D3DXVec3TransformNormal(&up, &D3DXVECTOR3(0,1,0), &matRotation);
	//위치 == transformation하고 비슷
	//D3DXVec3TransformCoord
}

void Camera::View()
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
}
