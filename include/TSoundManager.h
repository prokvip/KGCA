#pragma once
#include "TSound.h"
class TSoundManager : public TSingleton<TSoundManager>
{
	FMOD::System* m_pSystem = nullptr;
	std::list<std::wstring>  m_fileList;
private:
	friend class TSingleton<TSoundManager>;
	std::map<std::wstring, TSound*> m_List;
public:
	T_STR		GetSplitName(std::wstring name);
	void		LoadDir(std::wstring path);
	void		LoadAll(std::wstring path);
	bool	    Init();
	bool		Frame();
	TSound*		Load(std::wstring filename);
	bool	    Release();
	TSound* GetPtr(W_STR name);
private:
	TSoundManager();
public:
	~TSoundManager();
};

#define I_Sound TSoundManager::GetInstance()
