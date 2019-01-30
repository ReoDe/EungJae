#pragma once
#include "stdafx.h"

class ILine
{
public:
	virtual void GetLine(D3DXMATRIX& world, vector<D3DXVECTOR3>& lines) = 0;
};