#include "framework.h"
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

//void Viewport::GetRay(Ray * ray, D3DXVECTOR3 & start, D3DXMATRIX & world, D3DXMATRIX & view, D3DXMATRIX & projection)
//{
//	D3DXVECTOR2 screenSize;
//	screenSize.x = width;
//	screenSize.y = height;
//
//	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();
//
//
//	D3DXVECTOR2 point;
//	//Inv Viewport
//	{
//		point.x = ((2.0f * mouse.x) / screenSize.x) - 1.0f;
//		point.y = (((2.0f * mouse.y) / screenSize.y) - 1.0f) * -1.0f;
//	}
//
//	//Inv Projection
//	{
//		point.x = point.x / projection._11;
//		point.y = point.y / projection._22;
//	}
//
//	D3DXVECTOR3 direction;
//	//Inv View
//	{
//		D3DXMATRIX invView;
//		D3DXMatrixInverse(&invView, NULL, &view);
//
//		D3DXVec3TransformNormal(&direction, &D3DXVECTOR3(point.x, point.y, 1), &invView);
//		D3DXVec3Normalize(&direction, &direction);
//	}
//
//	//Inv World
//	D3DXVECTOR3 tempStart;
//	{
//		D3DXMATRIX invWorld;
//		D3DXMatrixInverse(&invWorld, NULL, &world);
//
//		D3DXVec3TransformCoord(&tempStart, &start, &invWorld);
//		D3DXVec3TransformNormal(&direction, &direction, &invWorld);
//		D3DXVec3Normalize(&direction, &direction);
//	}
//
//	ray->Position = tempStart;
//	ray->Direction = direction;
//}

void Viewport::RSSetViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}
