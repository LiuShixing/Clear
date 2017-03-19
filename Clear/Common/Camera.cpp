#include"Camera.h"

Camera::Camera()
{
	mPosition=XMFLOAT3(0.0f,10.0f,-25.0f);

	mLook=XMFLOAT3(0.0f,0.0f,1.0f);//ָ��ԭ��
	mRight=XMFLOAT3(1.0f,0.0f,0.0f);//ָ��X��
	XMVECTOR LOOK=::XMLoadFloat3(&mLook);
	XMVECTOR RIGHT=::XMLoadFloat3(&mRight);
	XMVECTOR UP=::XMVector3Cross(LOOK,RIGHT);//����Up

	::XMStoreFloat3(&mUp,::XMVector3Normalize(UP));
}
Camera::~Camera()
{
	SetLen(0.25f*MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

void  Camera::SetLen(float fovY, float aspect, float zn, float zf)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(fovY, aspect, zn, zf);
	XMStoreFloat4x4(&mProj, P);
}
void Camera::SetLookAt(XMFLOAT3& pos, XMFLOAT3& target,XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(T,P));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(U, L));
	         U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, P);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	
}
void Camera::UpdateViewMatrix()
{
	XMVECTOR RIGHT = XMLoadFloat3(&mRight);
	XMVECTOR UP = XMLoadFloat3(&mUp);
	XMVECTOR LOOK = XMLoadFloat3(&mLook);
	XMVECTOR POS = XMLoadFloat3(&mPosition);

	LOOK=::XMVector3Normalize(LOOK);
	UP=::XMVector3Normalize(::XMVector3Cross(LOOK,RIGHT));
	RIGHT=::XMVector3Cross(UP,LOOK);//mUP��mLOOK�����ͳ�����

	float x = -XMVectorGetX(XMVector3Dot(POS, RIGHT));
	float y = -XMVectorGetX(XMVector3Dot(POS, UP));
	float z = -XMVectorGetX(XMVector3Dot(POS, LOOK));

	XMStoreFloat3(&mRight, RIGHT);
	XMStoreFloat3(&mUp, UP);
	XMStoreFloat3(&mLook, LOOK);

	mView(0,0) = mRight.x; 
	mView(1,0) = mRight.y; 
	mView(2,0) = mRight.z; 
	mView(3,0) = x;   

	mView(0,1) = mUp.x;
	mView(1,1) = mUp.y;
	mView(2,1) = mUp.z;
	mView(3,1) = y;  

	mView(0,2) = mLook.x; 
	mView(1,2) = mLook.y; 
	mView(2,2) = mLook.z; 
	mView(3,2) = z;   

	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;
}
XMMATRIX Camera::GetProj()
{
	return XMLoadFloat4x4(&mProj);
}
XMMATRIX Camera::GetView()
{
	return XMLoadFloat4x4(&mView);
}void Camera::SetPosition(XMFLOAT3& pos)
{
	mPosition = pos;
}
XMMATRIX Camera::InverseTranspose(CXMMATRIX M)
{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
}
void Camera::HideMouse(HWND hwnd)
{
	//������δ�����������������ƶ�����
	POINT lt, rb;
	RECT rect;
	GetClientRect(hwnd, &rect);  //ȡ�ô����ڲ�����
	//���������ϵ��������lt��
	lt.x = rect.left;
	lt.y = rect.top;
	//�����������������rb��
	rb.x = rect.right;
	rb.y = rect.bottom;
	//��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);
	//����Ļ���������趨��������
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);

	ShowCursor(false);		//���������
}
//FirstPersonCamera
void FirstPersonCamera::AlongLook(float dt)
{
	XMVECTOR LOOK = ::XMLoadFloat3(&mLook);
	XMVECTOR POS = ::XMLoadFloat3(&mPosition);
	POS += LOOK*dt;
	::XMStoreFloat3(&mPosition, POS);
}
void FirstPersonCamera::AlongUp(float dt)
{
	XMVECTOR UP = ::XMLoadFloat3(&mUp);
	XMVECTOR POS = ::XMLoadFloat3(&mPosition);
	POS += UP*dt;
	::XMStoreFloat3(&mPosition, POS);
}
void FirstPersonCamera::AlongWorldUp(float dt)
{

	XMVECTOR UP = XMVector3Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	XMVECTOR POS = ::XMLoadFloat3(&mPosition);
	POS += UP*dt;
	::XMStoreFloat3(&mPosition, POS);
}
void FirstPersonCamera::AlongRight(float dt)
{
	XMVECTOR RIGHT = ::XMLoadFloat3(&mRight);
	XMVECTOR POS = ::XMLoadFloat3(&mPosition);
	POS += RIGHT*dt;
	::XMStoreFloat3(&mPosition, POS);
}

void FirstPersonCamera::RotY(float angle)//ɨ��
{
	
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}
void FirstPersonCamera::RotRight(float angle)//����
{

	XMVECTOR RIGHT = ::XMLoadFloat3(&mRight);
	XMMATRIX R = ::XMMatrixRotationAxis(RIGHT, angle);
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}
// //  ThirdCamera

