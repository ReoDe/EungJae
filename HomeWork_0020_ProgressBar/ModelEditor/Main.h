#pragma once
#include "Systems/IExecute.h"

class Main : public IExecute
{
public:
	void Initialize() override;
	void Ready() override;
	void Destroy() override;
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void ResizeScreen() override;

private:
	void Push(IExecute* execute);

private:
	vector<IExecute *> executes;
};