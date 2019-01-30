#pragma once

class Window
{
public:
	static WPARAM Run(class IExecute* main);

private:
	static void Create();
	static void Destroy();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	static void ProgressRender();
	static void MainRender();

private:
	static class IExecute* mainExecute;

	static bool bInitialize;
	static mutex* criticalSection;
};