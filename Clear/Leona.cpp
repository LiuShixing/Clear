#include"Leona.h"
#include"MyUtil\MyUtil.h"



Leona::Leona() :mIdle1Begin(0.0f), mIdle1End(8.374f), //mIdle1Begin(0.0f), mIdle1End(8.374f),
mRunBegin(0.0f), mRunEnd(1.25f),
mAttack_1Begin(0.0f), mAttack_1End(1.25f),
mAttack_2Begin(0.0f), mAttack_2End(1.25f),
mAttack_3Begin(0.0f), mAttack_3End(1.25f),
mAttack_addBegin(1.29167f), mAttack_addEnd(5.5f),  //1.5 4.5  1.29167
mDefenBegin(0.0f), mDefenEnd(1.25f),
mJumpBegin(1.25f), mJumpEnd(2.0f), //2.45833f
mWouldBegin(2.0f), mWouldEnd(3.25f),
mState(STATE::IDLE_1),
mRunSpeed(10.0f*OBJ_SCALE), mIsAbleMove(true),
mIsCut(false), mIsChi(false), mBlood(MAX_BLOOD)
{
	mClipNames[0] = "Idle1";
	mClipNames[1] = "Run";
	mClipNames[2] = "Attack1";
	mClipNames[3] = "Attack2";
	mClipNames[4] = "Attack3";
	mClipNames[5] = "Defen";

	Scale = 0.03333f*OBJ_SCALE;
}
int  Leona::Init()
{
	Idle();
	return 1;
}
void Leona::UpdateWorldMat_Animation(float dt)
{
	UpdateWorld();
	if (UpdateAnimation(dt) == ACTION::END)
	{
		DoAtionEnd();
	}
	mIsCut = false;
	mIsChi = false;
	if ((mState == STATE::ATTACK_1 || mState == STATE::ATTACK_2) && mAttackTimeCount>-1.0f)
	{
		mAttackTimeCount += dt;
		if (mAttackTimeCount > 0.4f)
		{
			mAttackTimeCount = -10.0f;
			mIsCut = true;
		}
	}
	if (mState == STATE::ATTACK_3 && mAttackTimeCount>-1.0f)
	{
		mAttackTimeCount += dt;
		if (mAttackTimeCount > 0.5f)
		{
			mAttackTimeCount = -10.0f;
			mIsChi = true;
		}
	}
}
void Leona::UpdatePosition(float dt)
{
	if (mVerticalState != VERTICAL_STATE::LEVEL)
	{
		Position.y += mUpSpeed*dt;
		mUpSpeed = mUpSpeed - G*dt;
		if (mUpSpeed < 0.0f)
			mVerticalState = VERTICAL_STATE::DOWN;
	}

	mFootBox.SetPosition(Position.x, Position.y + WIDTH*0.5f, Position.z + Direction*0.4f*OBJ_SCALE);
	mBodyBox.SetPosition(Position.x, Position.y + 3.0f*WIDTH, Position.z + Direction*0.8f*OBJ_SCALE);

	mLeftFootCp.y = Position.y + WIDTH*0.5f;
	mLeftFootCp.z = Position.z + Direction*0.4f*OBJ_SCALE - WIDTH*0.5f;

	mRightFootCp.y = Position.y + WIDTH*0.5f;
	mRightFootCp.z = Position.z + Direction*0.4f*OBJ_SCALE + WIDTH*0.5f;

	//bodyBox depth=WIDTH*2.8f
	for (int i = 0; i < 4; i++)
	{
		// 0->3   y--     
		mLeftBodyCp[i].y = Position.y + (4 - i)*WIDTH + WIDTH*0.5f;
		mLeftBodyCp[i].z = Position.z + Direction*0.8f*OBJ_SCALE - WIDTH*0.9f;

		mRightBodyCp[i].y = Position.y + (4 - i)*WIDTH + WIDTH*0.5f;
		mRightBodyCp[i].z = Position.z + Direction*0.8f*OBJ_SCALE + WIDTH*0.9f;
	}
	mTopCp.y = mLeftBodyCp[0].y;
	mTopCp.z = Position.z + Direction*0.8f*OBJ_SCALE;

}
void Leona::LevelMove(float dt)
{
	if (mIsAbleMove)
	{
		Position.z += Direction*mRunSpeed*dt;
	}
}

void Leona::CollisionDetection()
{
}
void Leona::DoAtionEnd()
{
	if (mState == STATE::DEAD)
	{
		mState = STATE::DEAD_END;
	}
	else
	{
		mState = STATE::IDLE_1;
		Idle();
	}
}
void Leona::SetModel(SkinnedModel* model)
{
	Model = model;
	FinalTransforms.resize(model->SkinnedData.BoneCount());
}
bool Leona::IsAbleChangeState()
{
	if (mState == STATE::IDLE_1 || mState == STATE::RUN || mState == STATE::DEFEN )
		return true;
	else
		return false;
}
void Leona::Dead()
{
	mBlood = 0.0f;
	mState = STATE::DEAD;
	Jump();
}
void Leona::Idle()
{
	SetAction(mClipNames[STATE::IDLE_1], mIdle1Begin, mIdle1End);
}
void Leona::Run()
{
	SetAction(mClipNames[STATE::RUN], mRunBegin, mRunEnd);
}
void Leona::Attack_1()
{
	mAttackTimeCount = 0.0f;
	SetAction(mClipNames[STATE::ATTACK_1], mAttack_1Begin, mAttack_1End);
}
void Leona::Attack_2()
{
	mAttackTimeCount = 0.0f;
	SetAction(mClipNames[STATE::ATTACK_2], mAttack_2Begin, mAttack_2End);
}
void Leona::Attack_3()
{
	mAttackTimeCount = 0.0f;
	SetAction(mClipNames[STATE::ATTACK_3], mAttack_3Begin, mAttack_3End);
}
void Leona::Attack_add()
{
	SetAction(mClipNames[STATE::ATTACK_1], mAttack_addBegin, mAttack_addEnd);
}
void Leona::Defen()
{
	SetAction(mClipNames[STATE::DEFEN], mDefenBegin, mDefenEnd);
}
void Leona::Jump()
{
	mUpSpeed = MAX_SPEED;
	mVerticalState = VERTICAL_STATE::UP;
	SetAction(mClipNames[STATE::RUN], mJumpBegin, mJumpEnd);
}
void Leona::Would()
{
	mBlood -= BLOOD_CUT;
	if (mBlood < 0.0f)
	{

		mState = STATE::DEAD_END;
	}
	else
	{
		SetAction(mClipNames[STATE::RUN], mWouldBegin, mWouldEnd);
	}
}
void Leona::ConselJump()
{
	if (mState == STATE::JUMP)
	{
		mUpSpeed = 0.0f;
		mState = STATE::IDLE_1;
	}
}