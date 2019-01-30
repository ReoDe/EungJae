#pragma once

class Test : public Execute
{
public:
	Test();
	~Test();

	void Update() override;
	
	void PreRender() override {}
	void Render() override;
	void PostRender() override;

	void ResizeScreen() override {}

	void ClickMouse(int Ratio);
private:

	class Sky* sky;


	GameModel* tank;
	D3DXVECTOR3 tankPos;
	D3DXVECTOR2 MyPos;
	class Model* tankModel;

	Ray ray;

	wstring temp;
	D3DXVECTOR3 tempVec;


	// ��Ƽ ��ŷ���

	/*
	
	1. ù Ŭ�������� �������� ��ǥȹ��
	2. �� ��ǥ���� ���μ��� 10���� ���ڷ� �� 100���� ray����
	3. ���� �����ϰ� ��ŷ�õ�
	4. ��ŷ�õ� �� ���ÿ� �� �𵨸޽� �̸��� ���Ϳ� ����
	5. ���Ϳ� ����� �̸����� ���


	*/

	D3DXVECTOR3 FirstClick;
	D3DXVECTOR3 EndClick;

	//struct MultiPicking
	//{
	//	wstring Name; // Ȥ�� �߰��Ҽ��� ������
	//};
	Ray Multiray;
	vector<wstring> multiPicking;
	int PickingNum;
	vector<D3DXVECTOR3> test;

};