#include "TDxState.h"
ID3D11BlendState* TDxState::m_AlphaBlend = nullptr;
ID3D11BlendState* TDxState::m_AlphaBlendDisable = nullptr;
ID3D11SamplerState* TDxState::m_pSamplerState = nullptr;

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


	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MinLOD = FLT_MAX;
	sd.MaxLOD = FLT_MIN;
	hr = pd3dDevice->CreateSamplerState(&sd, &m_pSamplerState);

	return true;
}
bool TDxState::Release()
{
	if (m_AlphaBlend) m_AlphaBlend->Release();
	if (m_AlphaBlendDisable) m_AlphaBlendDisable->Release();
	m_AlphaBlend = nullptr;
	m_AlphaBlendDisable = nullptr;

	if (m_pSamplerState)m_pSamplerState->Release();
	return true;
}