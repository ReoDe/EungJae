#pragma once

class TestRect : public Execute
{
public:
	TestRect();
	~TestRect();

	void Update() override;
	
	void PreRender() override;
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}


private:


	Shader* shader;
	ID3D11Buffer* vertexBuffer;
	Texture* texture;
};