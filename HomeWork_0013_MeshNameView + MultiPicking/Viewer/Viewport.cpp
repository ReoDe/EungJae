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
	//Inv Viewport , 화면을 -1,-1 ~ 1,1 로 만들어줌
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

		// 이 과정을 거치면 정방행렬에서의 카메라위치가 World의 카메라 위치로 변환
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
	//Inv Viewport , 화면을 -1,-1 ~ 1,1 로 만들어줌
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

		// 이 과정을 거치면 정방행렬에서의 카메라위치가 World의 카메라 위치로 변환
		D3DXVec3TransformCoord(&tempStart, &start, &invWorld);


		D3DXVec3TransformNormal(&direction, &direction, &invWorld);
		D3DXVec3Normalize(&direction, &direction);
	}

	ray->Position = tempStart;
	ray->Direction = direction;

}


void Viewport::Get2DPos(D3DXVECTOR2& Pos2D, D3DXVECTOR3 Position, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
{


	// 여기서부턴 3D 를 다시 2D로 변환을 하면 결국에 다시 시스템마우스 포지션나오는데
	
	// Position은 각 월드의 포지션을 말하는것

	D3DXVECTOR3 tempPosition;
	{
		// World
		D3DXMATRIX World = world;
		// 포지션을 월드변환, 정방행렬이면 똑같다., 현재 기준은 정방행렬이 되었음
		D3DXVec3TransformCoord(&tempPosition, &Position, &world);
		// 만약 모델 월드면 0.01을 곱해서 나오겠지

	}

	D3DXMATRIX matVP; // 역순으로 V * P
	D3DXMatrixMultiply(&matVP, &view, &projection);

	D3DXVECTOR3 vPos;
	D3DXVec3TransformCoord(&vPos, &tempPosition, &matVP);

	D3DXVECTOR2 screenSize;
	screenSize.x = width;
	screenSize.y = height;


	D3DXVECTOR3 mouse; // 궁극적으로 구하고 싶은 2D 좌표

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
