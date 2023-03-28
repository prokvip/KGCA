#include "TSprite.h"

HRESULT TSprite::SetInputLayout()
{
	HRESULT hr = S_OK;
	if (m_bInstancing ==false)
	{
		TPlaneShape::SetInputLayout();
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

			{ "ANIMATION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ANIMATION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ANIMATION", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ANIMATION", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

			{ "MESHCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 128, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		};
		UINT numElements = sizeof(layout) / sizeof(layout[0]);
		m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(g_pd3dDevice,
			(DWORD)m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
			m_dxobj.g_pVSBlob.Get()->GetBufferPointer(),
			layout, numElements));
	}
	return hr;
}
HRESULT TSprite::CreateInstance(UINT iNumInstance)
{
	HRESULT hr=S_OK;
	m_pInstance.resize(iNumInstance);

	for (int iSt = 0; iSt < m_pInstance.size(); iSt++)
	{
		D3DXMatrixIdentity(	&m_pInstance[iSt].matWorld);
		m_pInstance[iSt].uv[0] = TVector4(0, 0, 0, 0);
		m_pInstance[iSt].uv[1] = TVector4(0.25f, 0, 0, 0);
		m_pInstance[iSt].uv[2] = TVector4(0.25f, 0.25f, 0, 0);
		m_pInstance[iSt].uv[3] = TVector4(0, 0.25f, 0, 0);

		m_pInstance[iSt].color =
			TVector4((rand() & RAND_MAX) / (float)RAND_MAX,
				(rand() & RAND_MAX) / (float)RAND_MAX,
				(rand() & RAND_MAX) / (float)RAND_MAX,	1);
		D3DXMatrixTranspose(&m_pInstance[iSt].matWorld,	&m_pInstance[iSt].matWorld);
	}
	m_pVBInstance.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_pInstance.at(0), (UINT)m_pInstance.size(), sizeof(TInstatnce)));
	
	
	return hr;
}

