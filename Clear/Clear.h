#pragma once
#include"Common\d3dApp.h"
#include"Common\Geometry.h"
#include"Common\Camera.h"
#include"Common\Effect.h"
#include"Common\GlobleVariable.h"
#include"Effect\BasicEffect.h"
#include"Qianmo\DirectInputClass.h"

#include"MyUtil\MyUtil.h"
#include"Effect\AnimateEffect.h"
#include"MeshModel\SkinnedModel.h"
#include"MeshModel\BasicModel.h"
#include"Leona.h"
#include"Zombie.h"
#include"Box.h"
#include"Util.h"
#include"BloodStrip.h"
#include"Sound\GameSound.h"

//标记地图位置
struct Mp
{
	Mp() :y(0.0f), z(0.0f){}
	Mp(float _y, float _z) :y(_y), z(_z){}
	bool operator<(const Mp& mp) const
	{
		if (y < mp.y)
			return true;
		if (y == mp.y && z < mp.z)
			return true;
		return false;

	}
	bool operator()(const Mp& mp1, const Mp&mp2)
	{
		return mp1 < mp2;
	}
	bool operator==(Mp& mp)
	{
		if (fabs(mp.y - y) < ZERO && fabs(mp.z - z) < ZERO)
			return true;
		return false;
	}
	float y;
	float z;
};

class Clear :public D3DApp
{
public:
	Clear(HINSTANCE hInstance);
	~Clear();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	void OnMouseWheel(WPARAM wheelState);

	void OnKeyDown(WPARAM wParam);
	void OnKeyUp(WPARAM wParam);

private:
	UINT mW, mS, mA, mD, mE, mQ, mSHIFT, mSPACE;
	bool mLeftMouseDown,mRightMouseDown;
	int mI, mK, mJ, mL;
public:
	POINT           mLastMousePos;
	FirstPersonCamera mCamera;
	DirectLight      mDirectLight[3];
	DInputClass*      mpInput;
	GameSound        mSound;
public:
	TextureMgr       mTextureMgr;
	SkinnedModel* mZombieModel;
	SkinnedModel* mLeonaModel;
	std::vector<BasicModel*> mBasicModels;
	Sky           mSky;
	Leona         mLeona;
	std::vector<Zombie>  mZombies;
	std::vector<CBox>   mCBoxs;
	std::vector<TransmitDoor> mTransmitDoors;
	Door mVictoryDoor;

	BasicModel *mBrick_1, *mBrick_2, *mBrick_3,*mCrate;

	std::map<Mp, int> mMapOccupy;
	int mlayerIndex;

	BloodStrip     mBloodStrip;
public:
	void DrawSkinnedModel(SkinnedModelInstance& model);
	void DrawBasicModel(BasicModelInstance& model);
	void DoInput(float dt);
	void InitMapUnit();
	void LoadMap();
	void ConverPos(Mp& mp,CollidePoint& cp);
	void LeonaCollisionDetection();
	void ZombieCollisionDetection(Zombie& zombie);
	bool IsPositionOccupied(Mp& mp);
	void RemoveOccupied(Mp& mp);

	void DoRunIntoTransmitDoor();
	void DoZombieBeAttacked(Zombie& zombie,int* beAttackedType);
	void DoZombieAttack(Zombie& zombie);

	void GameFinish();

	void UpdateSound();

	int sendError(LPCWSTR msg)
	{
		MessageBox(0, msg, 0, 0);
		return 0;
	}
};
