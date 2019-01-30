#pragma once

class TestModel : public Execute
{
public:
	TestModel();
	~TestModel();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:

	class Sky* sky;

	wstring floor;
	wstring stone;
	wstring brick;


	GameModel* tank;


	Material* sphereMaterial;
	MeshSphere* sphere[10];
	
	Material* cylinderMaterial;
	MeshCylinder* cylinder[10];

	Material* boxMaterial;
	MeshCube* box;

	Material* planeMaterial;
	MeshGrid* plane;



};