#pragma once

class TestBrush : public Execute
{
public:
	TestBrush(ExecuteValues* values);
	~TestBrush();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	Material * terrainMaterial;
	class Terrain* terrain;

	D3DXVECTOR3 position;

};