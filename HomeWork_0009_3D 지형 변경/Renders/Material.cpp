#include "stdafx.h"
#include "Material.h"

Material::Material()
	: shader(NULL)
	, diffuseMap(NULL), specularMap(NULL), normalMap(NULL)
	, bShaderDelete(false)
{
	buffer = new Buffer();
}

Material::Material(wstring shaderFile)
	: diffuseMap(NULL), specularMap(NULL), normalMap(NULL)
{
	assert(shaderFile.length() > 0);

	buffer = new Buffer();

	bShaderDelete = true;
	shader = new Shader(shaderFile);
}

Material::~Material()
{
	if (bShaderDelete == true)
		SAFE_DELETE(shader);

	SAFE_DELETE(diffuseMap);
	SAFE_DELETE(specularMap);
	SAFE_DELETE(normalMap);
}

void Material::SetShader(string file)
{
	SetShader(String::ToWString(file));
}

void Material::SetShader(wstring file)
{
	if (bShaderDelete == true)
		SAFE_DELETE(shader);

	bShaderDelete = false;
	if (file.length() > 0)
	{
		shader = new Shader(file);
		bShaderDelete = true;
	}
}

void Material::SetShader(Shader * shader)
{
	if (bShaderDelete == true)
		SAFE_DELETE(shader);

	this->shader = shader;
	bShaderDelete = false;
}

void Material::SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetDiffuseMap(String::ToWString(file), info);
}

void Material::SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(diffuseMap);

	diffuseMap = new Texture(file, info);
}

void Material::SetSpecularMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetSpecularMap(String::ToWString(file), info);
}

void Material::SetSpecularMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(specularMap);

	specularMap = new Texture(file, info);
}

void Material::SetNormalMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetNormalMap(String::ToWString(file), info);
}

void Material::SetNormalMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(normalMap);

	normalMap = new Texture(file, info);
}

void Material::PSSetBuffer()
{
	if (shader != NULL)
		shader->Render();


	UINT slot = 0;
	if (diffuseMap != NULL)
	{
		diffuseMap->SetShaderResource(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
	}


	slot = 1;
	if (specularMap != NULL)
	{
		specularMap->SetShaderResource(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
	}

	slot = 2;
	if (normalMap != NULL)
	{
		normalMap->SetShaderResource(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
	}

	buffer->SetPSBuffer(1);
}

void Material::Copy(Material ** clone)
{
	Material* material = new Material();
	material->name = name;

	if (shader != NULL)
		material->SetShader(shader);

	material->SetDiffuse(*GetDiffuse());
	material->SetSpecular(*GetSpecular());
	material->SetShininess(*GetShininess());

	if (diffuseMap != NULL)
		material->SetDiffuseMap(diffuseMap->GetFile());

	if (specularMap != NULL)
		material->SetSpecularMap(specularMap->GetFile());

	if (normalMap != NULL)
		material->SetNormalMap(normalMap->GetFile());

	*clone = material;
}
