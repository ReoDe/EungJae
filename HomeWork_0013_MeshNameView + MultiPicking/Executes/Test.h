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


	// 멀티 피킹방법

	/*
	
	1. 첫 클릭지점과 떼는지점 좌표획득
	2. 두 좌표사이 가로세로 10개씩 격자로 총 100개의 ray생성
	3. 레이 생성하고 피킹시도
	4. 피킹시도 와 동시에 각 모델메쉬 이름을 벡터에 저장
	5. 벡터에 저장된 이름들을 출력


	*/

	D3DXVECTOR3 FirstClick;
	D3DXVECTOR3 EndClick;

	//struct MultiPicking
	//{
	//	wstring Name; // 혹시 추가할수도 있으니
	//};
	Ray Multiray;
	vector<wstring> multiPicking;
	int PickingNum;
	vector<D3DXVECTOR3> test;

};