#ifndef BOX_H
#define BOX_H

#include"MeshModel\BasicModel.h"
#include"Util.h"
class CBox : public BasicModelInstance
{
public:
	CBox()
	{ Scale = 1.0f;
    	mModelIndex = 0;
	}
	int mModelIndex;
};

const int DOOR_BOX_COUNT = 9;
const float DOOR_WIDTH = WIDTH * 4.0f;
const float DOOR_HEIGHT = WIDTH*6.0f;

class Door
{
public:
	Door(){ mPos.x = 0.0f; mPos.y = 0.0f; mPos.z = 0.0f; mIsVictoryDoor = false; }

	void SetBoxsPosition(XMFLOAT3& pos);
	void SetBoxsPosition(float x, float y, float z);
	void ReSetDoorPosition()
	{
		mPos.x = 0.0f;
		mPos.y = mBoxs[0].Position.y - WIDTH*2.5f;
		mPos.z = mBoxs[0].Position.z + WIDTH*1.5f;
	}
	void SetModel(BasicModel* model);

	CBox mBoxs[DOOR_BOX_COUNT];
	XMFLOAT3 mPos;

	bool mIsVictoryDoor;
};
class TransmitDoor
{
public:
	TransmitDoor(){}

	void SetModel(BasicModel* model);
	Door mDoors[2];
};
#endif