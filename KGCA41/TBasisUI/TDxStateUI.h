#pragma once
#include "TBasisUIStd.h"
namespace TUI
{
	class TBASISUI_API TDxStateUI
	{
	public:
		static ID3D11SamplerState* g_pUI_WrapSS;
		static ID3D11SamplerState* g_pUI_MirrorSS;
		static ID3D11RasterizerState* g_pUI_WireFrameRS;
		static ID3D11RasterizerState* g_pUI_SolidRS;
		static ID3D11BlendState* g_pUI_AlphaBlend;

		static  ID3D11DepthStencilState* g_pDSSDepthEnable;
		static  ID3D11DepthStencilState* g_pDSSDepthDisable;
		static  ID3D11DepthStencilState* g_pDSSDepthEnableNoWrite;
		static  ID3D11DepthStencilState* g_pDSSDepthDisableNoWrite;
		static  ID3D11DepthStencilState* g_pDSSDepthAlways;
		static bool SetState(ID3D11Device* pd3dDevice);
		static bool Release();
	};
};