#pragma once
#include "TBasisStd.h"
enum STAGE_TYPE { t_VS, t_HS, t_DS, t_GS, t_PS, t_CS };

class TDxShader
{
public:
	T_STR		m_szName;
	T_STR		m_szPath;
	int					m_iIndex;
	ID3DBlob*			m_pBuffer;
	ID3DBlob*			m_pBlobVS;
public:
	//--------------------------------------------------------------------------------------
	// VS & PS Variables
	//--------------------------------------------------------------------------------------
	ID3D11VertexShader*         m_pVertexShader;//¡§¡°Ω¶¿Ã¥ı	
	ID3D11HullShader*			m_pHullShader;//«Ê Ω¶¿Ã¥ı
	ID3D11DomainShader*			m_pDomainShader;//µµ∏ﬁ¿ŒΩ¶¿Ã¥ı
	ID3D11GeometryShader*       m_pGeometryShader;//±‚«œΩ¶¿Ã¥ı
	ID3D11PixelShader*          m_pPixelShader;//«»Ω©Ω¶¿Ã¥ı
	ID3D11ComputeShader*		m_pComputeShader;//ƒƒ«ª≈ÕΩ¶¿Ã¥ı


	void			Apply(ID3D11DeviceContext*	pd3dContext);
	HRESULT			Load(ID3D11Device* pd3dDevice,
		TCHAR* pLoadShaderFile,
		CHAR* pEntryPoint,
		CHAR* pCompiler,
		STAGE_TYPE type = t_VS,
		ID3DBlob* pLoadBlob = 0);
	void			ApplyVS(ID3D11DeviceContext*	pd3dContext, bool bUse = true);
	void			ApplyHS(ID3D11DeviceContext*	pd3dContext, bool bUse = true);
	void			ApplyDS(ID3D11DeviceContext*	pd3dContext, bool bUse = true);
	void			ApplyGS(ID3D11DeviceContext*	pd3dContext, bool bUse = true);
	void			ApplyPS(ID3D11DeviceContext*	pd3dContext, bool bUse = true);
	void			ApplyCS(ID3D11DeviceContext*	pd3dContext, bool bUse = true);
	//--------------------------------------------------------------------------------------
	// ID3DX11EffectTechnique map
	//--------------------------------------------------------------------------------------
	HRESULT			Load(ID3D11Device* pDevice,
		const TCHAR* strShaderPath,
		LPCSTR strVSEntryPoint = 0,
		LPCSTR strPSEntryPoint = 0,
		LPCSTR strShaderModel = 0,
		UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS);

	//--------------------------------------------------------------------------------------
	// ∏‚πˆ «‘ºˆ
	//--------------------------------------------------------------------------------------
public:
	void					ClearShaderResources(ID3D11DeviceContext*  pImmediateContext);
	bool					Release();
	HRESULT					CompileShaderFromFile(const WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT					CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
public:
	TDxShader(void);
	virtual ~TDxShader(void);
};

class TShaderMgr : public TSingleton < TShaderMgr >
{
private:
	friend class TSingleton<TShaderMgr>;
	ID3D11Device*			m_pd3dDevice;
public:
	typedef map <INT, TDxShader*>					TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;


public:
	void			SetDevice(ID3D11Device*	 pDevice);
	INT				Add(const TCHAR *pFileName, LPCSTR strVSEntryPoint = 0,
		LPCSTR strPSEntryPoint = 0,
		LPCSTR strShaderModel = 0);
	INT				Add(TCHAR* pLoadShaderFile,
		CHAR* pEntryPoint,
		CHAR* pCompiler,
		STAGE_TYPE type = t_VS);
	TDxShader*		GetPtr(INT iIndex);
	bool			Release();
	bool			Delete(INT iDelete);
public:
	TShaderMgr();
	virtual ~TShaderMgr();
};
#define I_Shader TShaderMgr::GetInstance()