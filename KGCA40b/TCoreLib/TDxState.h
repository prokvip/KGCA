#include "TStd.h"

class TDxState
{
public:
	static ID3D11BlendState* m_AlphaBlend;
	static ID3D11BlendState* m_AlphaBlendDisable;
	static ID3D11BlendState* m_BSNoneColor;
	static ID3D11SamplerState* m_pSSLinear;
	static ID3D11SamplerState* m_pSSPoint;
	static ID3D11SamplerState* g_pSSMirrorLinear;
	static ID3D11SamplerState* g_pSSMirrorPoint;
	static ID3D11SamplerState* g_pSSClampLinear;
	static ID3D11SamplerState* g_pSSClampPoint;
	static ID3D11SamplerState* g_pSSShadowMap;

	static ID3D11RasterizerState* g_pRSBackCullWireFrame;
	static ID3D11RasterizerState* g_pRSNoneCullWireFrame;
	static ID3D11RasterizerState* g_pRSBackCullSolid;
	static ID3D11RasterizerState* g_pRSNoneCullSolid;
	static ID3D11RasterizerState* g_pRSSlopeScaledDepthBias;

	static  ID3D11DepthStencilState* g_pDSSDepthEnable;
	static  ID3D11DepthStencilState* g_pDSSDepthDisable;
	static  ID3D11DepthStencilState* g_pDSSDepthEnableWriteDisable;
	static  ID3D11DepthStencilState* g_pDSSDepthDisableWriteDisable;
public:
	static bool SetState(ID3D11Device* pd3dDevice);
	static bool Release();
};
static void ApplyRS(ID3D11DeviceContext* pContext,
	ID3D11RasterizerState* pState)
{
	assert(pContext);
	pContext->RSSetState(pState);
}
static void ApplyDSS(ID3D11DeviceContext* pContext,
	ID3D11DepthStencilState* pDepthStencilState,
	UINT iRef = 0x01)
{
	assert(pContext);
	pContext->OMSetDepthStencilState(pDepthStencilState, iRef);

};
static void ApplyBS(ID3D11DeviceContext* pContext,
	ID3D11BlendState* pBlendState,
	const FLOAT fBlendFactor[] = 0,
	UINT iMask = 0xffffffff)
{
	assert(pContext);
	pContext->OMSetBlendState(pBlendState, fBlendFactor, iMask);
}

static void ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pSamplerState,
	UINT iSlot = 0, UINT iArray = 1)
{
	assert(pContext);
	pContext->PSSetSamplers(iSlot, iArray, &pSamplerState);
}