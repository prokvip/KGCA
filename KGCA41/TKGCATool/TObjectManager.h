#pragma once
#include "TCharacter.h"
class TObjectManager : public TSingleton<TObjectManager>
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
private:
	friend class TSingleton<TObjectManager>;
	std::map<std::wstring, TCharacter*> m_List;
	std::map<std::wstring, TFbxFile*> m_fbxList;
public:
	TFbxFile*   LoadFbx(std::wstring name);
	TCharacter*	Load(std::wstring fbxfilename);

	TFbxFile*   FindFbx(std::wstring name);
	TCharacter* FindCharacter(std::wstring name);
	TCharacter*	Find(std::wstring name);
	bool		Release();
	W_STR		GetSplitName(std::wstring fullpath);
private:
	TObjectManager();
public:
	~TObjectManager();
};

#define I_Object TObjectManager::GetInstance()
