#ifndef ZOMBIE_H
#define ZOMBIE_H

#include"MeshModel\SkinnedModel.h"
#include"Box.h"

const float WALK_SPEED_1 = 3.0f;
const float WALK_SPEED_2 = 5.0f;
const float RUN_SPEED = 15.0f;
const int RANDOM_COUNT = 20;
const int MIN_STATE_FRAME = 300;
const int MAX_STATE_FRAME = 1000;

const float MAX_BLOOD_VALUE = 100.0f;
const float NORMAL_ATTED_BLOOD_CUT = 21.0f;
const float ATENSIVE_ATTED_BLOOD_CUT = 30.0f;

class Zombie :public SkinnedModelInstance
{
public:
	Zombie();
	~Zombie();
	void InitRandomNum(int zombieIndex);

	//更新状态
	void UpdateState(XMFLOAT3& enemyPos);
	//更新待机状态
	void UpdatePatrolState();

	//更新各种坐标
	void UpdatePosition(float dt);

	//更新世界矩阵和动画
	void UpdateWorldMat_Animation(float dt);
	void SetModel(SkinnedModel* model);

	void Idle();
	void Walk_1();
	void Walk_2();
	void Run();
	void Attack_1();
	void Attack_2();
	void Attack_3();
	void RightAtt();
	void LeftAtt();
	void BellyAtt();
	void Dead();

	//virtual
	void DoAtionEnd();

	void ChangeDirection();
	void SetDirection(int direct);
	bool IsMove();
	bool IsFindEnemy()const{ return mIsFindEnemy; }
	bool IsAttack()const { return mIsAttack; }
	void SetIsAttackSuccessTrue(){ mIsAttackSuccess = true; }

	//是否死透
	bool IsDead(){ return mIsDead; }

	//是否在播放死动画
	bool IsDeadAnimation()
	{
		if (mBloodValue < 0.0f)
			return true;
		return false;
	}
	
	enum STATE//动画状态
	{
		IDLE = 0, WALK_1, WALK_2, RUN, ATTACK_1, ATTACK_2, ATTACK_3, ATTED,DEAD
	};
	CBox mBox[6];
	CollidePoint mCP[6];
	bool mIsAbleMove;

	//横锯
	bool mIsHengJu;
private:
	float mRunSpeed;
	int mState;
	bool mIsFindEnemy;
	bool mIsAttack;
	bool mIsAttackSuccess;

	//横锯
	float mHengJuTimeCount;

	float mBloodValue;
	bool mIsDead;


	int mPatrolStateIndex;
	int mPatrolStateFrameCount;
	int mPatrolStates[RANDOM_COUNT];
	int mPatrolStateTimes[RANDOM_COUNT];
	std::string mClipName;
	//Idle1
	const float mIdle1Begin;
	const float mIdle1End;

	//walk_1
	const float mWalk_1Begin;
	const float mWalk_1End;

	//walk_2
	const float mWalk_2Begin;
	const float mWalk_2End;

	//run
	const float mRunBegin;
	const float mRunEnd;

	//attack1
	const float mAttack_1Begin;
	const float mAttack_1End;

	//attack2
	const float mAttack_2Begin;
	const float mAttack_2End;

	//attack3
	const float mAttack_3Begin;
	const float mAttack_3End;

	//right_arm_attacked
	const float mRightAttBegin;
	const float mRightAttEnd;

	//left_arm_attacked
	const float mLeftAttBegin;
	const float mLeftAttEnd;

	//belly_attacked
	const float mBellyAttBegin;
	const float mBellyAttEnd;

	//dead
	const float mDeadBegin;
	const float mDeadEnd;
};
#endif