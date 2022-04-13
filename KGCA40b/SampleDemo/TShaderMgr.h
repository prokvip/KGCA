#pragma once
#include "TBaseMgr.h"
#include <d3dcompiler.h>
class TShader
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader*  m_pPixelShader = nullptr;
	ID3DBlob* m_pVSCodeResult = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSCodeResult = nullptr;
public:	
	virtual bool    CreateVertexShader(ID3D11Device* pd3dDevice, 
		std::wstring filename, std::string entry="VS");
	virtual bool    CreatePixelShader(ID3D11Device* pd3dDevice, 
		std::wstring filename, std::string entry="PS");
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
public:
	TShader();
	virtual ~TShader();
};

class TShaderMgr :  public TBaseMgr<TShader, TShaderMgr>
{
	friend class TSingleton<TShaderMgr>;
public:
	TShader*	CreateVertexShader(ID3D11Device* pd3dDevice,
						std::wstring filename, std::string entry);
	TShader*	CreatePixelShader(ID3D11Device* pd3dDevice,
						std::wstring filename, std::string entry);
private:
	TShaderMgr();
public:
	~TShaderMgr();
};
#define I_Shader TShaderMgr::Get()