#include "framework.h"
#include "Fixity.h"

Fixity::Fixity()
{

}

Fixity::~Fixity()
{
}

void Fixity::Update()
{
	Rotation();
	View();
}
