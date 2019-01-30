#pragma once


class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update() = 0;
	virtual void Render() = 0;
public:

	void Position(float x, float y, float z);

	void Position(D3DXVECTOR3& vec); // 복사 안되도록
	void Position3(D3DXVECTOR3& vec); // 복사 안되도록
	//D3DXVECTOR3 Position() { return position; }
	void Position(D3DXVECTOR3* vec);

	void Rotation(float x, float y);// 여기 질문
	void Rotation(D3DXVECTOR2& vec);
	void Rotation(D3DXVECTOR2* vec);
	//D3DXVECTOR2 Rotation() { return rotation; }

	void RotationDegree(float x, float y);
	void RotationDegree(D3DXVECTOR2& vec); // 참조
	void RotationDegree(D3DXVECTOR2* vec); // 리턴받는것

	void Matrix(D3DXMATRIX* mat);

	void At(float x, float y, float z);
	void At(D3DXVECTOR3& vec);
	void At(D3DXVECTOR3* vec);


	D3DXVECTOR3 Forward() { return forward; }
	D3DXVECTOR3 Right() { return right; }
	D3DXVECTOR3 Up() { return up; }

	void Setforward(D3DXVECTOR3 _forward)
	{
		forward = _forward;
	}
	void Setat(D3DXVECTOR3 _at)
	{
		at = _at;
	}
	


protected:
	virtual void Move();
	virtual void Rotation();

	virtual void View();

	virtual void View3();


private:

	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;

	D3DXVECTOR3 forward;
	D3DXVECTOR3 right; // cross(up, z)의 값을 미리 계산한값
	D3DXVECTOR3 up;
	D3DXVECTOR3 at;

	D3DXMATRIX matRotation;
	D3DXMATRIX matView; // 실제 쓸 값

};
