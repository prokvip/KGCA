#pragma once
#include "TStd.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib,"fmod_vc.lib")
class TSound
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	FMOD::System*   m_pSystem = nullptr;
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;
public:	
	void	Set(FMOD::System* pSystem, std::wstring name, int iIndex);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	void    Play();
	void    Stop();
	void    Paused();
	void    Volume();
public:
	TSound();
	virtual ~TSound();
};

class TSoundMgr : public TSingleton<TSoundMgr>
{
	int		m_iIndex;
public:
	friend class TSingleton<TSoundMgr>;
	FMOD::System* m_pSystem = nullptr;
	std::map<std::wstring, TSound* >  m_list;
public:
	TSound*     Load(std::string filename);
	TSound*		GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	TSoundMgr();
	~TSoundMgr();
};

#define I_Sound TSoundMgr::Get()