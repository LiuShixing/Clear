#include"Zombie.h"

Zombie::Zombie() :mIdle1Begin(3.5f), mIdle1End(7.33333f),
mWalk_1Begin(0.0333333f), mWalk_1End(1.56667f),
mWalk_2Begin(2.33333f), mWalk_2End(3.33333f),
mRunBegin(1.66667f), mRunEnd(2.26667f),
mAttack_1Begin(7.33333f), mAttack_1End(12.5f),
mAttack_2Begin(12.6667f), mAttack_2End(15.6667f),
mAttack_3Begin(15.8333f), mAttack_3End(20.0f),
mRightAttBegin(20.1667f), mRightAttEnd(21.6667f),
mLeftAttBegin(21.8333f), mLeftAttEnd(24.0f),
mBellyAttBegin(24.1667f), mBellyAttEnd(25.0f),   //mBellyAttBegin(24.1667f), mBellyAttEnd(25.0f),
mDeadBegin(25.3333f), mDeadEnd(27.5f),          //mDeadEnd(28.0f)
mPatrolStateFrameCount(0), mPatrolStateIndex(0),
mIsFindEnemy(false), mIsAttack(false), mIsAbleMove(true),
mIsHengJu(false), mIsAttackSuccess(false), mBloodValue(MAX_BLOOD_VALUE),
mIsDead(false)

{
	mClipName = "Take001";
	Scale = 0.33333f;

	SetDirection(DIRECT::PZ);
	//	SetVerticalStateLevel();
}
Zombie::~Zombie()
{

}

void Zombie::InitRandomNum(int zombieIndex)
{
	srand(zombieIndex*time(0));
	for (int i = 0; i < RANDOM_COUNT; i++)
	{
		mPatrolStates[i] = rand() % 6;
		mPatrolStateTimes[i] = MIN_STATE_FRAME + rand() % (MAX_STATE_FRAME - MIN_STATE_FRAME);
	}

}
void Zombie::SetModel(SkinnedModel* model)
{
	Model = model;
	FinalTransforms.resize(model->SkinnedData.BoneCount());
}
bool Zombie::IsMove()
{
	if (mState == STATE::WALK_1 || mState == STATE::WALK_2 || mState == STATE::RUN)
		return true;
	return false;
}

