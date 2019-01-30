#pragma once

class Shader
{
public:
	Shader(wstring shaderFile, string vsName = "VS", string psName = "PS");
	~Shader();

	wstring GetFile() { return shaderFile; }

	void Render();

private:
	void CreateVertexShader();
	void CreatePixelShader();
	void CheckShaderError(HRESULT hr, ID3DBlob* error);
	void CreateInputLayout();

private:
	wstring shaderFile;
	string vsName;
	string psName;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;
	ID3DBlob* pixelBlob;

	ID3D11ShaderReflection* reflection;
};