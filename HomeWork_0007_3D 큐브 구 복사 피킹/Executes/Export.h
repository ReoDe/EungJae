#pragma once

class Export : public Execute
{
public:
	Export(ExecuteValues* values);
	~Export();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	
};