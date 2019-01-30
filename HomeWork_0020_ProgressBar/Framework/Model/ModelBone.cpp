#include "framework.h"
#include "ModelBone.h"

ModelBone::ModelBone()
	:parent(NULL)
{

}

ModelBone::~ModelBone()
{
}

void ModelBone::Copy(ModelBone ** clone)
{
	ModelBone* bone = new ModelBone();
	bone->index = index;
	bone->name = name;
	bone->parentIndex = parentIndex;
	bone->local = local;
	bone->global = global;

	*clone = bone;
}
