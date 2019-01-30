#include "framework.h"
#include "Progress.h"

Progress* Progress::instance = NULL;

Progress * Progress::Get()
{
	return instance;
}

void Progress::Create()
{
	assert(instance == NULL);

	instance = new Progress();
}

void Progress::Delete()
{
	SAFE_DELETE(instance);
}

void Progress::Render()
{
	ImGui::ProgressBar(rate);
}

Progress::Progress()
	: rate(0.0f)
{

}

Progress::~Progress()
{

}
