#include "Framework.h"
#include "MeshCubeMinMax.h"

MeshCubeMinMax::MeshCubeMinMax(Material * material, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
	: Mesh(material), Min(Min), Max(Max)
{

}

MeshCubeMinMax::~MeshCubeMinMax()
{

}

void MeshCubeMinMax::CreateData()
{
	vector<VertexTextureNormalTangent> vertices;

	//Front
	vertices.push_back(VertexTextureNormalTangent(Min.x, Min.y, Min.z , 0, 1, 0, 0, -1, 1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Min.z , 0, 0, 0, 0, -1, 1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Min.y, Min.z , 1, 0, 0, 0, -1, 1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Max.y, Min.z , 1, 1, 0, 0, -1, 1, 0, 0));

	//Back
	vertices.push_back(VertexTextureNormalTangent(Min.x, Min.y, Max.z, 1, 1, 0, 0, 1, -1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Max.z, 0, 1, 0, 0, 1, -1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Min.y, Max.z, 0, 0, 0, 0, 1, -1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Min.z, 1, 0, 0, 0, 1, -1, 0, 0));
	//Left
	vertices.push_back(VertexTextureNormalTangent(Min.x, Min.y, Max.z, 0, 1, -1, 0, 0, 0, 0, -1));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Max.z, 0, 0, -1, 0, 0, 0, 0, -1));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Min.y, Min.z, 1, 0, -1, 0, 0, 0, 0, -1));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Min.z, 1, 1, -1, 0, 0, 0, 0, -1));

	//Right
	vertices.push_back(VertexTextureNormalTangent(Max.x, Min.y, Max.z, 0, 1, 1, 0, 0, 0, 0, 1));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Max.y, Max.z, 0, 0, 1, 0, 0, 0, 0, 1));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Min.y, Min.z, 1, 0, 1, 0, 0, 0, 0, 1));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Max.y, Min.z, 1, 1, 1, 0, 0, 0, 0, 1));

	//Top
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Min.z, 0, 1, 0, 1, 0, 1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Max.y, Max.z, 0, 0, 0, 1, 0, 1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Max.y, Min.z, 1, 0, 0, 1, 0, 1, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Max.y, Max.z, 1, 1, 0, 1, 0, 1, 0, 0));

	//Bottom
	vertices.push_back(VertexTextureNormalTangent(Min.x, Min.y, Min.z, 0, -1, 0, -1, 0, 0, 1, 1));
	vertices.push_back(VertexTextureNormalTangent(Min.x, Min.y, Max.z, 0, -1, 0, -1, 0, 0, 0, 1));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Min.y, Min.z, 0, -1, 0, -1, 0, 0, 0, 0));
	vertices.push_back(VertexTextureNormalTangent(Max.x, Min.y, Max.z, 0, -1, 0, -1, 0, 0, 1, 0));


	

	this->vertices = new VertexTextureNormalTangent[vertices.size()];
	vertexCount = vertices.size();
	copy
	(
		vertices.begin(), vertices.end(),
		stdext::checked_array_iterator<VertexTextureNormalTangent *>(this->vertices, vertexCount)
	);


	indexCount = 36;
	
	this->indices = new UINT[indexCount];
	for (int i = 0; i < 6; i++)
	{
		// 반시계
		if (i % 2 == 0) {
			indices[i * 6 + 0] = i * 4 + 0;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 1;
			indices[i * 6 + 5] = i * 4 + 3;
		}
		// 시계방향으로 그려야함 // 전부 시계방향으로 그렷다가 뒤 오른쪽 아래 법선벡터가
		// 반대라서 안보임, 그래서 방향을 반대로 그려서 벡터방향을 바꿔야함.
		// 이부분은 전에 했던 사람것 코드를 가져왔습니다. 제가한건 밑에 주석처리된 부분
		else {
			indices[i * 6 + 0] = i * 4 + 1;
			indices[i * 6 + 1] = i * 4 + 0;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 1;
			indices[i * 6 + 4] = i * 4 + 2;
			indices[i * 6 + 5] = i * 4 + 3;
		}



	}

}
