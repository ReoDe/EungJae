#pragma once

class TestEffect : public Execute
{
public:
	TestEffect();
	~TestEffect();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	Shader* shader;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Vertex* vertices;
	UINT* indices;
};