#pragma once


class DrawTriangle : public Execute
{
public:
	DrawTriangle(ExecuteValues* values);
	~DrawTriangle();

	void Update() override {}//override의미는없지만 써주는게좋음
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}
private:
	Shader * shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;

	Vertex* vertices;
};