HRESULT TSprite::Load(ID3D11Device* pd3dDevice,	
	TCHAR* pLoadTextureString,
	TCHAR* pLoadShaderString,
	bool   bInstancing,
	ID3D11BlendState* pBlendState)
{
	m_bInstancing = bInstancing;
	if (!Create(pd3dDevice, pLoadTextureString, pLoadShaderString))
	{
		MessageBox(0, _T("Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pBlendState = pBlendState;
	return S_OK;
}
bool TSprite::Frame(ID3D11DeviceContext*    pContext, float fGlobalTime, float fElapsedTime)
{
	Updata(pContext, m_fTime, m_iApplyIndex, fGlobalTime, fElapsedTime);
	return true;
}
bool TSprite::PreDraw(ID3D11DeviceContext* pContext)
{
	if (m_pBlendState != nullptr) DX::ApplyBS(pContext, m_pBlendState);

	PreRender(pContext);
	if (m_pAnimSRV != nullptr)
	{
		pContext->PSSetShaderResources(0, 1, &m_pAnimSRV);
	}	
	return true;
}
bool TSprite::Render(ID3D11DeviceContext* pContext)
{
	PreDraw(pContext);
	PostDraw(pContext);
	return true;
}
bool TSprite::PostDraw(ID3D11DeviceContext* pContext)
{
	PostRender(pContext);
	return true;
}
bool	TSprite::RenderInstancing(ID3D11DeviceContext* pContext)
{
	PreDraw(g_pImmediateContext);
	{
		ID3D11Buffer* vb[2] = {	m_dxobj.g_pVertexBuffer.Get(),	m_pVBInstance.Get() };
		UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(TInstatnce) };
		UINT offset[2] = { 0, 0 };
		pContext->IASetVertexBuffers(0, 2, vb, stride, offset);
		pContext->UpdateSubresource(
			m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	}
	pContext->DrawIndexedInstanced(m_dxobj.m_iNumIndex, (UINT)m_pInstance.size(), 0, 0, 0);
	return true;
}
void  TSprite::SetUVAnimation(
	float fAnimTime,
	int iWidth,
	int iHeight)
{
	m_fAnimTime = fAnimTime;
	m_iNumTexture = iWidth * iHeight;
	m_fSecPerRender = m_fAnimTime / m_iNumTexture;

	float fStepW, fStepH;
	fStepW = 1.0f / iWidth;
	fStepH = 1.0f / iHeight;

	m_RectList.resize(iWidth*iHeight);
	for (int iRow = 0; iRow < iHeight; iRow++)
	{
		for (int iCol = 0; iCol <iWidth; iCol++)
		{
			m_RectList[iRow*iWidth + iCol].vUV.x = iCol*fStepW;
			m_RectList[iRow*iWidth + iCol].vUV.y = iRow* fStepH;
			m_RectList[iRow*iWidth + iCol].vUV.z = iCol*fStepW + fStepW;
			m_RectList[iRow*iWidth + iCol].vUV.w = iRow* fStepH + fStepH;
		}
	}
}
void TSprite::SetRectAnimation(float fAnimTime, int iWidth, int iWidthSize, int iHeight, int iHeightSize)
{
	m_fAnimTime = fAnimTime;

	m_RectSet.left = iWidth;
	m_RectSet.right = iWidthSize; // 텍스쳐 가로 셋 갯수 및 크기	
	m_RectSet.top = iHeight;
	m_RectSet.bottom = iHeightSize; // 텍스쳐 세로 셋 갯수 및 크기

	if (iWidth*iHeight <= 1)
	{
		m_fSecPerRender = fAnimTime / m_iNumTexture;
		return;
	}
	else
	{
		m_fSecPerRender = fAnimTime / (iWidth*iHeight);
		m_iNumTexture = iWidth *  iHeight;
	}

	for (int iHeight = 0; iHeight<m_RectSet.left; iHeight++)
	{
		for (int iWidth = 0; iWidth<m_RectSet.top; iWidth++)
		{
			TRectUV  tRect;
			tRect.Rect.left = iWidth		* m_RectSet.right;
			tRect.Rect.right = (iWidth + 1) * m_RectSet.right;
			tRect.Rect.top = iHeight	* m_RectSet.bottom;
			tRect.Rect.bottom = (iHeight + 1)* m_RectSet.bottom;

			tRect.vUV = SetUV(tRect.Rect);
			m_RectList.push_back(tRect);
		}
	}
}
TVector4 TSprite::SetUV(RECT& Rect)
{
	TVector4 vUV;
	float fOffSetX = 0.0f;
	if (Rect.left > 0)
	{
		fOffSetX = (float)Rect.left / (float)(m_RectSet.left*m_RectSet.right);
	}
	float fOffSetY = 0.0f;
	if (Rect.top > 0)
	{
		fOffSetY = (float)Rect.top / (float)(m_RectSet.top*m_RectSet.bottom);
	}

	vUV.x = fOffSetX;
	vUV.y = fOffSetY;

	fOffSetX = 1.0f;
	if (Rect.right > 0)
	{
		fOffSetX = (float)Rect.right / (float)(m_RectSet.left*m_RectSet.right);
	}
	fOffSetY = 1.0f;
	if (Rect.bottom > 0)
	{
		fOffSetY = (float)Rect.bottom / (float)(m_RectSet.top*m_RectSet.bottom);
	}
	vUV.z = fOffSetX;
	vUV.w = fOffSetY;
	return vUV;
}

void TSprite::SetTextureArray(T_STR_VECTOR FileList)
{
	HRESULT hr = S_OK;
	for (int iList = 0; iList < FileList.size(); iList++)
	{
		INT iIndex = I_Texture.Add(m_pd3dDevice, FileList[iList].c_str());
		if (iIndex <= 0)
		{
			MessageBox(0, _T("m_Texture.Load 실패"), _T("Fatal error"), MB_OK);
			return;
		}
		m_TextureIndex.push_back(iIndex);
	}
	m_iNumTexture = (int)m_TextureIndex.size();
}
// 스프라이트 타입 단위로 변경
void TSprite::Updata(ID3D11DeviceContext*    pContext,
	float& pfCurrentTimer,
	int& iApplyIndex,
	float fGlobalTime,
	float fElapsedTime)
{
	// 스프라이트 발생 경과 시간
	m_fElapseTime += fElapsedTime;
	// 에니메이션 교체 주기 누적 시간
	pfCurrentTimer += fElapsedTime;

	if (pfCurrentTimer >= m_fSecPerRender)
	{
		if (++iApplyIndex >= m_iNumTexture)
			iApplyIndex = 0;

		pfCurrentTimer = 0.0f;
	}
	// 텍스처 에니메이션
	if (m_TextureIndex.size())
	{
		m_pAnimSRV = I_Texture.GetPtr(m_TextureIndex[iApplyIndex])->m_pTextureSRV.Get();
	}
	else // UV 텍스처 에니메이션
	{
		if (m_RectList.size() <= 0) return;

		if (m_bInstancing)
		{
			for (int iSt = 0; iSt < m_pInstance.size(); iSt++)
			{
				TVector4 vUV    = m_RectList[m_pInstance[iSt].uv[0].z].vUV;
				TVector4 vColor = m_pInstance[iSt].color;
				m_pInstance[iSt].uv[0] = TVector4(vUV.x, vUV.y, 0, 0);
				m_pInstance[iSt].uv[1] = TVector4(vUV.z, vUV.y, 0, 0);
				m_pInstance[iSt].uv[2] = TVector4(vUV.z, vUV.w, 0, 0);
				m_pInstance[iSt].uv[3] = TVector4(vUV.x, vUV.w, 0, 0);
				D3DXMatrixTranspose(&m_pInstance[iSt].matWorld, &m_pInstance[iSt].matWorld);
			}

		/*	D3D11_BOX box;
			box.left = 0;
			box.right = 0 + sizeof(TInstatnce)*m_pInstance.size();
			box.top = 0; box.bottom = 1;
			box.front = 0; box.back = 1;*/

			pContext->UpdateSubresource( m_pVBInstance.Get(), 0, NULL, &m_pInstance.at(0),0, 0);
		}
		else
		{
			TVector4 vUV = m_RectList[iApplyIndex].vUV;
			TVector4 vColor = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

			if (m_dxobj.g_pTextureSRV != nullptr && m_RectList.size())
			{
				PNCT_VERTEX vertices[] =
				{
					{ TVector3(-1.0f, 1.0f, 0.0f),TVector3(-1.0f, 1.0f, 0.0f),		vColor,TVector2(vUV.x, vUV.y) },
					{ TVector3(1.0f, 1.0f, 0.0f),TVector3(-1.0f, 1.0f, 0.0f),		vColor,TVector2(vUV.z, vUV.y) },
					{ TVector3(1.0f, -1.0f, 0.0f),TVector3(-1.0f, 1.0f, 0.0f),		vColor,TVector2(vUV.z, vUV.w) },
					{ TVector3(-1.0f, -1.0f, 0.0f),TVector3(-1.0f, 1.0f, 0.0f),	vColor,TVector2(vUV.x, vUV.w) },
				};
				pContext->UpdateSubresource(GetVB(), 0, NULL, &vertices, 0, 0);
			}
		}
	}
}

TSprite::TSprite(void)
{
	m_fAnimTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_fElapseTime = 0.0f;
	m_bInstancing = false;
	m_BlendType = 0;
	m_fTime = 0.0f;
	m_iApplyIndex = 0;
	m_fSecPerRender = 0.2f;
	m_iNumTexture = 0;
	m_RectSet.left = 4;
	m_RectSet.right = 64; // 텍스쳐 가로 셋 갯수 및 크기	
	m_RectSet.top = 4;
	m_RectSet.bottom = 64; // 텍스쳐 세로 셋 갯수 및 크기
	m_pAnimSRV = nullptr;
	m_pBlendState = nullptr;
	m_ParticleColor = TVector4(1, 1, 1, 1);
}

TSprite::~TSprite(void)
{
}
