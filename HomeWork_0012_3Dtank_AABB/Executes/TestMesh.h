#pragma once

class TestMesh : public Execute
{
public:
	TestMesh();
	~TestMesh();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	//Material* material;
	//MeshQuad* quad;

	ID3D11Buffer* vertexBuffer;
	Shader* shader;

	Texture* texture;

};