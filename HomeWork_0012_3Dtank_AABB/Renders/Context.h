#pragma once

struct GlobalLight
{
	D3DXVECTOR3 Direction = D3DXVECTOR3(-1,-1,-1);
	D3DXCOLOR Color = D3DXCOLOR(1,1,1,1);
};

class Context
{
public:
	static void Create();
	static void Delete();

public:
	static class Perspective* GetPerspective();
	static class Viewport* GetViewport();
	static class Camera* GetMainCamera();
	static GlobalLight* GetGlobalLight();

	static void SetValues(class Shader* shader);

private:
	static class Perspective* perspective;
	static class Viewport* viewport;
	static class Camera* mainCamera;
	static GlobalLight* globalLight;
};