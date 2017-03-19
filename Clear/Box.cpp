#include"Box.h"


void Door::SetModel(BasicModel* model)
{
	for (int i = 0; i < DOOR_BOX_COUNT; i++)
	{
		mBoxs[i].SetModel(model);
	}
}
void Door::SetBoxsPosition(XMFLOAT3& pos)
{
	SetBoxsPosition(pos.x, pos.y, pos.z);
}
void Door::SetBoxsPosition(float x, float y, float z)
{
	mPos.x = x;
	mPos.y = y;
	mPos.z = z;

	float bx = 0.0f;
	float by = mPos.y + WIDTH*2.5f;
	float bz = mPos.z - WIDTH*1.5f;
	for (int i = 0; i < 4; i++)
	{
		mBoxs[i].SetPosition(bx, by, bz);
		bz += WIDTH;
	}
	bz -= WIDTH;
	for (int i = 4; i < DOOR_BOX_COUNT; i++)
	{
		by -= WIDTH;
		mBoxs[i].SetPosition(bx, by, bz);
	}
}
void TransmitDoor::SetModel(BasicModel* model)
{
	mDoors[0].SetModel(model);
	mDoors[1].SetModel(model);
}
