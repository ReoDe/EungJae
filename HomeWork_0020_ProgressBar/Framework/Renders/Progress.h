#pragma once

class Progress
{
public:

	static Progress* Get();

	static void Create();
	static void Delete();

	void Render();
	void Rate(float val) { rate = val; }
	void RateWidth(float val);

private:
	static Progress* instance;

	Progress();
	~Progress();

	

private:
	float rate;

	ID3D11ShaderResourceView* srv;
	class Render2D* render2D;
	ID3D11ShaderResourceView* barSrv;
	class Render2D* Bar;

	float Width; // ·Îµù¹Ù
};