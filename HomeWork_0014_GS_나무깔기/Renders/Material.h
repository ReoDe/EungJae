#pragma once

class Material
{
public:
	friend class Model;

public:
	Material();
	Material(wstring shaderFile);
	~Material();

	void Name(wstring val) { name = val; }
	wstring Name() { return name; }

	Shader* GetShader() { return shader; }
	void SetShader(string file);
	void SetShader(wstring file);

	D3DXCOLOR GetAmbient() { return color.Ambient; }
	void SetAmbient(D3DXCOLOR& color);
	void SetAmbient(float r, float g, float b, float a = 1.0f);

	D3DXCOLOR GetDiffuse() { return color.Diffuse; }
	void SetDiffuse(D3DXCOLOR& color);
	void SetDiffuse(float r, float g, float b, float a = 1.0f);

	D3DXCOLOR GetSpecular() { return color.Specular; }
	void SetSpecular(D3DXCOLOR& color);
	void SetSpecular(float r, float g, float b, float a = 1.0f);

	float GetShininess() { return color.Shininess; }
	void SetShininess(float val);


	Texture* GetDiffuseMap() { return diffuseMap; }
	void SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

	Texture* GetSpecularMap() { return specularMap; }
	void SetSpecularMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetSpecularMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

	Texture* GetNormalMap() { return normalMap; }
	void SetNormalMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetNormalMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

public:
	void Copy(Material** clone);

private:
	wstring name;
	Shader* shader;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

private:
	struct ColorDesc
	{
		D3DXCOLOR Ambient;
		D3DXCOLOR Diffuse;
		D3DXCOLOR Specular;

		float Shininess;
		float Padding[3];
	} color;
	CBuffer* buffer;
};