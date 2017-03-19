#ifndef LEONA_H
#define LEONA_H
#include"MeshModel\SkinnedModel.h"
#include"Box.h"
#include"Util.h"

const float MAX_BLOOD=100.0f;
const float BLOOD_CUT=10.0f;
class Leona :public SkinnedModelInstance
{
public:
	Leona();
	int  Init();

	//更新各种坐标
	void UpdatePosition(float dt);

	//更新世界矩阵和动画
	void UpdateWorldMat_Animation(float dt);

	void SetModel(SkinnedModel* model);

	void Idle();
	void Run();
	void Attack_1();
	void Attack_2();
	void Attack_3();
	void Attack_add();
	void Defen();
	void Jump();
	void ConselJump();
	void Dead();
	void Would();

	//virtual
	void DoAtionEnd();

	int GetState(){ return mState; }
	void SetState(int state){ mState = state; }

	void  SetIsAbleMoveTrue(){ mIsAbleMove = true; }
	void SetIsAbleMoveFalse(){ mIsAbleMove = false; }
	float GetRunSpeed(){ return mRunSpeed; }
	float GetUpSeed()const { return mUpSpeed; }
	void SetUpSpeed(float s){ mUpSpeed = s; }
	bool IsAbleChangeState();
	
	void LevelMove(float dt);
	void CollisionDetection();
	float GetBloodScaling(){ return mBlood > 0.0f ? mBlood / MAX_BLOOD : 0.0f; }
public:
	enum STATE//动画状态
	{
		IDLE_1 = 0, RUN, ATTACK_1, ATTACK_2, ATTACK_3, DEFEN, ATTACK_ADD, JUMP, FORWARD_JUMP,DEAD,DEAD_END,WOULD
	};
	CBox   mFootBox;
	CBox   mBodyBox;
	CollidePoint mLeftFootCp, mRightFootCp;
	CollidePoint mLeftBodyCp[4], mRightBodyCp[4];
	CollidePoint mTopCp;

	bool mIsCut,mIsChi;
	//顶部碰撞点偏移
	const float TOP_CP_OFFSET = 4.5f;
private:
	const float mRunSpeed;
	bool mIsAbleMove;
	float mBlood;
	int mState;
	float mAttackTimeCount;
	
	std::string mClipNames[6];

	//Idle1
	const float mIdle1Begin;
	const float mIdle1End;

	//Run
	const float mRunBegin;
	const float mRunEnd;

	//Attack_1
	const float mAttack_1Begin;
	const float mAttack_1End;

	//Attack_2
	const float mAttack_2Begin;
	const float mAttack_2End;

	//Attack_3
	const float mAttack_3Begin;
	const float mAttack_3End;

	//Attack_add
	const float mAttack_addBegin;
	const float mAttack_addEnd;

	//Defen
	const float mDefenBegin;
	const float mDefenEnd;

	//Jump
	const float mJumpBegin;
	const float mJumpEnd;

	//Would
	const float mWouldBegin;
	const float mWouldEnd;
};
#endif