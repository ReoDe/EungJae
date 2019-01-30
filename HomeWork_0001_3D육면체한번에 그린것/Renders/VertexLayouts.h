#pragma once

//=======================================================================================

struct Vertex
{
	Vertex()
		: Position(0, 0, 0) {}

	D3DXVECTOR3 Position;
};

//=======================================================================================

struct VertexColor
{
	VertexColor()
		: Position(0, 0, 0)
		, Color(0, 0, 0, 1) {}

	D3DXVECTOR3	Position;
	D3DXCOLOR Color;
};

//=======================================================================================

struct VertexColorNormal
{
	VertexColorNormal()
		: Position(0, 0, 0)
		, Color(0, 0, 0, 1)
		, Normal(0, 0, 0) {}

	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
	D3DXVECTOR3 Normal;
};

//=======================================================================================

struct VertexTexture
{
	VertexTexture()
		: Position(0, 0, 0)
		, Uv(0, 0) {}



	D3DXVECTOR3	Position;
	D3DXVECTOR2	Uv; // 정점 내에서의 2D 좌표, 2D로 바뀔때 쓰는것
	
};

//=======================================================================================

struct VertexTextureNormal
{
	VertexTextureNormal()
		: Position(0, 0, 0)
		, Uv(0, 0)
		, Normal(0, 0, 0) {}

	D3DXVECTOR3 Position;
	D3DXVECTOR2 Uv;
	D3DXVECTOR3 Normal;
};

//=======================================================================================

struct VertexColorTextureNormal
{
	VertexColorTextureNormal()
		: Position(0, 0, 0)
		, Color(0, 0, 0, 1)
		, Uv(0, 0)
		, Normal(0, 0, 0) {}

	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
	D3DXVECTOR2 Uv;
	D3DXVECTOR3 Normal;
};

//=======================================================================================

struct VertexTextureNormalBlend
{
	VertexTextureNormalBlend()
		: Position(0, 0, 0)
		, Uv(0, 0)
		, Normal(0, 0, 0)
		, BlendIndices(0, 0, 0, 0)
		, BlendWeights(0, 0, 0, 0) {}

	D3DXVECTOR3 Position;
	D3DXVECTOR2 Uv;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 BlendIndices;
	D3DXVECTOR4 BlendWeights;
};

//=======================================================================================

struct VertexTextureNormalTangent
{
	VertexTextureNormalTangent()
		: Position(0, 0, 0)
		, Uv(0, 0)
		, Normal(0, 0, 0)
		, Tangent(0, 0, 0)
	{}

	D3DXVECTOR3	Position;
	D3DXVECTOR2	Uv;
	D3DXVECTOR3	Normal;
	D3DXVECTOR3 Tangent;
};


struct VertexTextureColor2
{
	VertexTextureColor2()
		: Position(0, 0, 0)
		, Uv(0, 0)
		, Color(0, 0, 0, 1) 
		, Color2(0, 0, 0, 1)
	{
		
	}

	D3DXVECTOR3 Position;
	D3DXVECTOR2 Uv;
	D3DXCOLOR Color;
	D3DXCOLOR Color2;
};