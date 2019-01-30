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


private:
	ExecuteValues* values;
};