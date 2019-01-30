#include "stdafx.h"
#include "Viewport.h"
//#include "../Boundings/Ray.h"

Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{
	
}

void Viewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	viewport.TopLeftX = this->x = x;
	viewport.TopLeftY = this->y = y;
	viewport.Width = this->width = width;
	viewport.Height = this->height = height;
	viewport.MinDepth = this->maxDepth = minDepth;
	viewport.MaxDepth = this->maxDepth = maxDepth;

	RSSetViewport();
}

D3DXVECTOR3 Viewport::GetDirection(D3DXMATRIX view, D3DXMATRIX projection)
{
	D3DXVECTOR2 screenSize;
	screenSize.x = width;
	screenSize.y = height;

	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();

	
	D3DXVECTOR2 point;
	//Inv Viewport
	{
		point.x = ((2.0f * mouse.x) / screenSize.x) - 1.0f;
		point.y = (((2.0f * mouse.y) / screenSize.y) - 1.0f) * -1.0f;
	}

	//Inv Projection
	{
		point.x = point.x / projection._11;
		point.y = point.y / projection._22;
	}

	D3DXVECTOR3 direction;
	//Inv View
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);

		D3DXVec3TransformNormal(&direction, &D3DXVECTOR3(point.x, point.y, 1), &invView);
		D3DXVec3Normalize(&direction, &direction);
	}

	return direction;
}

void Viewport::GetRay(Ray * ray, D3DXVECTOR3 & start, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{
	D3DXVECTOR2 screenSize;
	screenSize.x = width;
	screenSize.y = height;

	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();


	D3DXVECTOR2 point;
	//Inv Viewport , ȭ���� -1,-1 ~ 1,1 �� �������
	{
		point.x = ((2.0f * mouse.x) / screenSize.x) - 1.0f;
		point.y = (((2.0f * mouse.y) / screenSize.y) - 1.0f) * -1.0f;
	}



	//Inv Projection, 
	{
		point.x = point.x / projection._11;
		point.y = point.y / projection._22;
	}




	D3DXVECTOR3 direction;
	//Inv View
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);

	

		D3DXVec3TransformNormal(&direction, &D3DXVECTOR3(point.x, point.y, 1), &invView);
		D3DXVec3Normalize(&direction, &direction);
	}

	//Inv World
	D3DXVECTOR3 tempStart;
	{
		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, NULL, &world);

		// �� ������ ��ġ�� ������Ŀ����� ī�޶���ġ�� World�� ī�޶� ��ġ�� ��ȯ
		D3DXVec3TransformCoord(&tempStart, &start, &invWorld);


		D3DXVec3TransformNormal(&direction, &direction, &invWorld);
		D3DXVec3Normalize(&direction, &direction);
	}

	ray->Position = tempStart;
	ray->Direction = direction;

	



}
void Viewport::GetRayMouse(D3DXVECTOR3 Mouse ,Ray * ray, D3DXVECTOR3 & start, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{
	D3DXVECTOR2 screenSize;
	screenSize.x = width;
	screenSize.y = height;

	D3DXVECTOR3 mouse = Mouse;


	D3DXVECTOR2 point;
	//Inv Viewport , ȭ���� -1,-1 ~ 1,1 �� �������
	{
		point.x = ((2.0f * mouse.x) / screenSize.x) - 1.0f;
		point.y = (((2.0f * mouse.y) / screenSize.y) - 1.0f) * -1.0f;
	}



	//Inv Projection, 
	{
		point.x = point.x / projection._11;
		point.y = point.y / projection._22;
	}




	D3DXVECTOR3 direction;
	//Inv View
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);



		D3DXVec3TransformNormal(&direction, &D3DXVECTOR3(point.x, point.y, 1), &invView);
		D3DXVec3Normalize(&direction, &direction);
	}

	//Inv World
	D3DXVECTOR3 tempStart;
	{
		D3DXMATRIX invWorld;
		D3DXMatrixInverse(&invWorld, NULL, &world);

		// �� ������ ��ġ�� ������Ŀ����� ī�޶���ġ�� World�� ī�޶� ��ġ�� ��ȯ
		D3DXVec3TransformCoord(&tempStart, &start, &invWorld);


		D3DXVec3TransformNormal(&direction, &direction, &invWorld);
		D3DXVec3Normalize(&direction, &direction);
	}

	ray->Position = tempStart;
	ray->Direction = direction;

}


void Viewport::Get2DPos(D3DXVECTOR2& Pos2D, D3DXVECTOR3 Position, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{


	// ���⼭���� 3D �� �ٽ� 2D�� ��ȯ�� �ϸ� �ᱹ�� �ٽ� �ý��۸��콺 �����ǳ����µ�
	
	// Position�� �� ������ �������� ���ϴ°�

	D3DXVECTOR3 tempPosition;
	{
		// World
		D3DXMATRIX World = world;
		// �������� ���庯ȯ, ��������̸� �Ȱ���., ���� ������ ��������� �Ǿ���
		D3DXVec3TransformCoord(&tempPosition, &Position, &world);
		// ���� �� ����� 0.01�� ���ؼ� ��������

	}

	D3DXMATRIX matVP; // �������� V * P
	D3DXMatrixMultiply(&matVP, &view, &projection);

	D3DXVECTOR3 vPos;
	D3DXVec3TransformCoord(&vPos, &tempPosition, &matVP);

	D3DXVECTOR2 screenSize;
	screenSize.x = width;
	screenSize.y = height;


	D3DXVECTOR3 mouse; // �ñ������� ���ϰ� ���� 2D ��ǥ

	{

		mouse.x = screenSize.x * (vPos.x + 1.0f)  / 2.0f;
		mouse.y = screenSize.y * (1.0f - (vPos.y + 1) * 0.5f);
		mouse.z = 0;

	}


	Pos2D.x = mouse.x;
	Pos2D.y = mouse.y;
}

void Viewport::RSSetViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}
