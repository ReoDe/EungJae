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
	Shader* shader;
	GameModel* buuny;

};