#include "TActor.h"

bool TActor::Convert(ID3D11Device* pDevice) {
	return true;
};
bool TActor::Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread)
{
	return Convert(pDevice);
};
bool TActor::Create(ID3D11Device* pDevice,
	const TCHAR* pLoadShaderFile,
	const TCHAR* pLoadTextureString)
{
	m_pd3dDevice = pDevice;

	if (FAILED(LoadShaderFile(pDevice, pLoadShaderFile)))
	{
		MessageBox(0, _T("LoadShaderFile 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(SetInputLayout()))
	{
		MessageBox(0, _T("SetInputLayout 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (!CreateVertexData())
	{
		MessageBox(0, _T("CreateVertexData 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (!CreateIndexData())
	{
		MessageBox(0, _T("CreateIndexData 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(0, _T("CreateVertexBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateIndexBuffer()))
	{
		MessageBox(0, _T("CreateIndexBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateConstantBuffer()))
	{
		MessageBox(0, _T("CreateConstantBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(LoadTextures(pDevice, pLoadTextureString)))
	{
		MessageBox(0, _T("LoadTextures 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (!UpdateBuffer())
	{
		MessageBox(0, _T("UpdateBuffer 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	if (FAILED(CreateResource()))
	{
		MessageBox(0, _T("CreateResource 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	// 기본 충돌데이터를 저장한다.
	SetCollisionData(m_matWorld);
	return Init();
}
HRESULT TActor::LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	m_dxobj.g_pTextureSRV.Attach(DX::CreateShaderResourceView(pDevice, m_pContext, pLoadTextureString));
	return hr;
}
HRESULT TActor::LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pShaderFile)
{
	m_dxobj.g_pVertexShader.Attach(DX::LoadVertexShaderFile(pDevice, pShaderFile, m_dxobj.g_pVSBlob.GetAddressOf()));
	m_dxobj.g_pPixelShader.Attach(DX::LoadPixelShaderFile(pDevice, pShaderFile));
	m_dxobj.g_pGeometryShader.Attach(DX::LoadGeometryShaderFile(pDevice, pShaderFile, m_dxobj.g_pGSBlob.GetAddressOf()));
	m_dxobj.g_pHullShader.Attach(DX::LoadHullShaderFile(pDevice, pShaderFile, m_dxobj.g_pHSBlob.GetAddressOf()));
	m_dxobj.g_pDomainShader.Attach(DX::LoadDomainShaderFile(pDevice, pShaderFile, m_dxobj.g_pDSBlob.GetAddressOf()));
	m_dxobj.g_pComputeShader.Attach(DX::LoadComputeShaderFile(pDevice, pShaderFile, m_dxobj.g_pCSBlob.GetAddressOf()));
	return S_OK;
}
void	TActor::ResetShader()
{
	m_dxobj.g_pVertexShader.Reset();
	m_dxobj.g_pPixelShader.Reset();
	m_dxobj.g_pGeometryShader.Reset();
	m_dxobj.g_pHullShader.Reset();
	m_dxobj.g_pDomainShader.Reset();
	m_dxobj.g_pComputeShader.Reset();
}
bool	TActor::CreateVertexData()
{
	return true;
}
bool	TActor::CreateIndexData()
{
	return true;
}
HRESULT TActor::CreateVertexBuffer()
{
	if (m_dxobj.m_iNumVertex <= 0) return S_OK;
	void** pData = nullptr;
	if (m_VertexList.size() > 0) pData = (void**)&m_VertexList.at(0);

	m_dxobj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice,
		pData,
		m_dxobj.m_iNumVertex,
		m_dxobj.m_iVertexSize));
	return S_OK;
}
HRESULT	TActor::CreateIndexBuffer()
{
	if (m_dxobj.m_iNumIndex <= 0) return S_OK;
	void** pData = nullptr;
	if (m_IndexList.size() > 0) pData = (void**)&m_IndexList.at(0);

	m_dxobj.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice,
		pData,
		m_dxobj.m_iNumIndex,
		m_dxobj.m_iIndexSize));
	return S_OK;
}
HRESULT TActor::CreateConstantBuffer()
{
	m_cbData.Color = TVector4(1, 1, 1, 1);
	m_dxobj.g_pConstantBuffer.Attach(DX::CreateConstantBuffer(m_pd3dDevice, &m_cbData, 1, sizeof(VS_CONSTANT_BUFFER)));
	return S_OK;
}
bool TActor::Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor) { return true; }
HRESULT TActor::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, (DWORD)m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool TActor::UpdateBuffer() { return true; }
bool TActor::Init() { 
	SetCollisionData(m_matWorld);
	return true; 
}
bool TActor::Frame() { return true; }
bool TActor::Release() { return true; }
bool TActor::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_dxobj.m_iPrimitiveType);
	m_dxobj.PreRender(pContext, m_dxobj.m_iVertexSize);
	return true;
}
void TActor::UpdateConstantBuffer(ID3D11DeviceContext* pContext, TActor* pParent)
{
	if( pParent != NULL && pParent->m_dxobj.g_pConstantBuffer != nullptr)
	{
		pContext->UpdateSubresource(pParent->m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &pParent->m_cbData, 0, 0);
	}
	else
	{
		if( m_dxobj.g_pConstantBuffer != nullptr)
			pContext->UpdateSubresource(m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	}
}
bool TActor::PostRender(ID3D11DeviceContext* pContext)
{
	UpdateConstantBuffer(pContext);
	m_dxobj.PostRender(pContext, m_dxobj.m_iNumIndex);
	return true;
}
bool TActor::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}

void TActor::SetAmbientColor(float fR, float fG, float fB, float fA)
{
	m_cbData.Color = TVector4(fR, fG, fB, fA);
}
void TActor::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}

	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);

	//m_cbData.matWorld=m_matWorld;
	//m_cbData.matView=m_matView;
	//m_cbData.matProj=m_matProj;

	if (m_bUpdateCollision)
	{
		SetCollisionData(m_matWorld);
	}
}
void TActor::SetCollisionData( TMatrix& matWorld)
{
	m_matWorld = matWorld;
	m_vCenter.x = m_matWorld._41;
	m_vCenter.y = m_matWorld._42;
	m_vCenter.z = m_matWorld._43;
	
	m_vMove = m_vCenter - m_vPrevCenter;
	m_vPrevCenter = m_vCenter;

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;

	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	m_vSide.x = m_matWorld._11;
	m_vSide.y = m_matWorld._12;
	m_vSide.z = m_matWorld._13;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vSide, &m_vSide);

	m_Box.vCenter = m_vCenter;
	// w= 1
	TVector3 vMax = m_InitBox.vMax;
	TVector3 vMin = m_InitBox.vMin;
	D3DXVec3TransformCoord(&m_Box.vMax, &vMax, &m_matWorld);
	D3DXVec3TransformCoord(&m_Box.vMin, &vMin, &m_matWorld);
	// w= 0 * _41
	D3DXVec3TransformNormal(&m_Box.vAxis[0], &m_InitBox.vAxis[0], &m_matWorld);
	D3DXVec3TransformNormal(&m_Box.vAxis[1], &m_InitBox.vAxis[1], &m_matWorld);
	D3DXVec3TransformNormal(&m_Box.vAxis[2], &m_InitBox.vAxis[2], &m_matWorld);

	D3DXVec3Normalize(&m_Box.vAxis[0], &m_Box.vAxis[0]);
	D3DXVec3Normalize(&m_Box.vAxis[1], &m_Box.vAxis[1]);
	D3DXVec3Normalize(&m_Box.vAxis[2], &m_Box.vAxis[2]);

	m_Box.fExtent[0] = (m_Box.vMax.x - m_Box.vMin.x) * 0.5f;
	m_Box.fExtent[1] = (m_Box.vMax.y - m_Box.vMin.y) * 0.5f;
	m_Box.fExtent[2] = (m_Box.vMax.z - m_Box.vMin.z) * 0.5f;

	m_Sphere.vCenter = m_vCenter;
	m_Sphere.fRadius = D3DXVec3Length(&(m_Box.vMax - m_Box.vCenter));
}
ID3D11Buffer* TActor::GetVB()
{
	return m_dxobj.g_pVertexBuffer.Get();
}
ID3D11Buffer* TActor::GetIB()
{
	return m_dxobj.g_pIndexBuffer.Get();
}
HRESULT TActor::CreateResource()
{
	m_dxobj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
HRESULT TActor::DeleteResource()
{
	return S_OK;
}
TActor::TActor(void)
{
	m_pd3dDevice = NULL;
	m_pContext = NULL;
	m_InitBox.vMax = TVector3(1.0f, 1.0f, 1.0f);
	m_InitBox.vMin = TVector3(-1.0f, -1.0f, -1.0f);
	m_cbData.Color = TVector4(1, 1, 1, 1);
	m_vPrevCenter = m_vCenter = TVector3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matControlWorld);
	
	m_fElapseTime = 0.0f;
	m_iStartFrame = 0;
	m_iLastFrame = 0;
	m_iMaxVertex = 0;
	m_iMaxIndex = 0;
	m_iBeginPosVB = 0;
	m_iBeginPosIB = 0;
	m_ObjType = NULLFILE;
	m_bOptimization = true;
	m_iMatrixIndex = -1;
	m_bUpdateCollision = false;
	m_fTickSpeed = 1.0f;
}

TActor::~TActor(void)
{
	
}
