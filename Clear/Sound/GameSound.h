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

	HRESULT             result; //�������ܽ�����ķ���ֵ
	LPDIRECTSOUND       pDS;  //����������DirectSound����
	LPDIRECTSOUNDBUFFER pMainBuf;  //������������ָ��
	DSBUFFERDESC        desc;  //���������ṹ��������ʼ����������
	WAVEFORMATEX        pwfmt;  //���������ṹ�������趨���Ÿ�ʽ

    WAVEFORMATEX        swfmt;  //���������ṹ
	MMCKINFO            ckRiff;  //RIFF�������Ϣ
	MMCKINFO            ckInfo;  //���������Ϣ
	MMRESULT            mmresult;  //���ؽ��
	DWORD               size;   //ʵ�����ϵĴ�С
	HMMIO               hbackground;  //�򿪵Ķ�ý���ļ�
public:
	GameSound();
	void GameSoundInit(HWND);      //GameSound����Ľ���   0
	void GameSoundbufferConstruct();   //�������Ĵ���     0
	void GameSoundfmtSet(int,int,int);   //ͨ����������ָ�������ò��Ÿ�ʽ
	void GameSoundReadWAVfile(wchar_t*, HMMIO&);  //�������ļ����벢����ϸ����HMMIO�ṹ��  0
	void GameSoundReadinbuffer(LPDIRECTSOUNDBUFFER&, wchar_t*);   //�������ļ�����λ�������  0

	LPDIRECTSOUNDBUFFER pStartmusic;   //�����ӻ�����ָ��
	LPDIRECTSOUNDBUFFER pBaground;
	LPDIRECTSOUNDBUFFER pWalkmusic;
	LPDIRECTSOUNDBUFFER pHuijian;
	LPDIRECTSOUNDBUFFER pHuijianjizhong;   
	LPDIRECTSOUNDBUFFER pChi;
	LPDIRECTSOUNDBUFFER pChizhong;
	LPDIRECTSOUNDBUFFER pDianjuQidong;
	LPDIRECTSOUNDBUFFER pLeonaHitted;
    
	void GameSoundSomestop ();
	void GameSoundAllstop();   //for�������֣��ñ������ָ���ʱ����ǰ���������ֶ�ֹͣ���Ӷ������µ�����
	void GameMusicplay(LPDIRECTSOUNDBUFFER&);  //��������ѭ������
	void GameSoundplay(LPDIRECTSOUNDBUFFER&);	//��������һ������Ч


	//һЩ���
	bool isWalk;
	int  jiZhong;  //�ӽ�����Ϊ1������Ϊ2������Ϊ0
	double huijianTime;
	long chiTime;
	bool isHui;
	bool isCi;

	const int HUI_JIAN_ATT = 1;
	const int CI_ATT = 2;
	const int NOT_ATT = 0;
};

#endif // !defined(AFX_GAMESOUND_H__56321890_FB26_4FE5_BF1B_E99CDD291A25__INCLUDED_)