ThirdPersonCamera::ThirdPersonCamera() :mDy(0.0f)
{
}
bool ThirdPersonCamera::Init(XMFLOAT3& pos, XMFLOAT3& target, float maxR, float minR)
{
	XMVECTOR EyePos = ::XMLoadFloat3(&pos);
	XMVECTOR TAR = ::XMLoadFloat3(&target);
	XMVECTOR ToEye = EyePos - TAR;
	XMVECTOR Length = ::XMVector3Length(ToEye);
	XMFLOAT3 length;
	XMStoreFloat3(&length, Length);
	if (length.x > maxR || length.x < minR)
		return false;

	mPosition = pos;
	mTarget = target;
	mMaxRadius = maxR;
	mMinRadius = minR;

	XMVECTOR Look = TAR - EyePos;
	XMFLOAT3 right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMVECTOR Right = XMLoadFloat3(&right);
	XMVECTOR Up = ::XMVector3Cross(Look, Right);
	XMFLOAT3 up;
	XMStoreFloat3(&up, Up);
	SetLookAt(mPosition, mTarget, up);

	return true;
}

void ThirdPersonCamera::RotObjectY(XMFLOAT3& pos, float angle)
{
	XMVECTOR ObjectPos=::XMLoadFloat3(&pos);

	float dx=pos.x;
	float dy=pos.y;
	float dz=pos.z;

	XMFLOAT3 eyePos=mPosition;
	eyePos.x-=dx;//ת��������������ϵ
	eyePos.y-=dy;
	eyePos.z-=dz;

	XMVECTOR EyePos=::XMLoadFloat3(&eyePos);
	XMMATRIX matY=::XMMatrixRotationY(angle);

	EyePos=::XMVector3TransformCoord(EyePos,matY);//�ڡ�������ϵ�ڱ任
	XMStoreFloat3(&mRight,   XMVector3TransformNormal(XMLoadFloat3(&mRight),matY));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp),matY));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook),matY));

	XMStoreFloat3(&eyePos,EyePos);

	eyePos.x+=dx;//ת����ԭ����ϵ
	eyePos.y+=dy;
	eyePos.z+=dz;

	mPosition=eyePos;//������任���
//-----
}

void ThirdPersonCamera::RotObjectAxis(XMFLOAT3& pos, float angle)
{
	//mDy+=angle;
	//if(mDy>XM_PI/6 )
	//{
	//	mDy=XM_PI/6;
	//	return;
	//}
	//if(mDy<-XM_PI/3.5f )
	//{
	//	mDy=-XM_PI/3.5f;
	//	return;
	//}

	XMVECTOR ObjectPos=::XMLoadFloat3(&pos);

	float dx=pos.x;
	float dy=pos.y;
	float dz=pos.z;

	XMFLOAT3 eyePos=mPosition;
	eyePos.x-=dx;//ת��������������ϵ
	eyePos.y-=dy;
	eyePos.z-=dz;

	XMVECTOR EyePos=::XMLoadFloat3(&eyePos);
	XMVECTOR  A=XMLoadFloat3(&mRight);
	A=::XMVector3Normalize(A);
	XMMATRIX matA=::XMMatrixRotationAxis(A,angle);

	EyePos=::XMVector3TransformCoord(EyePos,matA);//�ڡ�������ϵ�ڱ任
	XMStoreFloat3(&eyePos,EyePos);

//	if(eyePos.y<0.5f)
//		return;
	

	XMStoreFloat3(&mRight,   XMVector3TransformNormal(XMLoadFloat3(&mRight),matA));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp),matA));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook),matA));

	

	eyePos.x+=dx;//ת����ԭ����ϵ
	eyePos.y+=dy;
	eyePos.z+=dz;

	mPosition=eyePos;//������任���
}

void ThirdPersonCamera::Scale(XMFLOAT3& pos, float fParam)
{
	XMVECTOR EyePos=::XMLoadFloat3(&mPosition);
	XMVECTOR Pos=::XMLoadFloat3(&pos);
	XMVECTOR vScale=Pos-EyePos;
	EyePos+=vScale*(fParam);

	XMVECTOR ToEye=EyePos-Pos;
	XMVECTOR Length=::XMVector3Length(ToEye);
	XMFLOAT3 length;
	XMStoreFloat3(&length,Length);
	float newR=length.x;//���°뾶
	if(newR<mMaxRadius && newR >mMinRadius)
	{
		XMStoreFloat3(&mPosition, EyePos);
	}
}
void ThirdPersonCamera::UpdatePosition(XMFLOAT3& pos)
{
	float dx = pos.x - mTarget.x;
	float dy = pos.y - mTarget.y;
	float dz = pos.z - mTarget.z;
	mPosition.x+=dx;
	mPosition.y+=dy;
	mPosition.z+=dz;
	mTarget = pos;
}

