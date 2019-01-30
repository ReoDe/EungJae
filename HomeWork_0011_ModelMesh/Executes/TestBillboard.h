#pragma once

class TestBillboard : public Execute
{
public:
	TestBillboard(ExecuteValues* values);
	~TestBillboard();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	D3DXVECTOR3 Position;
	class Sky* sky;


	Material * terrainMaterial;
	class Terrain* terrain;

	Material* billboardMaterial;
	vector<MeshQuad*> billboards;
	
//private:
//	class Buffer : public ShaderBuffer
//	{
//
//	public:
//
//		Buffer() : ShaderBuffer(&Data, sizeof(Data))
//		{
//			D3DXMatrixIdentity(&Data.Billboard);
//		}
//		// 행 우선으로 바꿈, z축 반대라
//		void Billboard(D3DXMATRIX& mat)
//		{
//			D3DXMatrixTranspose(&mat, &mat);
//		}
//
//	private:
//		struct Struct
//		{
//			D3DXMATRIX Billboard;
//			
//		}Data;
//	};
//
//	Buffer* buffer;

};