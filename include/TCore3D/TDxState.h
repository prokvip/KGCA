#include "TStd.h"

class TDxState
{
public:
public:
	static ID3D11BlendState* m_AlphaBlend;
	static ID3D11BlendState* m_AlphaBlendDisable;
	static ID3D11SamplerState* m_pSamplerState;
	static ID3D11RasterizerState* g_pRSBackCullSolid;

	static  ID3D11DepthStencilState* g_pDSSDepthEnable;
public:
	static bool SetState(ID3D11Device* pd3dDevice);
	static bool Release();
};