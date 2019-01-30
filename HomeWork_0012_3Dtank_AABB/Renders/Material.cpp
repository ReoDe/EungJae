#include "stdafx.h"
#include "Material.h"

Material::Material()
	: shader(NULL)
	, diffuseMap(NULL), specularMap(NULL), normalMap(NULL)
	, ambient(0, 0, 0, 1), diffuse(0, 0, 0, 1), specular(0, 0, 0, 1), shininess(0)
	, bShaderDelete(false)
{
	
}

Material::Material(wstring shaderFile)
	: diffuseMap(NULL), specularMap(NULL), normalMap(NULL)
	, ambient(0, 0, 0, 1), diffuse(0, 0, 0, 1), specular(0, 0, 0, 1), shininess(0)
{
	assert(shaderFile.length() > 0);

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

void Material::SetShaderValues()
{
	if (shader == NULL) return;

	
	Context::SetValues(shader);

	shader->AsVector("Ambient")->SetFloatVector(ambient);
	shader->AsVector("Diffuse")->SetFloatVector(diffuse);
	shader->AsVector("Specular")->SetFloatVector(specular);
	shader->AsScalar("Shininess")->SetFloat(shininess);

	if (diffuseMap != NULL)
		shader->AsShaderResource("DiffuseMap")->SetResource(diffuseMap->SRV());

	if (specularMap != NULL)
		shader->AsShaderResource("SpecularMap")->SetResource(specularMap->SRV());
	
	if (normalMap != NULL)
		shader->AsShaderResource("NormalMap")->SetResource(normalMap->SRV());
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
