#pragma once

class Material
{
public:
	Material();
	Material(wstring shaderFile);
	~Material();

	void Name(wstring val) { name = val; }
	wstring Name() { return name; }

	Shader* GetShader() { return shader; }
	void SetShader(string file);
	void SetShader(wstring file);
	void SetShader(Shader* shader);

	D3DXCOLOR* GetDiffuse() { return &diffuse; }
	void SetDiffuse(D3DXCOLOR& color) { diffuse = color; }
	void SetDiffuse(float r, float g, float b, float a = 1.0f)
	{
		diffuse = D3DXCOLOR(r, g, b, a);
	}

	D3DXCOLOR* GetSpecular() { return &specular; }
	void SetSpecular(D3DXCOLOR& color) { specular = color; }
	void SetSpecular(float r, float g, float b, float a = 1.0f)
	{
		specular = D3DXCOLOR(r, g, b, a);
	}

	float* GetShininess() { return &shininess; }
	void SetShininess(float val) { shininess = val; }


	Texture* GetDiffuseMap() { return diffuseMap; }
	void SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

	Texture* GetSpecularMap() { return specularMap; }
	void SetSpecularMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetSpecularMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

	Texture* GetNormalMap() { return normalMap; }
	void SetNormalMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetNormalMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

	void SetShaderValues();

	void Copy(Material** clone);

private:
	wstring name;

	bool bShaderDelete;
	Shader* shader;

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	float shininess;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;
};