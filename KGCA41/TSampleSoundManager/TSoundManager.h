#pragma once
#include "TSound.h"
class TSoundManager : public TSingleton<TSoundManager>
{
	FMOD::System* m_pSystem = nullptr;
private:
	friend class TSingleton<TSoundManager>;
	std::map<std::wstring, TSound*> m_List;
public:
	bool	   Init();
	bool		Frame();
	TSound*		Load(std::wstring filename);
	bool	  Release();
private:
	TSoundManager();
public:
	~TSoundManager();
};

#define I_Sound TSoundManager::GetInstance()
