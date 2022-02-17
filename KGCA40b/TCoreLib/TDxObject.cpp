#include "TDxObject.h"
#include "TObjectMgr.h"
void TBaseObject::HitOverlap(TBaseObject* pObj, DWORD dwState)
{
}
void TBaseObject::HitSelect(TBaseObject* pObj, DWORD dwState)
{

}
void    TDxObject::SetDevice(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext   = pContext;
}
bool    TDxObject::LoadTexture(const TCHAR* szColorFileName,
	const TCHAR* szMaskFileName)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		szColorFileName,
		(ID3D11Resource**)&m_pTexture0,
		&m_pSRV0);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			m_pd3dDevice,
			szColorFileName,
			(ID3D11Resource**)&m_pTexture0,
			&m_pSRV0);
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_pTexture0->GetDesc(&m_TextureDesc);

	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		szMaskFileName,
		(ID3D11Resource**)&m_pTexture1,
		&m_pSRV1);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			m_pd3dDevice,
			szMaskFileName,
			(ID3D11Resource**)&m_pTexture0,
			&m_pSRV0);
		if (FAILED(hr))
		{
			return false;
		}
	}
	//m_pTexture0->GetDesc(&m_TextureDesc);
	return true;
}
bool    TDxObject::SetVertexData()
{	
	return true;
}
bool	TDxObject::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext,	
	const TCHAR* szColorFileName,
	const TCHAR* szMaskFileName)
{
	HRESULT hr;
	m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);
	I_ObjectMgr.AddCollisionExecute(this, 
		std::bind(&TBaseObject::HitOverlap,this, 
											std::placeholders::_1, 
											std::placeholders::_2 ));
	I_ObjectMgr.AddSelectExecute(this,
		std::bind(&TBaseObject::HitSelect, this,
			std::placeholders::_1,
			std::placeholders::_2));

	SetDevice(pd3dDevice, pContext);	
	if (!LoadTexture(szColorFileName, szMaskFileName))
	{
		return false;
	}
	if (!SetVertexData())
	{
		return false;
	}
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);

	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer)))
	{
		return false;
	}
	// 새항목->유틸리티->txt파일 작성
	// 쉐이더 컴파일->오브젝트 파일을 통해서 쉐이더객체 생성 
	
	hr = D3DCompileFromFile(
		L"VertexShader.txt",
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		0,
		0,
		&m_pVSCodeResult,
		&m_pErrorMsgs
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if(m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(
		m_pVSCodeResult->GetBufferPointer(),
		m_pVSCodeResult->GetBufferSize(),
		NULL,
		&m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}

	
	hr = D3DCompileFromFile(
		L"PixelShader.txt",
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		0,
		0,
		&m_pPSCodeResult,
		&m_pErrorMsgs
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(
		m_pPSCodeResult->GetBufferPointer(),
		m_pPSCodeResult->GetBufferSize(),
		NULL,
		&m_pPixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	// 정점쉐이더의 결과를 통해서 정점레이아웃을 생성한다.	
	// 정점버퍼의 각 정점의 어떤 성분을 정점쉐이더에 전달할 거냐
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32_FLOAT, 0,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,8,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		NumElements,
		m_pVSCodeResult->GetBufferPointer(),
		m_pVSCodeResult->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}

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
	hr =m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlend);

	blenddesc.RenderTarget[0].BlendEnable = FALSE;
	hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlendDisable);
	return true;
}
bool	TDxObject::Init()
{
	
	return true;
}
bool	TDxObject::Frame()
{
	return true;
}
bool	TDxObject::Render()
{
	m_pContext->PSSetShaderResources(0, 1, &m_pSRV0);
	m_pContext->PSSetShaderResources(1, 1, &m_pSRV1);
	if (m_bAlphaBlend)
	{
		m_pContext->OMSetBlendState(m_AlphaBlend, 0, -1);
	}
	else
	{
		m_pContext->OMSetBlendState(m_AlphaBlendDisable, 0, -1);
	}

	m_pContext->IASetInputLayout(m_pVertexLayout);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);

	UINT StartSlot;
	UINT NumBuffers;
	UINT Strides = sizeof(SimpleVertex);
	UINT Offsets = 0;

	m_pContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer,
		&Strides, &Offsets);
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		//D3D_PRIMITIVE_TOPOLOGY_POINTLIST
		//D3D_PRIMITIVE_TOPOLOGY_LINELIST
	);
	m_pContext->Draw(m_VertexList.size(), 0);
	return true;
}
bool	TDxObject::Release()
{
	if (m_AlphaBlend) m_AlphaBlend->Release();
	if (m_AlphaBlendDisable) m_pSRV1->Release();
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSRV0) m_pSRV0->Release();
	if (m_pTexture1) m_pTexture1->Release();
	if (m_pSRV1) m_pSRV1->Release();
	m_AlphaBlend = nullptr;
	m_AlphaBlendDisable = nullptr;
	m_pTexture0 = nullptr;
	m_pSRV0 = nullptr;
	m_pTexture1 = nullptr;
	m_pSRV1 = nullptr;

	if (m_pVSCodeResult) m_pVSCodeResult->Release();
	if (m_pPSCodeResult) m_pPSCodeResult->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVSCodeResult = nullptr;
	m_pPSCodeResult = nullptr;
	return true;
}
TDxObject::TDxObject()
{
	m_fSpeed = 0.0001f;
}
TDxObject::~TDxObject()
{}
