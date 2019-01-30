#pragma once

struct CubeStruct
{

	Material* cubeMaterial;
	MeshCube* cubeMesh;
	BBox* cubeBounding;
	bool cubeTest;
	DebugLine* cubeLine;

	int Number;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Rotation;


};

struct SphereStruct
{

	Material* sphereMaterial;
	MeshSphere* sphereMesh;
	BSphere* sphereBounding;
	bool sphereTest;
	DebugLine* sphereLine;

	int Number;

	float radius;
	D3DXVECTOR3 Center;

};

class Draw : public Execute
{
public:
	Draw(ExecuteValues* values);
	~Draw();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}


	void InsertCube();

	void InsertSphere();

private:
	RasterizerState* rasterizerState[2];


	Ray* ray;

	//Å¥ºê
	vector<CubeStruct*> Cube;
	int CubeSelect;

	//±¸
	vector<SphereStruct*> Sphere;
	int SphereSelect;

};