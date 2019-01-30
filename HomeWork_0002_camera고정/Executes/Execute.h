#pragma once

#include "../Viewer/Perspective.h"
#include "../Viewer/Viewport.h"
#include "../Viewer/Camera.h"

struct ExecuteValues
{
	class ViewProjectionBuffer* ViewProjection;

	class Perspective* Perspective;
	class Viewport* Viewport;

	class Camera* MainCamera;
	class Camera* PixedCamera;
};

class Execute
{
public:
	Execute(ExecuteValues* values)
		: values(values)
	{

	}
	virtual ~Execute(){ }

	virtual void Update() = 0;

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	virtual void ResizeScreen() = 0;

	virtual D3DXVECTOR3 XYZ() = 0;
private:
	ExecuteValues* values;
};