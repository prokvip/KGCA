#pragma once
#include "TStd.h"
#include <d3dcompiler.h>
class TShader
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader*  m_pPixelShader;
	ID3DBlob* m_pVSCodeResult = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSCodeResult = nullptr;
public:	
	virtual bool    CreateVertexShader(ID3D11Device* pd3dDevice, std::wstring filename);
	virtual bool    CreatePixelShader(ID3D11Device* pd3dDevice, std::wstring filename);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
public:
	TShader();
	virtual ~TShader();
};

class TShaderMgr : public TSingleton<TShaderMgr>
{
	int		m_iIndex;
public:
	friend class TSingleton<TShaderMgr>;
	std::map<std::wstring, TShader* >  m_list;
	ID3D11Device* m_pd3dDevice;
	void		  Set(ID3D11Device* pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}
public:
	TShader*     Load(std::wstring filename);
	TShader*		GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	TShaderMgr();
public:
	~TShaderMgr();
};

#define I_Shader TShaderMgr::Get()