void Zombie::UpdatePosition(float dt)
{
	if (mVerticalState == VERTICAL_STATE::DOWN)
	{
		Position.y += mUpSpeed*dt;
		mUpSpeed = mUpSpeed - G*dt;
		if (mUpSpeed < 0.0f)
			mVerticalState = VERTICAL_STATE::DOWN;
	}
	if (IsMove() &&  mIsAbleMove && mVerticalState == VERTICAL_STATE::LEVEL)
	{
		Position.z += Direction*mRunSpeed*dt;
	}
	float y = Position.y + WIDTH*0.5f;
	for (int i = 0; i < 6; i++)
	{
		if (Direction == DIRECT::PZ)
		{
			mBox[i].SetPosition(0.0f, y, Position.z + WIDTH*0.5f);
			mCP[i].y = y;
			mCP[i].z = Position.z + WIDTH*0.5f;
		}
		else
		{
			mBox[i].SetPosition(0.0f, y, Position.z - WIDTH*0.5f);
			mCP[i].y = y;
			mCP[i].z = Position.z - WIDTH*0.5f;
		}
		y += WIDTH;
	}
}
void Zombie::UpdateState(XMFLOAT3& enemyPos)
{
	mIsFindEnemy = false;
	if (fabs(Position.z - enemyPos.z)<ZOM_ALERT_DISTANCE)
	{//发现敌人
		mIsFindEnemy = true;
		if (mState != STATE::ATTED)
		{
		/*	if (!mIsAttack)
			{*/
				if (Position.z > enemyPos.z)
					SetDirection(DIRECT::NZ);
				else
					SetDirection(DIRECT::PZ);
		//	}

			if (fabs(Position.z - enemyPos.z) < ZOM_ATTACK_DISTANCE)
			{//攻击敌人
				mIsAttack = true;
				mIsAbleMove = false;
				if (mState != STATE::ATTACK_3)
					Attack_3();
			}
			else
			{
				if (mState != STATE::RUN && !mIsAttack)
					Run();
			}
		}
	}
	else
	{
		UpdatePatrolState();
	}
}
void Zombie::UpdatePatrolState()
{
	if (mState == STATE::RUN)
	{
		Walk_1();
	}
	mPatrolStateFrameCount++;
	if (mPatrolStateFrameCount > mPatrolStateTimes[mPatrolStateIndex])
	{
		mPatrolStateFrameCount = 0;
		mPatrolStateIndex = (mPatrolStateIndex + 1) % RANDOM_COUNT;
		switch (mPatrolStates[mPatrolStateIndex])
		{
		case 0:
			Idle();
			break;
		case 1:
			SetDirection(DIRECT::PZ); 
			Walk_2();
			break;
		case 2:
			SetDirection(DIRECT::NZ); 
			Walk_1();
			break;
		case 3:
			SetDirection(DIRECT::PZ); 
			Walk_2();
			break;
		case 4:
			SetDirection(DIRECT::NZ); 
			Walk_1();
			break;
		case 5:
			Idle();
			break;
		default:
			break;
		}
	}
}
void Zombie::UpdateWorldMat_Animation(float dt)
{
	if (UpdateAnimation(dt) == ACTION::END)
	{
		DoAtionEnd();
	}
	if (mState == STATE::ATTACK_3 && !mIsHengJu)
	{
		mHengJuTimeCount += dt;
		if (mHengJuTimeCount > 1.0f)
		{
			mIsHengJu = true;
		}
	}
	UpdateWorld();
}
void Zombie::ChangeDirection()
{
	if (Direction == DIRECT::PZ)
	{
		SetDirection(DIRECT::NZ);
	}
	else
	{
		SetDirection(DIRECT::PZ);
	}
}
void Zombie::SetDirection(int direct)
{
	if (direct == DIRECT::PZ)
	{
		Direction = DIRECT::PZ;
		RotAngle = -XM_PI*0.5f;
	}
	if (direct == DIRECT::NZ)
	{
		RotAngle = XM_PI*0.5f;
		Direction = DIRECT::NZ;
	}
}
void Zombie::DoAtionEnd()
{
	mIsAttack = false;
	if (mState == STATE::ATTED)
	{
		Idle();
	}
	mHengJuTimeCount = 0.0f;
	mIsHengJu = false;

	if (mState == STATE::DEAD)
	{
		mIsDead = true;
	}
	////攻击成功后执行一次idle，直到idle执行完才能进行下次攻击
	//if (mIsAttackSuccess)
	//{
	//	Idle();
	//}
	//if (mIsAttackSuccess && mState == STATE::IDLE)
	//{
	//	mIsAttackSuccess = false;
	//}
}
void Zombie::Idle()
{
	mActionAddSpeed = 1.0f;
	mState = STATE::IDLE;
	SetAction(mClipName, mIdle1Begin, mIdle1End);
}
void Zombie::Walk_1()
{
	mActionAddSpeed = 1.0f;
	mRunSpeed = WALK_SPEED_1;
	mState = STATE::WALK_1;
	SetAction(mClipName, mWalk_1Begin, mWalk_1End);
}
void Zombie::Walk_2()
{
	mActionAddSpeed = 1.0f;
	mRunSpeed = WALK_SPEED_2;
	mState = STATE::WALK_2;
	SetAction(mClipName, mWalk_2Begin, mWalk_2End);
}
void Zombie::Run()
{
	mActionAddSpeed = 1.0f;
	mRunSpeed = RUN_SPEED;
	mState = STATE::RUN;
	SetAction(mClipName, mRunBegin, mRunEnd);
}
void Zombie::Attack_1()
{
	mActionAddSpeed = 3.0f;
	mHengJuTimeCount = 0.0f;
	mState = STATE::ATTACK_1;
	SetAction(mClipName, mAttack_1Begin, mAttack_1End);
}
void Zombie::Attack_2()
{
	mActionAddSpeed = 2.0f;
	mHengJuTimeCount = 0.0f;
	mState = STATE::ATTACK_2;
	SetAction(mClipName, mAttack_2Begin, mAttack_2End);
}
void Zombie::Attack_3()
{
	mActionAddSpeed = 1.6f;
	mHengJuTimeCount = 0.0f;
	mState = STATE::ATTACK_3;
	SetAction(mClipName, mAttack_3Begin, mAttack_3End);
}
void Zombie::RightAtt()
{
	mActionAddSpeed = 1.0f;
	mBloodValue -= NORMAL_ATTED_BLOOD_CUT;
	if (mBloodValue < 0.0f)
	{
		mState = STATE::DEAD;
		Dead();
	}
	else
	{
		mState = STATE::ATTED;
		SetAction(mClipName, mRightAttBegin, mRightAttEnd);
	}
}
void Zombie::LeftAtt()
{
	mActionAddSpeed = 1.0f;
	mBloodValue -= NORMAL_ATTED_BLOOD_CUT;
	if (mBloodValue < 0.0f)
	{
		mState = STATE::DEAD;
		Dead();
	}
	else
	{
		mState = STATE::ATTED;
		SetAction(mClipName, mLeftAttBegin, mLeftAttEnd);
	}
}
void Zombie::BellyAtt()
{
	mActionAddSpeed = 1.0f;
	mBloodValue -= ATENSIVE_ATTED_BLOOD_CUT;
	if (mBloodValue < 0.0f)
	{
		mState = STATE::DEAD;
		Dead();
	}
	else
	{
		mState = STATE::ATTED;
		SetAction(mClipName, mBellyAttBegin, mBellyAttEnd);
	}
}
void Zombie::Dead()
{
	mActionAddSpeed = 1.0f;
	mState = STATE::DEAD;
	SetAction(mClipName, mDeadBegin, mDeadEnd);
}