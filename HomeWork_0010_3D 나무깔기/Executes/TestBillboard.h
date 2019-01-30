#pragma once

class TestBillboard : public Execute
{
public:
	TestBillboard(ExecuteValues* values);
	~TestBillboard();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	D3DXVECTOR3 position;
	Material * terrainMaterial;
	class Terrain* terrain;


};