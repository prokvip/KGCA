#include "TDxState.h"
//extern ID3D11Device*    g_pd3dDevice;

namespace DX
{
	ID3D11RasterizerState* TDxState::g_pRSWireFrame = 0;
	ID3D11RasterizerState* TDxState::g_pRSSolid = 0;
	ID3D11RasterizerState* TDxState::g_pRSBackCullSolid = 0;
	ID3D11RasterizerState* TDxState::g_pRSNoneCullSolid = 0;		
	ID3D11RasterizerState* TDxState::g_pRSFrontCullSolid = 0;
	ID3D11RasterizerState* TDxState::g_pRSSlopeScaledDepthBias;
	ID3D11RasterizerState* TDxState::g_pRSScissorBackCullSolid;
	ID3D11RasterizerState* TDxState::g_pRS[6] = { 0, };
	
	ID3D11SamplerState*	 TDxState::g_pTexSS = 0;
	ID3D11SamplerState*  TDxState::g_pSSWrapLinear = NULL;
	ID3D11SamplerState*  TDxState::g_pSSWrapPoint = NULL;
	ID3D11SamplerState*  TDxState::g_pSSMirrorLinear = NULL;
	ID3D11SamplerState*  TDxState::g_pSSMirrorPoint = NULL;
	ID3D11SamplerState*  TDxState::g_pSSClampLinear = NULL;
	ID3D11SamplerState*  TDxState::g_pSSClampPoint = NULL;		
	ID3D11SamplerState*  TDxState::g_pSSShadowMap = NULL;
	ID3D11SamplerState*  TDxState::g_pSS[8] = { 0, };

	ID3D11DepthStencilState*	TDxState::g_pDSSDepthEnable = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSSDepthDisable = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSSDepthEnableNoWrite = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSSDepthDisableNoWrite = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSSDepthStencilAdd =0;
	ID3D11DepthStencilState*	TDxState::g_pDSSDepthAlways = 0;
	ID3D11DepthStencilState*	TDxState::g_pDSS[6] = { 0, };

	ID3D11BlendState*	  TDxState::g_pAlphaBlend = 0;
	ID3D11BlendState*	  TDxState::g_pNoAlphaBlend = 0;
	ID3D11BlendState*	  TDxState::g_pBSColorOne = 0;
	ID3D11BlendState*	  TDxState::g_pBSOneOne = 0;
	ID3D11BlendState*	  TDxState::g_pBSOneZero = 0;
	ID3D11BlendState*	  TDxState::g_pBSAlphaOne = 0;
	ID3D11BlendState*	  TDxState::g_pBSMaxOneOne = 0;
	ID3D11BlendState*	  TDxState::g_pBS[7] = { 0, };
	
	Microsoft::WRL::ComPtr<ID3D11BlendState>		TDxState::g_pBSEdit = 0;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	TDxState::g_pRSEdit = 0;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>      TDxState::g_pSSEdit = 0;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> TDxState::g_pDSSEdit = 0;
	D3D11_BLEND_DESC			TDxState::g_BlendDesc = { 0, };
	D3D11_RASTERIZER_DESC		TDxState::g_RasterizerDesc;
	D3D11_SAMPLER_DESC			TDxState::g_SamplerDesc;
	D3D11_DEPTH_STENCIL_DESC	TDxState::g_DepthStencilDesc = { 0, };

	FLOAT TDxState::g_fBlendFactor[4] = { 0, };
	UINT  TDxState::g_iMask = 0xff;
	UINT TDxState::g_iSlot = 0;
	UINT TDxState::g_iArray = 1;
	UINT TDxState::g_iRef = 1;

