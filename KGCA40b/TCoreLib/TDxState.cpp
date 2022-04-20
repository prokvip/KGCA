#include "TDxState.h"
ID3D11BlendState* TDxState::m_AlphaBlend = nullptr;
ID3D11BlendState* TDxState::m_AlphaBlendDisable = nullptr;
ID3D11BlendState* TDxState::m_BSNoneColor = nullptr;
ID3D11SamplerState* TDxState::m_pSSLinear = nullptr;
ID3D11SamplerState* TDxState::m_pSSPoint = nullptr;
ID3D11SamplerState* TDxState::g_pSSMirrorLinear = nullptr;
ID3D11SamplerState* TDxState::g_pSSMirrorPoint = nullptr;
ID3D11SamplerState* TDxState::g_pSSClampLinear = nullptr;
ID3D11SamplerState* TDxState::g_pSSClampPoint = nullptr;
ID3D11SamplerState* TDxState::g_pSSShadowMap=nullptr;
ID3D11RasterizerState* TDxState::g_pRSBackCullSolid =nullptr;
ID3D11RasterizerState* TDxState::g_pRSNoneCullSolid = nullptr;
ID3D11RasterizerState* TDxState::g_pRSBackCullWireFrame = nullptr;
ID3D11RasterizerState* TDxState::g_pRSNoneCullWireFrame = nullptr;
ID3D11RasterizerState* TDxState::g_pRSSlopeScaledDepthBias=nullptr;
ID3D11DepthStencilState* TDxState::g_pDSSDepthEnable=nullptr;
ID3D11DepthStencilState* TDxState::g_pDSSDepthDisable = nullptr;
ID3D11DepthStencilState*  TDxState::g_pDSSDepthEnableWriteDisable = nullptr;
ID3D11DepthStencilState* TDxState::g_pDSSDepthDisableWriteDisable = nullptr;
bool TDxState::SetState(ID3D11Device* pd3dDevice)
{
	HRESULT hr;
	// (소스컬러*D3D11_BLEND_SRC_ALPHA) 
	//                  + 
	// (대상컬러*D3D11_BLEND_INV_SRC_ALPHA)
	// 컬러   =  투명컬러값 = (1,1,1,1)
	// 마스크 =  1.0 - 투명컬러값 = (1,1,1,1)

	// FinalColor = SrcColor*SrcAlpha + DestColor*(1.0f- SrcAlpha) 	    
	// if SrcAlpha == 0 완전투명
	//           FinalColor() = SrcColor*0 + DestColor*(1-0)
	//                FinalColor = DestColor;
	// if SrcAlpha == 1 완전불투명
	//           FinalColor() = SrcColor*1 + DestColor*(1-1)
	//                FinalColor = SrcColor;
	// 혼합상태 = 소스(지금드로우객체 픽셀) (연산) 대상(백버퍼 객체:픽셀)
	// 혼합상태 = 픽셀쉐이더 출력 컬러  (연산:사칙연산) 출력버퍼의 컬러
	D3D11_BLEND_DESC  blenddesc;
	ZeroMemory(&blenddesc, sizeof(D3D11_BLEND_DESC));
	/*bd.AlphaToCoverageEnable;
	bd.IndependentBlendEnable;*/
	blenddesc.RenderTarget[0].BlendEnable = TRUE;
	blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//// A 연산 저장
	blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blenddesc.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlend);

	blenddesc.RenderTarget[0].BlendEnable = FALSE;
	hr = pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlendDisable);

	blenddesc.RenderTarget[0].RenderTargetWriteMask = 0;
	hr = pd3dDevice->CreateBlendState(&blenddesc, &m_BSNoneColor);

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MinLOD = FLT_MAX;
	sd.MaxLOD = FLT_MIN;
	hr = pd3dDevice->CreateSamplerState(&sd, &m_pSSLinear);
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = pd3dDevice->CreateSamplerState(&sd, &m_pSSPoint);

	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSMirrorLinear)))
	{
		return hr;
	}


	sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSMirrorPoint)))
	{
		return hr;
	}


	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSClampLinear)))
	{
		return hr;
	}

	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSClampPoint)))
	{
		return hr;
	}

	D3D11_SAMPLER_DESC SamDescShad =
	{
		D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,// D3D11_FILTER Filter;
		D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressU;
		D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressV;
		D3D11_TEXTURE_ADDRESS_BORDER, //D3D11_TEXTURE_ADDRESS_MODE AddressW;
		0,//FLOAT MipLODBias;
		0,//UINT MaxAnisotropy;
		D3D11_COMPARISON_LESS , //D3D11_COMPARISON_FUNC ComparisonFunc;
		0.0,0.0,0.0,0.0,//FLOAT BorderColor[ 4 ];
		0,//FLOAT MinLOD;
		0//FLOAT MaxLOD;   
	};
	if (FAILED(hr = pd3dDevice->CreateSamplerState(&SamDescShad, &g_pSSShadowMap)))
	{
		return hr;
	}


	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSBackCullSolid))) return hr;

	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, 
		&TDxState::g_pRSNoneCullSolid))) return hr;


	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(
		&rsDesc, &TDxState::g_pRSBackCullWireFrame))) return hr;

	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_NONE;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(
		&rsDesc, &TDxState::g_pRSNoneCullWireFrame))) return hr;

	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthBias = 100000;
	rsDesc.DepthBiasClamp = 0.0f;
	rsDesc.SlopeScaledDepthBias = 1.0f;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSSlopeScaledDepthBias)))
		return hr;

	D3D11_DEPTH_STENCIL_DESC dsDescDepth;
	ZeroMemory(&dsDescDepth, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDescDepth.DepthEnable = TRUE;
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDescDepth.StencilEnable = FALSE;
	dsDescDepth.StencilReadMask = 1;
	dsDescDepth.StencilWriteMask = 1;
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// 디폴트 값
	dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthEnable)))
	{
		return hr;
	}

	dsDescDepth.DepthEnable = FALSE;
	if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, 
		&g_pDSSDepthDisable)))
	{
		return hr;
	}
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth,
		&g_pDSSDepthDisableWriteDisable)))
	{
		return hr;
	}
	dsDescDepth.DepthEnable = TRUE;
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth,
		&g_pDSSDepthEnableWriteDisable)))
	{
		return hr;
	}

	return true;
}
bool TDxState::Release()
{
	if (g_pRSBackCullSolid) g_pRSBackCullSolid->Release();
	if (g_pRSNoneCullSolid) g_pRSNoneCullSolid->Release();
	if (g_pRSBackCullWireFrame) g_pRSBackCullWireFrame->Release();
	if (g_pRSNoneCullWireFrame) g_pRSNoneCullWireFrame->Release();
	if (g_pRSSlopeScaledDepthBias) g_pRSSlopeScaledDepthBias->Release(); g_pRSSlopeScaledDepthBias = NULL;
	
	if (g_pDSSDepthEnable) g_pDSSDepthEnable->Release();
	if (g_pDSSDepthDisable) g_pDSSDepthDisable->Release();
	if (g_pDSSDepthEnableWriteDisable) g_pDSSDepthEnableWriteDisable->Release();
	if (g_pDSSDepthDisableWriteDisable) g_pDSSDepthDisableWriteDisable->Release();
	if (m_AlphaBlend) m_AlphaBlend->Release();
	if (m_AlphaBlendDisable) m_AlphaBlendDisable->Release();
	if (m_BSNoneColor) m_AlphaBlendDisable->Release();
	m_AlphaBlend = nullptr;
	m_AlphaBlendDisable = nullptr;
	m_BSNoneColor = nullptr;
	if (m_pSSLinear)m_pSSLinear->Release(); m_pSSLinear = NULL;
	if (m_pSSPoint)m_pSSPoint->Release(); m_pSSPoint = NULL;
	if (g_pSSMirrorLinear)	g_pSSMirrorLinear->Release(); g_pSSMirrorLinear = NULL;
	if (g_pSSMirrorPoint)	g_pSSMirrorPoint->Release(); g_pSSMirrorPoint = NULL;
	if (g_pSSClampLinear)	g_pSSClampLinear->Release(); g_pSSClampLinear = NULL;
	if (g_pSSClampPoint)	g_pSSClampPoint->Release(); g_pSSClampPoint = NULL;
	if (g_pSSShadowMap)		g_pSSShadowMap->Release(); g_pSSShadowMap = NULL;
	
	return true;
}
