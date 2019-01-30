#pragma once

class ModelBone
{
public:
	friend class Model;
	friend class Models;

private:
	ModelBone();
	~ModelBone();

public:
	int Index() { return index; }

	int ParentIndex() { return parentIndex; }
	ModelBone* Parent() { return parent; }

	UINT ChildCount() { return childs.size(); }
	ModelBone* Child(UINT index) { return childs[index]; }
	vector<ModelBone *>& GetChilds() { return childs; }

	wstring Name() { return name; }

	D3DXMATRIX& Local() { return local; }
	void Local(D3DXMATRIX& matrix) { local = matrix; }

	D3DXMATRIX& Global() { return global; }
	void Global(D3DXMATRIX& matrix) { global = matrix; }

public:
	void Copy(ModelBone** clone);

private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	vector<ModelBone *> childs;

	D3DXMATRIX local;
	D3DXMATRIX global;
};