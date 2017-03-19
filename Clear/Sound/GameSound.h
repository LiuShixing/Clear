// GameSound.h: interface for the GameSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESOUND_H__56321890_FB26_4FE5_BF1B_E99CDD291A25__INCLUDED_)
#define AFX_GAMESOUND_H__56321890_FB26_4FE5_BF1B_E99CDD291A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include<Windows.h>
#include "mmsystem.h"
#include "dsound.h"
#include "windows.h"
#include "mmreg.h"
#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"dsound.lib")
//#ifndef GAMESOUND
//#define GAMESOUND

class GameSound  
{
private:
//	virtual ~GameSound();
	HWND                soundhwnd;

	HRESULT             result; //用来接受建立后的返回值
	LPDIRECTSOUND       pDS;  //代表声卡的DirectSound对象
	LPDIRECTSOUNDBUFFER pMainBuf;  //声明主缓冲区指针
	DSBUFFERDESC        desc;  //声明描述结构，用来初始化缓冲区域
	WAVEFORMATEX        pwfmt;  //声明声音结构，用来设定播放格式

    WAVEFORMATEX        swfmt;  //声明声音结构
	MMCKINFO            ckRiff;  //RIFF区块的信息
	MMCKINFO            ckInfo;  //子区块的信息
	MMRESULT            mmresult;  //返回结果
	DWORD               size;   //实际资料的大小
	HMMIO               hbackground;  //打开的多媒体文件
public:
	GameSound();
	void GameSoundInit(HWND);      //GameSound对象的建立   0
	void GameSoundbufferConstruct();   //缓冲区的创建     0
	void GameSoundfmtSet(int,int,int);   //通过主缓冲区指针来设置播放格式
	void GameSoundReadWAVfile(wchar_t*, HMMIO&);  //将声音文件读入并将明细存在HMMIO结构中  0
	void GameSoundReadinbuffer(LPDIRECTSOUNDBUFFER&, wchar_t*);   //将声音文件读入次缓冲区中  0

	LPDIRECTSOUNDBUFFER pStartmusic;   //声明子缓冲区指针
	LPDIRECTSOUNDBUFFER pBaground;
	LPDIRECTSOUNDBUFFER pWalkmusic;
	LPDIRECTSOUNDBUFFER pHuijian;
	LPDIRECTSOUNDBUFFER pHuijianjizhong;   
	LPDIRECTSOUNDBUFFER pChi;
	LPDIRECTSOUNDBUFFER pChizhong;
	LPDIRECTSOUNDBUFFER pDianjuQidong;
	LPDIRECTSOUNDBUFFER pLeonaHitted;
    
	void GameSoundSomestop ();
	void GameSoundAllstop();   //for背景音乐，让背景音乐更换时，先前的所有音乐都停止，从而播放新的音乐
	void GameMusicplay(LPDIRECTSOUNDBUFFER&);  //用来播放循环音乐
	void GameSoundplay(LPDIRECTSOUNDBUFFER&);	//用来播放一次性音效


	//一些标记
	bool isWalk;
	int  jiZhong;  //挥剑击中为1，刺中为2，不中为0
	double huijianTime;
	long chiTime;
	bool isHui;
	bool isCi;

	const int HUI_JIAN_ATT = 1;
	const int CI_ATT = 2;
	const int NOT_ATT = 0;
};

#endif // !defined(AFX_GAMESOUND_H__56321890_FB26_4FE5_BF1B_E99CDD291A25__INCLUDED_)
