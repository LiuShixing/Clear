
#include"Clear.h"

#include<MMSystem.h>
#pragma comment (lib,"winmm.lib")





Clear::Clear(HINSTANCE hInstance) :D3DApp(hInstance), mW(0), mS(0), mA(0), mD(0), mE(0), mQ(0),
mLeftMouseDown(false), mRightMouseDown(false), mI(0), mK(0), mJ(0), mL(0),
mBrick_1(NULL), mBrick_2(NULL), mBrick_3(NULL), mCrate(NULL), mlayerIndex(0)






{
	this->mMainWndCaption = L"Clear";
	::XMMATRIX O(::XMMatrixIdentity());


	mDirectLight[0].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirectLight[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectLight[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirectLight[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	mDirectLight[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirectLight[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectLight[1].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirectLight[1].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirectLight[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirectLight[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectLight[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirectLight[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);

	float y = ((float)(MAX_LAYER_COUNT - mlayerIndex) - 0.5f)*MAP_PLAYER_HEIGHT;
	float z = 0.5f*MAP_UNIT_LENGHT;
	mCamera.SetLookAt(XMFLOAT3(CAMERA_OFFSET_X, y, z), XMFLOAT3(0.0f, y, z), XMFLOAT3(0.0f, 1.0f, 0.0f));

}
Clear::~Clear()
{
	delete mLeonaModel;
	delete mZombieModel;
	for (int i = 0; i < mBasicModels.size(); i++)
	{
		delete mBasicModels[i];
	}
}
void Clear::OnResize()
{
	D3DApp::OnResize();
	mCamera.SetLen(0.25f*XM_PI, AspectRatio(), 1.0f, 1000.0f);
}
bool Clear::Init()
{
	int start = clock();
	if (!D3DApp::Init())
	{
		return 0;
	}
	mCamera.HideMouse(this->mhMainWnd);

	this->md3dImmediateContext->ClearRenderTargetView(this->mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	this->mSwapChain->Present(0, 0);

	if (!BasicEffect::Init(md3dDevice, L"HLSL/Basic.hlsl"))
		return sendError(L"build BasicEffect failed!");

	if (!InputLayout::InitPosNormTex(md3dDevice, BasicEffect::mTech))
		return sendError(L"InitPosNormalTexSkinned Failed!");

	if (!AnimateEffect::Init(md3dDevice, L"HLSL/Animate.hlsl"))
		return sendError(L"build AnimateEffect failed!");
	//	MyUtil::WriteString("build FX Success");

	if (!InputLayout::InitPosNormalTexSkinned(md3dDevice, AnimateEffect::mTech))
		return sendError(L"InitPosNormalTexSkinned Failed!");
	//	MyUtil::WriteString("InitPosNormalTexSkinned Success");

	RasterizerState::InitAll(md3dDevice);

	mSky.Init(md3dDevice, L"res/sky/snowcube1024.dds", L"HLSL/Sky.hlsl");
	mTextureMgr.Init(md3dDevice);

	mLeonaModel = new SkinnedModel(md3dDevice, mTextureMgr, "res/leona-m3d/leona.binm3d", L"res\\leona-m3d\\");
	mLeona.SetModel(mLeonaModel);
	mLeona.SetPosition(0.0f, (float)(MAX_LAYER_COUNT - mlayerIndex)*MAP_PLAYER_HEIGHT - WIDTH*5.0f, MAP_LEFT + WIDTH*3.0f);
	mLeona.Init();
	mLeona.mFootBox.SetModel(new BasicModel(md3dDevice, mTextureMgr, L"res/box/checkboard.dds", WIDTH, WIDTH*2.0f, WIDTH));
	mLeona.mBodyBox.SetModel(new BasicModel(md3dDevice, mTextureMgr, L"res/box/checkboard.dds", WIDTH, WIDTH*2.8f, WIDTH*4.0f));

	mZombieModel = new SkinnedModel(md3dDevice, mTextureMgr, "res/dianjunan-m3d/dianjuguai.binm3d", L"res\\dianjunan-m3d\\");

	//map
	InitMapUnit();
	LoadMap();


	//-----------------------------
	//进行DirectInput类的初始化
	mpInput = new DInputClass();
	mpInput->Init(this->mhMainWnd, this->mhAppInst, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//-----------------------------
	std::vector<std::wstring> Filenames;
	Filenames.push_back(L"res/box/1.jpg");
	Filenames.push_back(L"res/box/1.jpg");
	std::vector<Vertex::PosSize> bloodVertex;
	bloodVertex.resize(2);
	bloodVertex[0].pos = XMFLOAT3(0.5f, 0.5f, -1.0f);
	bloodVertex[0].size = XMFLOAT2(10.0f, 4.0f);

	bloodVertex[1].pos = XMFLOAT3(0.5f, 0.5f, -1.0f);
	bloodVertex[1].size = XMFLOAT2(5.0f, 4.0f);

	mBloodStrip.Init(md3dDevice,L"res/leona.png", L"res/bloodfw.png", L"res/blood.png",-0.86f,0.92f,0.4f,0.06f);
	//---------------------------
	MyUtil::WriteDataFloat("time", double(clock() - start), true);

	//音效初始化
	mSound.GameSoundInit(this->mhMainWnd);  
	mSound.GameMusicplay(mSound.pBaground);
	return 1;
}
void Clear::GameFinish()
{
	MessageBox(this->mhMainWnd, L"You Win!", 0, 0);
	exit(0);
}
void Clear::DoRunIntoTransmitDoor()
{
	XMFLOAT3 lpos = mLeona.GetPosition();

	//进入胜利们，游戏结束
	if (lpos.y > mVictoryDoor.mPos.y - WIDTH*3.5f
		&& lpos.y<mVictoryDoor.mPos.y + WIDTH*2.0f
		&& lpos.z>mVictoryDoor.mPos.z - WIDTH*2.0f
		&& lpos.z < mVictoryDoor.mPos.z + WIDTH*2.0f)
	{
		GameFinish();
	}

	for (int td = 0; td < mTransmitDoors.size(); td++)
	{
		for (int d = 0; d < 2; d++)
		{
			if (lpos.y>mTransmitDoors[td].mDoors[d].mPos.y - WIDTH*3.5f
				&& lpos.y<mTransmitDoors[td].mDoors[d].mPos.y + WIDTH*2.0f
				&& lpos.z>mTransmitDoors[td].mDoors[d].mPos.z - WIDTH*2.0f
				&& lpos.z < mTransmitDoors[td].mDoors[d].mPos.z + WIDTH*2.0f)
			{
			
				d = (d + 1) % 2;
				lpos = mTransmitDoors[td].mDoors[d].mPos;
				lpos.y -= WIDTH*3.0f;
				mLeona.SetPosition(lpos);
				mLeona.SetDirection(Leona::DIRECT::NZ);//改变方向

				//调整摄像机
				XMFLOAT3 cpos = mCamera.GetPosition();
				cpos.z = lpos.z;
				int newLayerIndex = MAX_LAYER_COUNT - floor(lpos.y / MAP_PLAYER_HEIGHT) - 1;
				if (newLayerIndex != mlayerIndex)
				{
					if (newLayerIndex > -1 && newLayerIndex < MAX_LAYER_COUNT)
					{
						mlayerIndex = newLayerIndex;
						cpos.y = ((float)(MAX_LAYER_COUNT - mlayerIndex) - 0.5f)*MAP_PLAYER_HEIGHT;
					}
				}
				mCamera.SetPosition(cpos);
				return;
			}
		}
	}
}
void Clear::UpdateScene(float dt)
{
	if (mLeona.GetState() == Leona::STATE::DEAD || mLeona.GetState() == Leona::STATE::DEAD_END)
	{
		mLeona.UpdatePosition(dt);
		mLeona.UpdateWorldMat_Animation(dt);
		if (mLeona.GetState() == Leona::STATE::DEAD_END)
		{
			MessageBox(this->mhMainWnd, L"Abandon All Hope..", 0, 0);
			exit(0);
		}
		return;
	}

	DoInput(dt);
	//-----------------
	if (mD == 0 && mLeona.GetDirection() == Leona::DIRECT::PZ)
	{//进门的方向都为Leona::DIRECT::PZ
		DoRunIntoTransmitDoor();
	}

	XMFLOAT3 pos = mCamera.GetPosition();
	pos.z = mLeona.GetPosition().z;
	if (pos.z < CAMERA_MIN_Z)
		pos.z = CAMERA_MIN_Z;
	if (pos.z>CAMERA_MAX_Z)
		pos.z = CAMERA_MAX_Z;
	mCamera.SetPosition(pos);


	float cy = mCamera.GetPosition().y;
	float cz = mCamera.GetPosition().z;
	float minY = cy - MAP_PLAYER_HEIGHT*0.6f;
	float maxY = cy + MAP_PLAYER_HEIGHT*0.6f;

	float minZ = cz - MAP_PLAYER_HEIGHT*1.5f;
	float maxZ = cz + MAP_PLAYER_HEIGHT*1.5f;
	XMFLOAT3 zpos;
	XMFLOAT3 leonaPos = mLeona.GetPosition();
	for (std::vector<Zombie>::iterator it = mZombies.begin(); it != mZombies.end(); it++)
	{
		if (it->IsDead())
			continue;
		zpos = it->GetPosition();

		//丧尸在可见区域
		if (zpos.y > minY && zpos.y<maxY && zpos.z>minZ && zpos.z < maxZ)
		{
			Mp tmp;		
			ConverPos(tmp, it->mCP[4]);
			RemoveOccupied(tmp);
			ConverPos(tmp, it->mCP[5]);
			RemoveOccupied(tmp);

			if (it->IsDeadAnimation())
			{
				it->UpdateWorldMat_Animation(dt);
				continue;
			}
			//更新位置
			it->UpdatePosition(dt);
			ZombieCollisionDetection(*it);

			//重新占位
			ConverPos(tmp, it->mCP[4]);
			mMapOccupy[tmp] = 1;
			ConverPos(tmp, it->mCP[5]);
			mMapOccupy[tmp] = 1;

			it->UpdateState(leonaPos);
			DoZombieAttack(*it);
			DoZombieBeAttacked(*it,&mSound.jiZhong);
			it->UpdateWorldMat_Animation(dt);
		}

	}
	//Update leona
	Mp lemp;
	ConverPos(lemp, mLeona.mLeftBodyCp[0]);
	RemoveOccupied(lemp);
	ConverPos(lemp, mLeona.mRightBodyCp[0]);
	RemoveOccupied(lemp);

	mLeona.UpdatePosition(dt);
	LeonaCollisionDetection();

	//重新占位
	ConverPos(lemp, mLeona.mLeftBodyCp[0]);
	mMapOccupy[lemp] = 1;
	ConverPos(lemp, mLeona.mRightBodyCp[0]);
	mMapOccupy[lemp] = 1;

	mLeona.UpdateWorldMat_Animation(dt);


	//播放声音
	UpdateSound();
	//-----------------------------------------------------------------------------
	mCamera.UpdateViewMatrix();

}
void Clear::UpdateSound()
{
	if (mLeona.GetState() == Leona::STATE::WOULD)
	{
	//	mSound.GameSoundplay(mSound.pLeonaHitted);
	}

	if (mLeona.GetState() == Leona::STATE::ATTACK_1  ||
		mLeona.GetState() == Leona::STATE::ATTACK_2)
	{
		if (mSound.huijianTime == 0)
			mSound.huijianTime = clock();

		if (clock() - mSound.huijianTime > 400)
		{
			if (mSound.jiZhong == mSound.HUI_JIAN_ATT)
				mSound.GameSoundplay(mSound.pHuijianjizhong);
			else
				mSound.GameSoundplay(mSound.pHuijian);
			mSound.jiZhong = mSound.NOT_ATT;
		}
	}
	else
	{
		mSound.huijianTime = 0;
	}
		

	if (mLeona.GetState() == Leona::STATE::ATTACK_3  && !mSound.isCi)
	{
		
		if (mSound.chiTime == 0)
			mSound.chiTime = clock();

		if (clock() - mSound.chiTime > 500 )
		{
			if (mSound.jiZhong == mSound.CI_ATT)
				mSound.GameSoundplay(mSound.pChizhong);
			else
				mSound.GameSoundplay(mSound.pChi);

			mSound.chiTime = 0;
			mSound.isCi = true;

			mSound.jiZhong = mSound.NOT_ATT;
		}
	}
	if (mLeona.GetState() != Leona::STATE::ATTACK_3)
	{
		mSound.isCi = false;
	}
}
void Clear::DoInput(float dt)
{
	//使用DirectInput类读取数据  
	mpInput->GetInput();

	float dx = mpInput->MouseDX();
	float dy = mpInput->MouseDY();
	float dz = mpInput->MouseDZ();


	bool isRun = false;
	if (mA)
	{
		isRun = true;
		if (mLeona.GetState() == Leona::STATE::IDLE_1 ||
			mLeona.GetState() == Leona::STATE::RUN ||
			mLeona.GetState() == Leona::STATE::JUMP ||
			mLeona.GetState() == Leona::STATE::DEFEN)
		{
			int oldDirect = mLeona.GetDirection();
			mLeona.SetDirection(Leona::DIRECT::NZ);

			//转身时检查身后是否有障碍，有则向原方向偏移
			if (oldDirect == Leona::DIRECT::PZ)
			{
				Mp mp[4];
				for (int i = 0; i < 4; i++)
				{
					ConverPos(mp[i], mLeona.mLeftBodyCp[i]);
					mp[i].z - +WIDTH;
					if (IsPositionOccupied(mp[i]))
					{

						break;
					}
				}
			}
		}
	}
	if (mD)
	{
		isRun = true;
		if (mLeona.GetState() == Leona::STATE::IDLE_1 ||
			mLeona.GetState() == Leona::STATE::RUN ||
			mLeona.GetState() == Leona::STATE::JUMP ||
			mLeona.GetState() == Leona::STATE::DEFEN)
		{
			int oldDirect = mLeona.GetDirection();
			mLeona.SetDirection(Leona::DIRECT::PZ);
		}
	}
	if (isRun)
	{
		if (mLeona.GetState() == Leona::STATE::IDLE_1 ||
			mLeona.GetState() == Leona::STATE::RUN ||
			mLeona.GetState() == Leona::STATE::JUMP)
		{
			mLeona.LevelMove(dt);
		}
	}

	if (mLeona.IsAbleChangeState())
	{
		if (mI)
		{
			if (mA || mD)
			{
				mLeona.SetState(Leona::STATE::ATTACK_3);
				mLeona.Attack_3();
			}
			else
			{
				mLeona.SetState(Leona::STATE::ATTACK_1);
				mLeona.Attack_1();
			}
		}
		if (mJ)
		{
			mLeona.SetState(Leona::STATE::ATTACK_2);
			mLeona.Attack_2();
		}
	}
	if (mLeona.IsAbleChangeState())
	{
		if (isRun && mLeona.GetVerticalState() == Leona::VERTICAL_STATE::LEVEL
			&& mLeona.GetState() != Leona::STATE::RUN && mL == 0)
		{
			mLeona.SetState(Leona::STATE::RUN);
			mLeona.Run();
		}
	}
	if (mLeona.IsAbleChangeState())
	{
		if (mL)
		{
			//同时按下run 和 defen 时仍保持defen
			if (mLeona.GetState() != Leona::STATE::DEFEN
				&& mLeona.GetVerticalState() == Leona::VERTICAL_STATE::LEVEL)
			{
				mLeona.SetState(Leona::STATE::DEFEN);
				mLeona.Defen();
			}
		}
		else
		{
			if (!isRun && mLeona.GetState() != Leona::STATE::IDLE_1)
			{
				mLeona.SetState(Leona::STATE::IDLE_1);
				mLeona.Idle();
			}
		}
	}
	if (mK && (mLeona.IsAbleChangeState() || mLeona.GetState() == Leona::STATE::WOULD)
		&& mLeona.GetVerticalState() == Leona::VERTICAL_STATE::LEVEL &&
		(mLeona.GetState() != Leona::STATE::JUMP || mLeona.GetState() != Leona::STATE::FORWARD_JUMP))
	{
		mLeona.SetState(Leona::STATE::JUMP);
		mLeona.Jump();
	}

}
void Clear::DoZombieBeAttacked(Zombie& zombie, int* beAttackedType)
{
	
	//Leona attack
	if (mLeona.mIsCut)
	{
		float zy = zombie.GetPosition().y;
		float zz = zombie.GetPosition().z;

		float ly = mLeona.GetPosition().y;
		float lz = mLeona.GetPosition().z;

		const float UP_Y = WIDTH*5.0f;
		const float DOWN_Y = WIDTH*5.0f;
	
		if (mLeona.GetDirection() == Leona::DIRECT::PZ)
		{
			if (zz - lz > 0 && zz - lz < LEONA_CUT_VALID_DISTANCE
				&& zy>ly-DOWN_Y && zy<ly+UP_Y)
			{
				zombie.RightAtt();
				*beAttackedType = 1;
			}
		}
		else
		{
			if (lz - zz > 0 && lz - zz < LEONA_CUT_VALID_DISTANCE
				&& zy>ly - DOWN_Y && zy<ly + UP_Y)
			{
				zombie.LeftAtt();
				*beAttackedType = 1;
			}
		}
	}
	if (mLeona.mIsChi)
	{
		
		float zy = zombie.GetPosition().y;
		float zz = zombie.GetPosition().z;

		float ly = mLeona.GetPosition().y;
		float lz = mLeona.GetPosition().z;

		const float UP_Y_2 = WIDTH*2.0f;
		const float DOWN_Y_2 = WIDTH*2.0f;
		if (mLeona.GetDirection() == Leona::DIRECT::PZ)
		{
			if (zz - lz >= 0 && zz - lz <= LEONA_CHI_VALID_DISTANCE
				&& zy>=ly - DOWN_Y_2 && zy<=ly + UP_Y_2)
			{
				zombie.BellyAtt();
				*beAttackedType = 2;
			}
		}
		else
		{
			if (lz - zz >= 0 && lz - zz <= LEONA_CHI_VALID_DISTANCE
				&& zy>=ly - DOWN_Y_2 && zy<=ly + UP_Y_2)
			{
				zombie.BellyAtt();
				*beAttackedType = 2;
			}
		}
	}
}
void Clear::DoZombieAttack(Zombie& zombie)
{
	if (zombie.mIsHengJu && mLeona.GetState() != Leona::STATE::WOULD &&mLeona.GetState() != Leona::STATE::JUMP)
	{
		float zy = zombie.GetPosition().y;
		float zz = zombie.GetPosition().z;

		float ly = mLeona.GetPosition().y;
		float lz = mLeona.GetPosition().z;

		const float UP_Y_2 = WIDTH*2.0f;
		const float DOWN_Y_2 = WIDTH*2.0f;
		if (zombie.GetDirection() == Zombie::DIRECT::NZ)
		{
			if (mLeona.GetDirection() == Leona::DIRECT::PZ && mLeona.GetState() == Leona::STATE::DEFEN)
				return;
			if (zz - lz >= 0 && zz - lz <= ZOM_ATTACK_DISTANCE
				&& zy >= ly - DOWN_Y_2 && zy <= ly + UP_Y_2)
			{
				mLeona.SetState(Leona::STATE::WOULD);
				mLeona.Would();
			}
		}
		else
		{
			if (mLeona.GetDirection() == Leona::DIRECT::NZ && mLeona.GetState() == Leona::STATE::DEFEN)
				return;
			if (lz - zz >= 0 && lz - zz <= ZOM_ATTACK_DISTANCE
				&& zy >= ly - DOWN_Y_2 && zy <= ly + UP_Y_2)
			{
				mLeona.SetState(Leona::STATE::WOULD);
				mLeona.Would();
			}
		}
	}
}
void Clear::DrawScene()
{
	this->md3dImmediateContext->ClearRenderTargetView(this->mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	this->md3dImmediateContext->ClearDepthStencilView(this->mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//	this->md3dImmediateContext->RSSetState(RasterizerState::mCounterClockFrontRS);

	//-----------------draw map-----------------
	float cy = mCamera.GetPosition().y;
	float cz = mCamera.GetPosition().z;
	float minY = cy - MAP_PLAYER_HEIGHT*0.6f;
	float maxY = cy + MAP_PLAYER_HEIGHT*0.6f;

	float minZ = cz - MAP_PLAYER_HEIGHT*1.5f;
	float maxZ = cz + MAP_PLAYER_HEIGHT*1.5f;
	for (int i = 0; i < mCBoxs.size(); i++)
	{
		if (mCBoxs[i].Position.z> minZ && mCBoxs[i].Position.z < maxZ
			&& mCBoxs[i].Position.y> minY && mCBoxs[i].Position.y < maxY)
			DrawBasicModel(mCBoxs[i]);
	}

	//draw door
	for (int td = 0; td < mTransmitDoors.size(); td++)
	{
		for (int t = 0; t < 2; t++)
		{
			float y = mTransmitDoors[td].mDoors[t].mPos.y;
			float z = mTransmitDoors[td].mDoors[t].mPos.z;
			if (z + DOOR_WIDTH*0.5f>minZ && z - DOOR_WIDTH*0.5f <maxZ
				&& y + DOOR_HEIGHT*0.5f >minY && y - DOOR_HEIGHT*0.5f < maxY)
			{
				for (int i = 0; i < DOOR_BOX_COUNT; i++)
				{
					float by = mTransmitDoors[td].mDoors[t].mBoxs[i].Position.y;
					float bz = mTransmitDoors[td].mDoors[t].mBoxs[i].Position.z;
					if (bz> minZ && bz < maxZ
						&& by> minY && by < maxY)
						DrawBasicModel(mTransmitDoors[td].mDoors[t].mBoxs[i]);
				}
			}
		}
	}
	if (mlayerIndex == 0 && cz>MAX_MAP_LENGHT - MAP_UNIT_LENGHT)
	{
		for (int i = 0; i < DOOR_BOX_COUNT; i++)
		{
			DrawBasicModel(mVictoryDoor.mBoxs[i]);
		}
	}
	//------------------------------------------

	mSky.Draw(md3dImmediateContext, mCamera);

	//draw leona
	DrawSkinnedModel(mLeona);
	//this->md3dImmediateContext->RSSetState(RasterizerState::mWireframeRS);
	//DrawBasicModel(mLeona.mFootBox);
	//DrawBasicModel(mLeona.mBodyBox); 
	//this->md3dImmediateContext->RSSetState(0);

	//draw zombies
	XMFLOAT3 zpos;
	for (std::vector<Zombie>::iterator it = mZombies.begin(); it != mZombies.end(); it++)
	{
		if (it->IsDead())
			continue;
		zpos = it->GetPosition();
		if (zpos.y > minY && zpos.y<maxY && zpos.z>minZ && zpos.z < maxZ)
		{
			DrawSkinnedModel(*it);
		}
	}
	//this->md3dImmediateContext->RSSetState(RasterizerState::mWireframeRS);
	//for (int i = 0; i < 6; i++)
	//{
	//	DrawBasicModel(mZombies[0].mBox[i]);
	//}
	//this->md3dImmediateContext->RSSetState(0);z
	mBloodStrip.Draw(this->md3dImmediateContext,mLeona.GetBloodScaling());
	//--------------------------------------------------------------------------------------------------
	this->mSwapChain->Present(0, 0);

	////draw blood
	//HDC hdc = GetDC(this->MainWnd());
	//int rLeft = 0, rRight = 200, rBottom = 50, rTop = 0;

	//HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	//SelectObject(hdc, pen);
	//RoundRect(hdc, rLeft, rTop, rRight, rBottom, 3, 4);
	//DeleteDC(hdc);
}
void Clear::DrawBasicModel(BasicModelInstance& model)
{
	md3dImmediateContext->IASetInputLayout(InputLayout::mPosNormTex);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX ScalMat = ::XMMatrixScaling(model.GetScale(), model.GetScale(), model.GetScale());
	XMMATRIX World = XMLoadFloat4x4(&model.GetWorld());
	World = ScalMat*World;

	XMMATRIX WorldViewProj = World*mCamera.GetView()*mCamera.GetProj();
	XMMATRIX WorldInvTranspose = mCamera.InverseTranspose(World);

	BasicEffect::mWorld->SetMatrix(reinterpret_cast<float*>(&World));
	BasicEffect::mWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));
	BasicEffect::mWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&WorldInvTranspose));

	float texscaling = 1.0f;
	XMMATRIX TexTran = ::XMMatrixScaling(texscaling, texscaling, texscaling);
	BasicEffect::mTexTransform->SetMatrix(reinterpret_cast<float*>(&TexTran));
	BasicEffect::mDirLight->SetRawValue(&mDirectLight, 0, 3 * sizeof(DirectLight));
	BasicEffect::mEyePosW->SetRawValue(&mCamera.GetPosition(), 0, sizeof(XMFLOAT3));

	D3DX11_TECHNIQUE_DESC techDesc;
	BasicEffect::mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		model.Model->DrawIndex(this->md3dImmediateContext, BasicEffect::mTech, BasicEffect::mTechAlphaClip, p, BasicEffect::mMaterial, BasicEffect::mDiffuseMap);
	}
}
void Clear::DrawSkinnedModel(SkinnedModelInstance& model)
{
	md3dImmediateContext->IASetInputLayout(InputLayout::mPosNormalTexSkinned);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX ScalMat = ::XMMatrixScaling(model.GetScale(), model.GetScale(), model.GetScale());
	XMMATRIX World = XMLoadFloat4x4(&model.GetWorld());
	World = ScalMat*World;

	XMMATRIX WorldViewProj = World*mCamera.GetView()*mCamera.GetProj();
	XMMATRIX WorldInvTranspose = mCamera.InverseTranspose(World);

	AnimateEffect::mWorld->SetMatrix(reinterpret_cast<float*>(&World));
	AnimateEffect::mWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));
	AnimateEffect::mWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&WorldInvTranspose));

	float texscaling = 1.0f;
	XMMATRIX TexTran = ::XMMatrixScaling(texscaling, texscaling, texscaling);
	AnimateEffect::mTexTransform->SetMatrix(reinterpret_cast<float*>(&TexTran));
	AnimateEffect::mDirLight->SetRawValue(&mDirectLight, 0, 3 * sizeof(DirectLight));
	AnimateEffect::mEyePosW->SetRawValue(&mCamera.GetPosition(), 0, sizeof(XMFLOAT3));

	AnimateEffect::mBoneTransforms->SetMatrixArray(reinterpret_cast<float*>(&model.FinalTransforms[0]),
		0, model.FinalTransforms.size());

	D3DX11_TECHNIQUE_DESC techDesc;
	AnimateEffect::mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		model.Model->Draw(this->md3dImmediateContext, AnimateEffect::mTech, AnimateEffect::mTechAlphaClip, p, AnimateEffect::mMaterial, AnimateEffect::mDiffuseMap);
	}
}
void Clear::InitMapUnit()
{
	mBrick_1 = new BasicModel(md3dDevice, mTextureMgr, L"res/box/brick.dds", LENGHT, WIDTH, WIDTH);
	mBasicModels.push_back(mBrick_1);

	mBrick_2 = new BasicModel(md3dDevice, mTextureMgr, L"res/box/bricks.dds", LENGHT, WIDTH, WIDTH);
	mBasicModels.push_back(mBrick_2);

	mBrick_3 = new BasicModel(md3dDevice, mTextureMgr, L"res/box/checkboard.dds", LENGHT, WIDTH, WIDTH);
	mBasicModels.push_back(mBrick_3);

	mCrate = new BasicModel(md3dDevice, mTextureMgr, L"res/box/WoodCrate02.dds", LENGHT, WIDTH, WIDTH);
	mBasicModels.push_back(mCrate);

	//目的地
	mVictoryDoor.mIsVictoryDoor = true;
	mVictoryDoor.SetModel(mCrate);
	mVictoryDoor.SetBoxsPosition(0.0f, (MAX_LAYER_COUNT - 1)*MAP_PLAYER_HEIGHT + WIDTH*5.0f, MAP_LEFT + MAX_MAP_LENGHT - WIDTH*2.0f);
	for (int i = 0; i < DOOR_BOX_COUNT; i++)
	{
		mMapOccupy[Mp(mVictoryDoor.mBoxs[i].Position.y, mVictoryDoor.mBoxs[i].Position.z)] = true;
	}
	mVictoryDoor.ReSetDoorPosition();
}
void Clear::LoadMap()
{
	using namespace std;
	ifstream fr("Map.txt");
	if (!fr.is_open())
		return;

	int numTransmitDoor;
	int numBox;
	int numZombie;
	string ignore;

	fr >> ignore >> numTransmitDoor;
	fr >> ignore >> numBox;
	fr >> ignore >> numZombie;

	mTransmitDoors.resize(numTransmitDoor);
	mCBoxs.resize(numBox);

	for (int td = 0; td < numTransmitDoor; td++)
	{
		XMFLOAT3 pos;
		fr >> pos.x >> pos.y >> pos.z;
		mTransmitDoors[td].mDoors[0].SetModel(mBrick_3);
		mTransmitDoors[td].mDoors[0].SetBoxsPosition(pos);
		mTransmitDoors[td].mDoors[0].ReSetDoorPosition();


		fr >> pos.x >> pos.y >> pos.z;
		mTransmitDoors[td].mDoors[1].SetModel(mBrick_3);
		mTransmitDoors[td].mDoors[1].SetBoxsPosition(pos);
		mTransmitDoors[td].mDoors[1].ReSetDoorPosition();

		for (int t = 0; t < 2; t++)
		{
			for (int i = 0; i < DOOR_BOX_COUNT; i++)
			{
				mMapOccupy[Mp(mTransmitDoors[td].mDoors[t].mBoxs[i].Position.y, mTransmitDoors[td].mDoors[t].mBoxs[i].Position.z)] = true;
			}
		}
	}

	for (int i = 0; i < numBox; i++)
	{
		int index;
		float x, y, z;
		fr >> index >> x >> y >> z;
		mCBoxs[i].mModelIndex = index;
		mCBoxs[i].SetModel(mBasicModels[index]);
		mCBoxs[i].SetPosition(x*OBJ_SCALE, y*OBJ_SCALE, z*OBJ_SCALE);

		mMapOccupy[Mp(y, z)] = 1;
	}
	Zombie zombie;
	zombie.SetModel(mZombieModel);
	for (int i = 0; i < numZombie; i++)
	{
		float x, y, z;
		fr >> x >> y >> z;
		if (y >= MAP_BUTTOM && y <= MAP_TOP)
		{
			zombie.SetPosition(x, y, z);
			zombie.UpdatePosition(0.0f);
			zombie.Walk_1();
			zombie.InitRandomNum(i);
			mZombies.push_back(zombie);
		}
	}
}

void Clear::ConverPos(Mp& mp, CollidePoint& cp)
{
	int indexY = (int)(cp.y / WIDTH);
	int indexZ = (int)(cp.z / WIDTH);
	if (cp.y < 0.0f)
	{
		mp.y = indexY*WIDTH - WIDTH / 2.0f;
	}
	else
	{
		mp.y = indexY*WIDTH + WIDTH / 2.0f;
	}
	if (cp.z < 0.0f)
	{
		mp.z = indexZ*WIDTH - WIDTH / 2.0f;
	}
	else
	{
		mp.z = indexZ*WIDTH + WIDTH / 2.0f;
	}
}
void Clear::LeonaCollisionDetection()
{
	//----------------------leona------------------
	if (mLeona.GetPosition().y <= (MAX_LAYER_COUNT - mlayerIndex - 1)*MAP_PLAYER_HEIGHT)
	{
		mLeona.Dead();
		return;
	}
	if (mLeona.GetVerticalState() == Leona::VERTICAL_STATE::UP)
	{//向上检测
		Mp tlmp, tcmp, trmp;//上面三个碰撞点标记
		ConverPos(tlmp, mLeona.mLeftBodyCp[0]);
		ConverPos(tcmp, mLeona.mTopCp);
		ConverPos(trmp, mLeona.mRightBodyCp[0]);

		if (mLeona.mLeftBodyCp[0].y >= tlmp.y
			|| mLeona.mTopCp.y >= tcmp.y
			|| mLeona.mRightBodyCp[0].y >= trmp.y)
		{//可能有碰撞

			//检测上面一块位置是否被占用
			tlmp.y += WIDTH;
			tcmp.y += WIDTH;
			trmp.y += WIDTH;

			if (IsPositionOccupied(tlmp)
				|| IsPositionOccupied(tcmp)
				|| IsPositionOccupied(trmp))
			{
				//碰撞
				mLeona.SetVerticalStateDown();
				mLeona.SetUpSpeed(-mLeona.GetUpSeed());
				//	mLeona.SetPositionY(tlmp.y-mLeona.TOP_CP_OFFSET);
			}
		}
	}
	else
	{//向下检测
		Mp lmp, rmp, cmp, ltmp;

		//向下时脚部增加一个碰撞点，使得即使只有脚的一小部分也可站立
		CollidePoint left = mLeona.mLeftFootCp;
		CollidePoint right = mLeona.mRightFootCp;
		CollidePoint center = mLeona.mLeftFootCp;
		left.z -= WIDTH*0.5f;
		right.z += WIDTH*0.5f;
		center.z += WIDTH*0.5f;

		ConverPos(lmp, left);
		ConverPos(rmp, right);
		ConverPos(cmp, center);

		//是否已陷入障碍
		if (IsPositionOccupied(lmp) || IsPositionOccupied(cmp) || IsPositionOccupied(rmp))
		{
			/*		lmp.y += WIDTH;
					cmp.y = lmp.y;
					rmp.y = lmp.y;
					float oy =lmp.y - mLeona.mLeftFootCp.y;
					mLeona.MovePosition(0.0f, oy, 0.0f);*/
			mLeona.SetVerticalStateLevel();
		}
		else
		{


			if (mLeona.mLeftFootCp.y > lmp.y && mLeona.mRightFootCp.y > rmp.y)
			{
				mLeona.SetVerticalStateDown();
			}
			else
			{
				//下面一块位置
				lmp.y -= WIDTH;
				rmp.y -= WIDTH;
				cmp.y -= WIDTH;

				if (IsPositionOccupied(lmp) || IsPositionOccupied(rmp) || IsPositionOccupied(cmp))
				{
					mLeona.SetPositionY(lmp.y + WIDTH*0.5f);
					mLeona.ConselJump();
					mLeona.SetVerticalStateLevel();

				}
				else
				{
					mLeona.SetVerticalStateDown();
				}

				if (mLeona.GetVerticalState() == Leona::VERTICAL_STATE::DOWN)
				{//保证下落时盾牌不会嵌入障碍
					if (mLeona.GetDirection() == Leona::DIRECT::NZ)//向左
					{

						ConverPos(ltmp, mLeona.mLeftBodyCp[3]);

						ltmp.y -= WIDTH;
						if (IsPositionOccupied(ltmp))
						{
							//盾牌碰撞
							float oz = ltmp.z + WIDTH - mLeona.mLeftBodyCp[3].z;
							mLeona.MovePosition(0.0f, 0.0f, oz);
						}
					}
					else
					{
						ConverPos(ltmp, mLeona.mRightBodyCp[3]);
						ltmp.y -= WIDTH;
						if (IsPositionOccupied(ltmp))
						{
							//盾牌碰撞
							float oz = ltmp.z - WIDTH - mLeona.mRightBodyCp[3].z;
							mLeona.MovePosition(0.0f, 0.0f, oz);
						}
					}
				}
			}
		}
	}

	if (mA)
	{//向左检测
		mLeona.SetIsAbleMoveTrue();//先设为能动

		Mp mp[5];
		bool isIn = false;//是否已陷入障碍
		for (int i = 0; i < 4; i++)
		{
			ConverPos(mp[i], mLeona.mLeftBodyCp[i]);
			if (IsPositionOccupied(mp[i]))
				isIn = true;
		}
		ConverPos(mp[4], mLeona.mLeftFootCp);
		if (IsPositionOccupied(mp[4]))
			isIn = true;
		if (isIn)
		{
			for (int i = 0; i < 5; i++)
				mp[i].z += WIDTH;
			float oz = mp[0].z - mLeona.mLeftBodyCp[0].z;
			mLeona.MovePosition(0.0f, 0.0f, oz);
		}


		bool is = false;
		//先检测 bodycp
		for (int i = 0; i < 4; i++)
		{
			if (mLeona.mLeftBodyCp[i].z <= mp[i].z)
			{
				is = true;
				break;
			}
		}
		if (is)
		{//可能碰撞
			bool isCollid = false;
			int index;
			for (index = 0; index < 4; index++)
			{
				mp[index].z -= WIDTH;  //左边一块位置
				if (IsPositionOccupied(mp[index]))
				{
					isCollid = true;
					break;
				}
			}
			if (isCollid)
			{
				mLeona.SetIsAbleMoveFalse();
				//		float oz = mp[index].z + WIDTH - mLeona.mLeftBodyCp[index].z; //+WIDTH 变为原来的位置
				//		mLeona.MovePosition(0.0f, 0.0f, oz);
			}
		}
		//检测脚部
		if (mLeona.mLeftFootCp.z <= mp[4].z)
		{
			mp[4].z -= WIDTH;
			if (IsPositionOccupied(mp[4]))
			{
				mLeona.SetIsAbleMoveFalse();
			}
		}

	}
	if (mD)
	{//向右检测
		mLeona.SetIsAbleMoveTrue();//先设为能动

		Mp mp[5];
		bool isIn = false;//是否已陷入障碍
		for (int i = 0; i < 4; i++)
		{
			ConverPos(mp[i], mLeona.mRightBodyCp[i]);
			if (IsPositionOccupied(mp[i]))
				isIn = true;
		}
		ConverPos(mp[4], mLeona.mRightFootCp);
		if (IsPositionOccupied(mp[4]))
			isIn = true;
		if (isIn)
		{
			for (int i = 0; i < 5; i++)
				mp[i].z -= WIDTH;
			float oz = mp[0].z - mLeona.mRightBodyCp[0].z;
			mLeona.MovePosition(0.0f, 0.0f, oz);
		}


		bool is = false;
		//先检测 bodycp
		for (int i = 0; i < 4; i++)
		{
			if (mLeona.mRightBodyCp[i].z >= mp[i].z)
			{
				is = true;
				break;
			}
		}
		if (is)
		{//可能碰撞
			bool isCollid = false;
			int index;
			for (index = 0; index < 4; index++)
			{
				mp[index].z += WIDTH;  //左边一块位置
				if (IsPositionOccupied(mp[index]))
				{
					isCollid = true;
					break;
				}
			}
			if (isCollid)
			{
				mLeona.SetIsAbleMoveFalse();
			}
		}
		//检测脚部
		if (mLeona.mRightFootCp.z >= mp[4].z)
		{
			mp[4].z += WIDTH;
			if (IsPositionOccupied(mp[4]))
			{
				mLeona.SetIsAbleMoveFalse();
			}
		}

	}
}
void Clear::ZombieCollisionDetection(Zombie& zombie)
{
	if (zombie.GetVerticalState() == Zombie::VERTICAL_STATE::DOWN)
	{
		Mp mp;
		ConverPos(mp, zombie.mCP[0]);
		if (IsPositionOccupied(mp))
		{
			//已经陷入障碍
			zombie.SetPositionY(mp.y + WIDTH*0.5f);
			zombie.SetVerticalStateLevel();
		}
		else
		{
			if (mp.y < zombie.mCP[0].y)
			{
				zombie.SetVerticalStateDown();
			}
			else
			{
				mp.y -= WIDTH;
				if (IsPositionOccupied(mp))
				{
					zombie.SetVerticalStateLevel();
					zombie.SetPositionY(mp.y + WIDTH*0.5f);
				}
				else
				{
					zombie.SetVerticalStateDown();
				}
			}
		}
	}
	else
	{//LEVEL
		if (zombie.IsMove())
		{
			zombie.mIsAbleMove = true;

			Mp mp[6];
			//计算前方 mp 并检测前方是否有障碍物
			bool is = false;
			for (int i = 0; i < 6; i++)
			{
				ConverPos(mp[i], zombie.mCP[i]);
				mp[i].z += zombie.GetDirection()*WIDTH;
				if (IsPositionOccupied(mp[i]))
					is = true;
			}
			if (is)
			{
				//有障碍
				if (zombie.IsFindEnemy())
				{
					zombie.mIsAbleMove = false;
				}
				else
				{
					zombie.ChangeDirection();
				}
			}
			else
			{
				//检测脚下是否有路可走
				mp[0].y -= WIDTH; //前方脚下mp
				if (!IsPositionOccupied(mp[0]))
				{
					//没有路
					if (zombie.IsFindEnemy())
					{
						zombie.mIsAbleMove = false;
					}
					else
					{
						zombie.ChangeDirection();
					}
				}
			}
		}
	}
}
bool Clear::IsPositionOccupied(Mp& mp)
{
	std::map<Mp, int>::iterator it;

	it = mMapOccupy.find(mp);
	if (it != mMapOccupy.end())
		return true;
	return false;
}
void Clear::RemoveOccupied(Mp& mp)
{
	//删除原来位置的占用标识
	std::map<Mp, int>::iterator it;
	it = mMapOccupy.find(mp);
	if (it != mMapOccupy.end())
		mMapOccupy.erase(it);
}
void Clear::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);

	if ((btnState & MK_RBUTTON) != 0)
	{
		//		mRightMouseDown = true;
	}
	if ((btnState & MK_LBUTTON) != 0)
	{
		//		mLeftMouseDown = true;
	}
}
void Clear::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{
		//		mRightMouseDown = false;
	}
	if ((btnState & MK_LBUTTON) == 0)
	{
		//		mLeftMouseDown = false;
	}
	ReleaseCapture();
}
void Clear::OnMouseMove(WPARAM btnState, int x, int y)
{


	// Make each pixel correspond to a quarter of a degree.
	float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
	float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
void Clear::OnMouseWheel(WPARAM wheelState)
{
	short zDelta = (short)HIWORD(wheelState);

	float delta = 0.15f;
	if (zDelta > 0);
	//   	mCamera.Scale(delta);
	if (zDelta < 0);
	//  	mCamera.Scale(-delta);

}
void Clear::OnKeyDown(WPARAM wParam)
{
	switch (wParam)
	{
	case 'W':
		mW = 1;
		break;
	case 'S':
		mS = 1;
		break;
	case 'A':
		mA = 1;
		break;
	case 'D':
		mD = 1;
		break;
	case 'E':
		mE = 1;
		break;
	case 'Q':
		mQ = 1;
		break;
	case VK_SHIFT:
		mSHIFT = 1;
		break;
	case VK_SPACE:
		mSPACE = 1;
		break;
	case 'I':
		mI = 1;
		break;
	case 'K':
		mK = 1;
		break;
	case 'J':
		mJ = 1;
		break;
	case 'L':
		mL = 1;
		break;
	}
}
void Clear::OnKeyUp(WPARAM wParam)
{
	switch (wParam)
	{
	case 'W':
		mW = 0;
		break;
	case 'S':
		mS = 0;
		break;
	case 'A':
		mA = 0;
		break;
	case 'D':
		mD = 0;
		break;
	case 'E':
		mE = 0;
		break;
	case 'Q':
		mQ = 0;
		break;
	case VK_SHIFT:
		mSHIFT = 0;
		break;
	case VK_SPACE:
		mSPACE = 0;
		break;
	case 'I':
		mI = 0;
		break;
	case 'K':
		mK = 0;
		break;
	case 'J':
		mJ = 0;
		break;
	case 'L':
		mL = 0;
		break;
	}
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	Clear clear(hInstance);
	if (!clear.Init())
		return -1;
	return clear.Run();
}
