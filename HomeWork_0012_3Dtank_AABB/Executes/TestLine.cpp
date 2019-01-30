#include "stdafx.h"
#include "TestLine.h"

TestLine::TestLine()
{
	boundingBox = new BBox(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	boxLine = new DebugLine();
	
	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	boxLine->Draw(W, boundingBox);
}

TestLine::~TestLine()
{
	SAFE_DELETE(boundingBox);
	SAFE_DELETE(boxLine);
}

void TestLine::Update()
{
	
}

void TestLine::Render()
{
	boxLine->Render();
}

