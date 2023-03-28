#pragma once
#include "TDxRT.h"
#include "./wic/WICTextureLoader.h"
#include "./wic/DDSTextureLoader.h"

namespace DX
{
	////////////////////////// 아래의 경고가 있을 경우 사용한다.
	// 이와 같은 경고는 이미 쉐이더 파이프라인에 할당(리소스 및 상태값들이)되어 사용 중일 경우에 발생한다.
	//D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Resource being set to SO buffer slot 0 is still bound on input![STATE_SETTING WARNING #10: DEVICE_SOSETTARGETS_HAZARD]
	//D3D11 WARNING : ID3D11DeviceContext::SOSetTargets : Forcing Vertex Buffer slot 0 to NULL.[STATE_SETTING WARNING #1: DEVICE_IASETVERTEXBUFFERS_HAZARD]
	void		ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);

	HRESULT	CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT D3DX11CompileFromFile(LPCWSTR pSrcFile, CONST D3D_SHADER_MACRO* pDefines, LPD3DINCLUDE pInclude,LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
												/*ID3DX11ThreadPump* pPump, */ID3DBlob** ppShader, ID3DBlob** ppErrorMsgs, HRESULT* pHResult);
	ID3D11VertexShader* LoadVertexShaderFile(		ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut=nullptr, const char *pFuntionName=0, bool bBinary = false);
	ID3D11PixelShader*  LoadPixelShaderFile(		ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, const char *pFuntionName = 0, bool bBinary = false, ID3DBlob** pRetBlob = nullptr);
	ID3D11GeometryShader*  LoadGeometryShaderFile(	ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut=nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11HullShader* LoadHullShaderFile(			ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11DomainShader*  LoadDomainShaderFile(		ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11ComputeShader*  LoadComputeShaderFile(	ID3D11Device*  pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);


	ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device*  pd3dDevice, void *data, UINT iNumIndex, UINT iSize, bool bDynamic = false);

	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath);
	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext, const TCHAR* strFilePath);
	ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);

	class TDxObject
	{
	public:
		
		Microsoft::WRL::ComPtr<ID3D11Buffer>			g_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>			g_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>			g_pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>		g_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>		g_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>	g_pGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11HullShader>		g_pHullShader;
		Microsoft::WRL::ComPtr<ID3D11DomainShader>		g_pDomainShader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader>		g_pComputeShader;		
		Microsoft::WRL::ComPtr<ID3DBlob>				g_pVSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				g_pGSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				g_pPSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				g_pHSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				g_pDSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob>				g_pCSBlob;

		Microsoft::WRL::ComPtr<ID3D11InputLayout>			g_pInputlayout;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	g_pTextureSRV;	

		UINT					m_iPrimitiveType;
		UINT					m_iCullMode;
		UINT					m_iSamplerMode;
		UINT					m_iNumVertex;
		UINT					m_iNumIndex;
		UINT					m_iVertexSize;
		UINT					m_iIndexSize;
		D3D11_BOX				m_BoxVB;
		D3D11_BOX				m_BoxIB;
		UINT					m_iBeginVB;
		UINT					m_iBeginIB;
	public:
		void PreRender(ID3D11DeviceContext*    pContext, UINT iVertexSize = 0);
		void PostRender(ID3D11DeviceContext*    pContext,  UINT iCount = 0);
		bool Render(ID3D11DeviceContext*    pContext, UINT iVertexSize=0, UINT iCount=0);
	public:
		TDxObject();
		~TDxObject();
	};

	
}
