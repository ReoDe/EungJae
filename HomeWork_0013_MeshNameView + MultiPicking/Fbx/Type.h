#pragma once
#include "stdafx.h"

namespace Fbx
{
	struct FbxMaterial
	{
		string Name;
		
		string DiffuseFile;
		D3DXCOLOR Diffuse;
		
		string SpecularFile;
		D3DXCOLOR Specular;
		float SpecularExp;

		string NormalFile;
	};

	struct FbxBoneData
	{
		int Index;
		string Name;

		int Parent;

		D3DXMATRIX LocalTransform;
		D3DXMATRIX GlobalTransform;
	};

	struct FbxMeshPartData
	{
		string MaterialName;

		vector<VertexTextureNormal> Vertices;
		vector<UINT> Indices;
	};

	struct FbxVertex
	{
		int ControlPoint;
		string MaterialName;

		VertexTextureNormal Vertex;
	};

	struct FbxMeshData
	{
		string Name;
		int ParentBone;

		FbxMesh* Mesh;

		vector<FbxVertex *> Vertices;
		vector<FbxMeshPartData *> MeshParts;
	};
}