#pragma once


class TestOBB : public Execute
{
public:
	TestOBB(ExecuteValues* values);
	~TestOBB();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}
	
private:

	//

	Shader * shader;
	class GameModel* tank;

	MeshCubeParts* CubeParts;


	wstring temp;

	Ray ray;


	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;

	D3DXMATRIX Bone;

	struct OBB_Ray
	{
		D3DXVECTOR3 ray_origin; // ray.Ω√¿€
		D3DXVECTOR3 ray_direction; // ray ≥°
		D3DXVECTOR3 aabb_min; // 
		D3DXVECTOR3 aabb_max;
	};

	class OBB* Obb;

	bool obbcheck;

	
};

