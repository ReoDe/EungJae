#pragma once

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update() = 0;

public:
	void Position(float x, float y, float z);
	void Position(D3DXVECTOR3& vec);
	void Position(D3DXVECTOR3* vec);

	void Rotation(float x, float y);
	void Rotation(D3DXVECTOR2& vec);
	void Rotation(D3DXVECTOR2* vec);
		
	void RotationDegree(float x, float y);
	void RotationDegree(D3DXVECTOR2& vec);
	void RotationDegree(D3DXVECTOR2* vec);

	void Matrix(D3DXMATRIX* mat);

	D3DXVECTOR3 Forward() { return forward; }
	D3DXVECTOR3 Right() { return right; }
	D3DXVECTOR3 Up() { return up; }
	

protected:
	virtual void Move();
	virtual void Rotation();
	virtual void View();

private:
	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;

	D3DXVECTOR3 forward;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;

	D3DXMATRIX matRotation;
	D3DXMATRIX matView;
};