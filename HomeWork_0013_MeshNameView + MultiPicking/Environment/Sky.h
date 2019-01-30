#pragma once

class Sky
{
public:
	Sky();
	~Sky();

	void Update();
	void Render();

private:
	

	Material* material;
	MeshSphere * sphere;

	D3DXCOLOR apex, center;
	float height;

	//Data.Center = D3DXCOLOR(0xFF0080FF);
	//Data.Apex = D3DXCOLOR(0xFF9BCDFF);
	//Data.height = 4.5f;



};
