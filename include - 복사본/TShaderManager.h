#pragma once
#include "TShader.h"
class TShaderManager : public TSingleton<TShaderManager>
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
private:
	friend class TSingleton<TShaderManager>;
	std::map<std::wstring, TShader*> m_List;
public:
	TShader* Load(std::wstring name);
	TShader* VLoad(std::wstring name, std::string funName = "VS");
	TShader* PLoad(std::wstring name, std::string funName = "PS");
	bool	  Release();
private:
	TShaderManager();
public:
	~TShaderManager();
};

#define I_Shader TShaderManager::GetInstance()
