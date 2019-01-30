#pragma once

class Cube 
{
public:
	Cube(wstring name, Vertex* vertices, int width, D3DXMATRIX& mat);
	Cube(wstring name, VertexTexture* vertices, int width, D3DXMATRIX& mat);
	Cube(wstring name, VertexTextureNormal* vertices, int width, D3DXMATRIX& mat);
	//Cube()
	~Cube();
	
	void Update();
	void Render();

	void SetWorldMatrix(D3DXMATRIX& mat);
	D3DXMATRIX GetWorldMat() { return matWorld; }

	wstring GetName() { return objectName; }
	void GetVertexPosition(D3DXVECTOR3** arr);
	void GetIndices(UINT** arr);


	bool& GetVisible() { return bVisible; }
	void SetVisible(bool vs) { bVisible = vs; }

	
private:	
	void Initialize();
	void SetVertices();
	void Binding();


private:
	Shader* shader;
	WorldBuffer* worldBuffer;
	RasterizerState* rState[2];

	Vertex* vertices;
	UINT* indices;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	D3DXMATRIX matWorld;
	D3DXVECTOR3 scale;

	wstring objectName;
	int boneIndex;
	D3DXVECTOR3* object;
	int arrWidth;

	bool bVisible;
};