	HRESULT TDxState::SetBlendState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_BLEND_DESC& ds,
		const FLOAT fBlendFactor[],
		UINT iMask)
	{
		HRESULT hr = S_OK;
		g_pBSEdit.Reset();
		g_BlendDesc = ds;
		memcpy(&g_fBlendFactor, fBlendFactor, sizeof(FLOAT) * 4);
		g_iMask = iMask;
		if (FAILED(hr = pd3dDevice->CreateBlendState(&ds, g_pBSEdit.GetAddressOf())))
		{
			return hr;
		}
		if (pContext != nullptr)
		{
			ApplyBS(pContext, g_pBSEdit.Get(), fBlendFactor, iMask);
		}
		return hr;
	}
	HRESULT TDxState::SetRasterizerState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_RASTERIZER_DESC& rsDesc)
	{
		HRESULT hr = S_OK;
		g_pRSEdit.Reset();
		g_RasterizerDesc = rsDesc;
		if (FAILED(hr =
			pd3dDevice->CreateRasterizerState(&rsDesc, g_pRSEdit.GetAddressOf())))
		{
			return hr;
		}
		if (pContext != nullptr)
		{
			ApplyRS(pContext, g_pRSEdit.Get());
		}
		return hr;
	}
	HRESULT TDxState::SetSamplerState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_SAMPLER_DESC& sd,
		UINT iSlot,
		UINT iArray)
	{
		HRESULT hr = S_OK;
		g_pSSEdit.Reset();
		g_SamplerDesc = sd;
		g_iSlot = iSlot;
		g_iArray = iArray;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(
			&sd,
			g_pSSEdit.GetAddressOf())))
		{
			return hr;
		}
		if (pContext != nullptr)
		{
			ApplySS(pContext, g_pSSEdit.Get(), iSlot, iArray);
		}
		return hr;
	}
	HRESULT TDxState::SetDipthStencilState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_DEPTH_STENCIL_DESC& dsd,
		UINT iRef)
	{
		HRESULT hr;
		g_pDSSEdit.Reset();
		g_DepthStencilDesc = dsd;
		g_iRef = iRef;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsd, g_pDSSEdit.GetAddressOf())))
		{
			return hr;
		}
		if (pContext != nullptr)
		{
			ApplyDSS(pContext, g_pDSSEdit.Get(), iRef);
		}
		return hr;
	}
	HRESULT TDxState::SetState(ID3D11Device*	pd3dDevice)
	{
		if (pd3dDevice == nullptr) return S_OK;
		if (g_pTexSS != nullptr) return S_OK;

		HRESULT hr = S_OK;
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		// 필터링 방식
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		// 텍스처 주소 지정 방식
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MaxLOD = FLT_MAX;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = FLT_MIN;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pTexSS)))
			return hr;

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = TRUE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[1].BlendEnable = FALSE;
		blendDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		pd3dDevice->CreateBlendState(&blendDesc, &g_pAlphaBlend);

		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		pd3dDevice->CreateBlendState(&blendDesc, &g_pNoAlphaBlend);

		// DECAL BLENDING ONE, INV_SRC_ALPHA , ADD
		// ADDIVIVE BLENDING ONE, ONE, ADD
		// SUBRACITIVE BLENDING ONE, ONE REV_SUBTRACT
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &TDxState::g_pBSColorOne);

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &TDxState::g_pBSOneOne);

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;// DEST_COLOR;
		pd3dDevice->CreateBlendState(&blendDesc, &TDxState::g_pBSOneZero);

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		pd3dDevice->CreateBlendState(&blendDesc, &TDxState::g_pBSAlphaOne);

		blendDesc.AlphaToCoverageEnable = TRUE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		pd3dDevice->CreateBlendState(&blendDesc, &TDxState::g_pBSMaxOneOne);

		//레스터라이즈 상태 객체 생성
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(rsDesc));
		rsDesc.DepthClipEnable = TRUE;
		rsDesc.ScissorEnable = TRUE;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;	
		
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSScissorBackCullSolid))) return hr;

		rsDesc.ScissorEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSBackCullSolid))) return hr;


		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSWireFrame)))
			return hr;
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSSolid)))
			return hr;

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSNoneCullSolid)))
			return hr;


		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_FRONT;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSFrontCullSolid)))
			return hr;

		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		rsDesc.DepthBias = 20000;
		rsDesc.DepthBiasClamp = 0.0f;
		rsDesc.SlopeScaledDepthBias = 1.0f;
		if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSSlopeScaledDepthBias)))
			return hr;

		g_pRS[0] = g_pRSNoneCullSolid;
		g_pRS[1] = g_pRSFrontCullSolid;
		g_pRS[2] = g_pRSBackCullSolid;


		ID3D11SamplerState* pSamplerState = nullptr;
		D3D11_SAMPLER_DESC sd;
		memset(&sd, 0, sizeof(sd));
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.MaxLOD = FLT_MAX;
		sd.MinLOD = FLT_MIN;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSWrapLinear)))
		{
			return hr;
		}
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		if (FAILED(hr = pd3dDevice->CreateSamplerState(&sd, &g_pSSWrapPoint)))
		{
			return hr;
		}

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

		/*sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.BorderColor[0] = 0.0f;
		sd.BorderColor[1] = 0.0f;
		sd.BorderColor[2] = 0.0f;
		sd.BorderColor[3] = 0.0f;
		sd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;*/
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

		g_pSS[0] = g_pSSWrapLinear;
		g_pSS[1] = g_pSSWrapPoint;
		g_pSS[2] = g_pSSMirrorLinear;
		g_pSS[3] = g_pSSMirrorPoint;
		g_pSS[4] = g_pSSClampLinear;
		g_pSS[5] = g_pSSClampPoint;
		g_pSS[6] = g_pSSShadowMap;		

		// 깊이버퍼 상태값 세팅
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
		// 깊이 버퍼 비교 비 활성화.
		dsDescDepth.DepthEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthDisable)))
		{
			return hr;
		}
		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_ALWAYS;//DepthEnable = TRUE 이어야 한다.
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthAlways)))
		{
			return hr;
		}

		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDescDepth.StencilEnable = TRUE;
		dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL; // 뒷면 비교 함수
		dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;// 앞면 비교 함수
		dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthStencilAdd)))
		{
			return hr;
		}

		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDescDepth.DepthEnable = TRUE;
		dsDescDepth.StencilEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthEnableNoWrite)))
		{
			return hr;
		}
		// 깊이 버퍼 비교 비 활성화.
		dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDescDepth.DepthEnable = FALSE;
		dsDescDepth.StencilEnable = FALSE;
		if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthDisableNoWrite)))
		{
			return hr;
		}
		return hr;
	}
	bool TDxState::Release()
	{
		if (g_pBSColorOne)		g_pBSColorOne->Release(); g_pBSColorOne = NULL;
		if (g_pTexSS)		g_pTexSS->Release(); g_pTexSS = NULL;
		if (g_pAlphaBlend)	g_pAlphaBlend->Release(); g_pAlphaBlend = NULL;
		if (g_pNoAlphaBlend)g_pNoAlphaBlend->Release(); g_pNoAlphaBlend = NULL;
		if (g_pBSOneOne)g_pBSOneOne->Release(); g_pBSOneOne = NULL;
		if (g_pBSOneZero)g_pBSOneZero->Release(); g_pBSOneZero = NULL;
		if (g_pBSAlphaOne)g_pBSAlphaOne->Release(); g_pBSAlphaOne = NULL;
		if (g_pBSMaxOneOne)g_pBSMaxOneOne->Release(); g_pBSMaxOneOne = NULL;
		
		if (g_pRSBackCullSolid)	g_pRSBackCullSolid->Release(); g_pRSBackCullSolid = NULL;
		if (g_pRSWireFrame)		g_pRSWireFrame->Release(); g_pRSWireFrame = NULL;
		if (g_pRSSolid)			g_pRSSolid->Release(); g_pRSSolid = NULL;
		if (g_pRSNoneCullSolid)	g_pRSNoneCullSolid->Release(); g_pRSNoneCullSolid = NULL;
		if (g_pRSFrontCullSolid)g_pRSFrontCullSolid->Release(); g_pRSFrontCullSolid = NULL;
		if (g_pRSScissorBackCullSolid)g_pRSScissorBackCullSolid->Release(); g_pRSScissorBackCullSolid = NULL;

		if (g_pSSWrapLinear)	g_pSSWrapLinear->Release(); g_pSSWrapLinear = NULL;
		if (g_pSSWrapPoint)		g_pSSWrapPoint->Release(); g_pSSWrapPoint = NULL;
		if (g_pSSMirrorLinear)	g_pSSMirrorLinear->Release(); g_pSSMirrorLinear = NULL;
		if (g_pSSMirrorPoint)	g_pSSMirrorPoint->Release(); g_pSSMirrorPoint = NULL;
		if (g_pSSClampLinear)	g_pSSClampLinear->Release(); g_pSSClampLinear = NULL;
		if (g_pSSClampPoint)	g_pSSClampPoint->Release(); g_pSSClampPoint = NULL;
		if (g_pSSShadowMap)		g_pSSShadowMap->Release(); g_pSSShadowMap = NULL;
		if (g_pRSSlopeScaledDepthBias)		g_pRSSlopeScaledDepthBias->Release(); g_pRSSlopeScaledDepthBias = NULL;
		
		if (g_pDSSDepthEnable) g_pDSSDepthEnable->Release(); g_pDSSDepthEnable = NULL;
		if (g_pDSSDepthDisable) g_pDSSDepthDisable->Release(); g_pDSSDepthDisable = NULL;
		if (g_pDSSDepthEnableNoWrite) g_pDSSDepthEnableNoWrite->Release(); g_pDSSDepthEnableNoWrite = NULL;
		if (g_pDSSDepthDisableNoWrite) g_pDSSDepthDisableNoWrite->Release(); g_pDSSDepthDisableNoWrite = NULL;
		if (g_pDSSDepthStencilAdd) g_pDSSDepthStencilAdd->Release(); g_pDSSDepthStencilAdd = NULL;
		if (g_pDSSDepthAlways) g_pDSSDepthAlways->Release(); g_pDSSDepthAlways = NULL;
		
		
		return true;
	}
	TDxState::~TDxState()
	{
		Release();
	}
	TDxState g_DxState;
}