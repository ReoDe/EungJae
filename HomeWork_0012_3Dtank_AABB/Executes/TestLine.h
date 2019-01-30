#pragma once

class TestLine : public Execute
{
public:
	TestLine();
	~TestLine();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	DebugLine* boxLine;
	BBox* boundingBox;
};