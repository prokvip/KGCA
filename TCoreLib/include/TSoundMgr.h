#pragma once
#include "TStd.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment( lib, "fmod_vc.lib")

class TSound
{
public:
	std::wstring   m_csName;
	std::wstring   m_csPath;
public:
	FMOD::System*	m_pSystem;
	//  사운드파일단위로 필요하다.
	FMOD::Sound*	m_pSound;
	FMOD::Channel*	m_pChannel;
public:
	void  SetName(std::wstring key)
	{
		m_csName = key;
	};
	void  Set(FMOD::System* pSystem) { m_pSystem = pSystem; }
	bool  Load(std::wstring filename);
	bool  Init();
	bool  Frame();	
	bool  Release();
public:
	void  Play(bool bLoop=true);
	void  PlayEffect();
	void  Stop();
	void  Paused();
	void  Volume();
	void  VolumeUp();
	void  VolumeDown();
};
//std::unique_ptr<TSound> NewFun()
//{
//	return std::make_unique<TSound>();
//}
class TSoundMgr
{
	FMOD::System* m_pSystem;
	using tList = std::map< std::wstring,
							std::unique_ptr<TSound>>;
public:
	static TSoundMgr& GetInstance()
	{
		static TSoundMgr input;
		return input;
	}
	tList   m_list;
public:
	bool  Frame();
	bool  Release();
	TSound* Load(std::wstring szFilepath);
	TSound* GetPtr(std::wstring key);
	bool  Get(std::wstring key, TSound& ret);
private:
	TSoundMgr();
public:
	virtual ~TSoundMgr();
};

#define I_Sound TSoundMgr::GetInstance()

