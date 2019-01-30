#pragma once

class Execute
{
public:
	Execute()
	{

	}
	virtual ~Execute(){ }

	virtual void Update() = 0;

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	virtual void ResizeScreen() = 0;
};