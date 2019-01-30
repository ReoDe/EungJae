#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	:position(0, 5, -5), rotation(0, 0)
	, forward(0, 0, 1), right(0, 0, 0), up(0, 0, 0) // up�� ����
	, at(0, 0, 0)
	
{
	// forward�� �� z ��, up�� ��������, �׸��� �ΰ��� x��(right)����

	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView); // ��� �ʱ�ȭ �������


	Rotation();
	Move();
	
}

Camera::~Camera()
{
}

void Camera::Render()
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

void Camera::Position3(D3DXVECTOR3 & vec)
{
	position = vec;
	View3();
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

void Camera::At(float x, float y, float z)
{
	At(D3DXVECTOR3(x, y, z));
}

void Camera::At(D3DXVECTOR3 & vec)
{
	at = vec;
	View();
}

void Camera::At(D3DXVECTOR3 * vec)
{
	*vec = at;
}

void Camera::Move()
{
	View();
}

void Camera::Rotation()
{
	D3DXMATRIX X, Y;
	D3DXMatrixRotationX(&X, rotation.x); // �ʱ�ȭ �ϰ� �� (���Ʒ�)ȸ��
	D3DXMatrixRotationY(&Y, rotation.y); // �ʱ�ȭ �ϰ� y�� ���� (�¿�)ȸ��

	matRotation = X * Y;

	//�븻�� ���� , ��κ� �� ���� ����
	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0,0,1), &matRotation);
	D3DXVec3TransformNormal(&right, &D3DXVECTOR3(1,0,0), &matRotation);
	D3DXVec3TransformNormal(&up, &D3DXVECTOR3(0,1,0), &matRotation);
	//��ġ == transformation�ϰ� ���
	//D3DXVec3TransformCoord
}

void Camera::View()
{
	D3DXMatrixLookAtLH(&matView, &position, &at, &up);
}

void Camera::View3()
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
}
