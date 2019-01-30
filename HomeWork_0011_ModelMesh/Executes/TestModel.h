#pragma once

enum ModelName
{
	Tank_geo = 0
};

class TestModel : public Execute
{
public:
	TestModel(ExecuteValues* values);
	~TestModel();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	Shader * shader;
	class GameModel* tank;

	UINT Index;

	//


	D3DXMATRIX turretTransform[12];

	D3DXVECTOR3 location[12];
	D3DXVECTOR3 size[12];
	D3DXVECTOR3 rotation[12];

	int pickingIndex;



};