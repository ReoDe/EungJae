#pragma once

class CsResource
{
public:
	static void CreateRawBuffer(UINT byteWidth, void* initData, ID3D11Buffer** buffer);
	static void CreateStructuredBuffer(UINT stride, UINT count, void* initData, ID3D11Buffer** buffer);

	static void CreateSrv(ID3D11Buffer* buffer, ID3D11ShaderResourceView** srv);
	static void CreateUav(ID3D11Buffer* buffer, ID3D11UnorderedAccessView** uav);

	static ID3D11Buffer* CreateAndCopyBuffer(ID3D11Buffer* src);
};