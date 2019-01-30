#pragma once

class Progress
{
public:
	static Progress* Get();

	static void Create();
	static void Delete();

	void Render();

	void Rate(float val) { rate = val; }

private:
	static Progress* instance;

	Progress();
	~Progress();

private:
	float rate;
};