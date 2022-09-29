#pragma once
//https://www.fmod.com/download#fmodengine
//C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\core
// inc, lib   -> core 에 복사
#include "TStd.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment (lib, "fmod_vc.lib")

class TSound
{
public:
	std::wstring    m_szName;
public:
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Sound*	m_pSound = nullptr;// 사운드파일
	FMOD::Channel*  m_pChannel = nullptr;
	float			m_fVolume;
	unsigned int    m_TotalTime;
	std::wstring    m_szBuffer;
public:
	void Paused();
	void VolumeUp(float fVolume=0.1f);
	void VolumeDown(float fVolume=0.1f);
	bool Play(bool bLoop=false);
	bool PlayEffect(bool bLoop = false);
	void Stop();
	void SetLoop(bool bLoop=false);
	bool IsPlay();
public:
	virtual bool		Load(
		FMOD::System* pSystem, 
		std::wstring filename);
	virtual bool		Init() ;
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release() ;
};

