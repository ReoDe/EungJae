#include "stdafx.h"
#include "Export.h"
#include "../Fbx/Exporter.h"

Export::Export(ExecuteValues * values)
	: Execute(values)
{
	Fbx::Exporter* exporter = NULL;

	////Capsule
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Capsule.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Capsule.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Capsule.mesh");
	//SAFE_DELETE(exporter);

	////Cube
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Cube.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Cube.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Cube.mesh");
	//SAFE_DELETE(exporter);

	////Cylinder
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Cylinder.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Cylinder.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Cylinder.mesh");
	//SAFE_DELETE(exporter);
	//
	////Plane
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Plane.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Plane.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Plane.mesh");
	//SAFE_DELETE(exporter);
	//
	////Quad
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Quad.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Quad.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Quad.mesh");
	//SAFE_DELETE(exporter);

	////Sphere
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Sphere.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Sphere.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Sphere.mesh");
	//SAFE_DELETE(exporter);

	////Teapot
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Teapot.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Teapot.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Teapot.mesh");
	//SAFE_DELETE(exporter);

	////Bunny
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Bunny.fbx");
	//exporter->ExportMaterial(Models + L"/Meshes/", L"Bunny.material");
	//exporter->ExportMesh(Models + L"/Meshes/", L"Bunny.mesh");
	//SAFE_DELETE(exporter);


	exporter = new Fbx::Exporter(Assets + L"Tank/Tank.fbx");
	exporter->ExportMaterial(Models + L"Tank/", L"Tank.material");
	exporter->ExportMesh(Models + L"Tank/", L"Tank.mesh");
	SAFE_DELETE(exporter);
}

Export::~Export()
{
	
}

void Export::Update()
{
	
}

void Export::Render()
{
	
}

