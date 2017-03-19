#pragma once

#include"d3dUtil.h"
#include"GlobleVariable.h"


class Camera
{
public:
	Camera();
	~Camera();

	XMMATRIX InverseTranspose(CXMMATRIX M);
	void     SetLen(float fovY, float aspect, float zn, float zf);
	void UpdateViewMatrix();
	void SetLookAt(XMFLOAT3& pos, XMFLOAT3& target, XMFLOAT3& up);

	XMMATRIX GetProj();
	XMMATRIX GetView();
	XMMATRIX GetViewProj()
	{
		return XMMatrixMultiply(GetView(), GetProj());
	}
	void SetPosition(XMFLOAT3& pos);
	XMFLOAT3 GetPosition()
	{
		return mPosition;
	}
	XMFLOAT3 GetLook()
	{
		return mLook;
	}
	XMFLOAT3 GetRight()
	{
		return mRight;
	}
	XMFLOAT3 GetUp()
	{
		return mUp;
	}
	void HideMouse(HWND hwnd);
protected:
	XMFLOAT3   mPosition;
	XMFLOAT3   mLook;
	XMFLOAT3   mUp;
	XMFLOAT3   mRight;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
}; 
class FirstPersonCamera :public Camera
{
public:
	void AlongLook(float dt);
	void AlongUp(float dt);
	void AlongWorldUp(float dt);
	void AlongRight(float dt);

	void RotY(float angle);//扫视
	void RotRight(float angle);//俯仰
};
class ThirdPersonCamera :public Camera
{
public:
	ThirdPersonCamera();

//	float GetRadius(){ return mRadius; }
	void RotObjectY(XMFLOAT3& pos,float angle);// 绕对象左右转
	void RotObjectAxis(XMFLOAT3& pos,float angle); // 绕对象上下转
	void Scale(XMFLOAT3& pos,float fParam);
	void UpdatePosition(XMFLOAT3& pos);
	bool Init(XMFLOAT3& pos,XMFLOAT3& target, float maxR, float minR);
private:
//	float mRadius;
	XMFLOAT3 mTarget;
	float mMaxRadius;
	float mMinRadius;

	float mDy;//鼠标的累积偏移量
};