#include "TDxObject.h"

void    TDxObject::SetDevice(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext   = pContext;
}
void	TDxObject::Convert(
	TVector2 center, float fWidth, float fHeight,
	std::vector<SimpleVertex>& retList)
{
	// 0       1,4

	//     c

	// 2,3      5
	std::vector<SimpleVertex> list(6);
	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;
	list[0].v = { center.x - halfWidth, center.y - halfHeight };
	list[1].v = { center.x + halfWidth, center.y - halfHeight };
	list[2].v = { center.x - halfWidth, center.y + halfHeight };
	list[3].v = list[2].v;
	list[4].v = list[1].v;
	list[5].v = { center.x + halfWidth, center.y + halfHeight };

	Convert(list, retList);

}
void	TDxObject::Convert(
	std::vector<SimpleVertex>& list,
	std::vector<SimpleVertex>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		retList[i].v.x = list[i].v.x / g_rtClient.right;
		retList[i].v.y = list[i].v.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		retList[i].v.x = retList[i].v.x * 2.0f - 1.0f;
		retList[i].v.y = -1.0f * (retList[i].v.y * 2.0f - 1.0f);
	}
}
bool	TDxObject::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext,TVector2 vPos, float fWidth, float fHeight)
{
	if ((vPos.x - fWidth / 2.0f) < 0) return false;
	if ((vPos.x + fWidth / 2.0f) > g_rtClient.right) return false;
	if ((vPos.y - fHeight / 2.0f) < 0) return false;
	if ((vPos.y + fHeight / 2.0f) > g_rtClient.bottom) return false;
	SetDevice(pd3dDevice, pContext);
	//화면좌표계
	// 0,0   ~      800,0
	// 
	//      400,300
	// 
	// 0,600        800,600
	//직각좌표계(왼손좌표계)    ->  NDC좌표로 변환 -> 화면좌표계
	// -1,1   ~      1,1             z=(0~1)
	// 
	//        0,0
	// 
	// -1,-1          1,-1
	HRESULT hr;
	// ui used
	m_InitScreenList.resize(6);
	m_InitScreenList[0].v = TVector2(0.0f, 0.0f);
	m_InitScreenList[1].v = TVector2(fWidth, 0.0f);
	m_InitScreenList[2].v = TVector2(0.0f, fHeight);
	m_InitScreenList[3].v = TVector2(0.0f, fHeight);
	m_InitScreenList[4].v = TVector2(fWidth, 0.0f);
	m_InitScreenList[5].v = TVector2(fWidth, fHeight);


	//Convert(m_InitScreenList, m_VertexList);

	m_vPos = vPos;
	Convert(m_vPos,
		fWidth, fHeight,
		m_VertexList);

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
	ID3DBlob* pVSCodeResult = nullptr;
	ID3DBlob* pErrorMsgs = nullptr;
	hr = D3DCompileFromFile(
		L"VertexShader.txt",
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		0,
		0,
		&pVSCodeResult,
		&pErrorMsgs
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(
		pVSCodeResult->GetBufferPointer(),
		pVSCodeResult->GetBufferSize(),
		NULL,
		&m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}

	ID3DBlob* pPSCodeResult = nullptr;
	hr = D3DCompileFromFile(
		L"PixelShader.txt",
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		0,
		0,
		&pPSCodeResult,
		&pErrorMsgs
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(
		pPSCodeResult->GetBufferPointer(),
		pPSCodeResult->GetBufferSize(),
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
	};
	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		NumElements,
		pVSCodeResult->GetBufferPointer(),
		pVSCodeResult->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	TDxObject::Init()
{
	
	return true;
}
bool	TDxObject::Frame()
{
	/*m_VertexList[0].v.x += m_fSpeed;
	m_VertexList[1].v.x += m_fSpeed;
	m_VertexList[2].v.x += m_fSpeed;
	m_VertexList[3].v.x += m_fSpeed;
	m_VertexList[4].v.x += m_fSpeed;
	m_VertexList[5].v.x += m_fSpeed;
	m_pContext->UpdateSubresource(
		m_pVertexBuffer,0, NULL, &m_VertexList.at(0), 0,0);*/
	return true;
}
bool	TDxObject::Render()
{
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
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	return true;
}
TDxObject::TDxObject()
{
	m_fSpeed = 0.00001f;
}
TDxObject::~TDxObject()
{}
