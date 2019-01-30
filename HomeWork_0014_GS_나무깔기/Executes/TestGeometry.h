#pragma once

class TestGeometry : public Execute
{
public:
	TestGeometry();
	~TestGeometry();

	void Update() override;
	
	void PreRender() override;
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

	void CreateVertex();


private:
	Material * terrainMaterial;
	class Terrain* terrain;


	Shader* shader;
	Texture* texture;

	//과제

	/*
	1. update에 클릭해서 하나씩 추가
	
	*/

	D3DXVECTOR3 position;
	int WindPower;
	int WindDirection;

private:


	struct Vertexes
	{
		Vertexes( class Terrain* terrain, Shader* shader, Texture* texture, D3DXVECTOR3 position, float ClickTime);
		

	public:
		VertexSizeClick * vertex;
		ID3D11Buffer* vertexBuffer;
		Shader* shader;
		Texture* texture;
		class Terrain* terrain;
		//Ray ray;
	
		float ClickTime;
		int WindPower;
		int WindDirection;

		void SetWindPower(float windPower, float windDirection)
		{
			WindPower = windPower;
			WindDirection = windDirection;
		}
		void VertexUpdate();
		void VertexRender();

	};

	vector<Vertexes*> Vertex;


};