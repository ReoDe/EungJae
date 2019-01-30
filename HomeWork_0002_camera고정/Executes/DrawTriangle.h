#pragma once


class DrawTriangle : public Execute
{
public:
	DrawTriangle(ExecuteValues* values);
	~DrawTriangle();

	void Update() override {}//override�ǹ̴¾����� ���ִ°�����
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