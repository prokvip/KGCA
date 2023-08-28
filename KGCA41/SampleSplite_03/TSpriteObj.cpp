#include "TSpriteObj.h"
bool  TSpriteObj::Load(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext,
	TSpriteInfo info)
{
	m_InitSpriteInfo = info;
	m_fAnimTimer = info.fAnimTimer;
	m_iNumSpriteX = info.iNumRow;
	m_iNumSpriteY = info.iNumColumn;
	Set(pDevice, pContext);
	SetPos(info.p);
	SetScale(info.s);
	if (!info.texAlphaFile.empty())
	{
		this->m_pAlphaTex = I_Tex.Load(info.texAlphaFile);
	}
	LoadTexArray(info.texList);
	SetUVFrame(info.iNumRow, info.iNumColumn );
	return Create(info.texFile, info.shaderFile);
}
bool   TSpriteTexture::LoadTexArray(T_STR_VECTOR& texList)
{
	for (auto& texname : texList)
	{
		const TTexture* pTex = I_Tex.Load(texname);
		m_pTexList.push_back(pTex);
	}
	m_fOffsetTime = m_fAnimTimer / m_pTexList.size();
	return true;
}
bool TSpriteTexture::Init()
{
	TPlaneObj::Init();
	return true;
}
bool TSpriteTexture::Frame()
{
	TPlaneObj::Frame();	
	m_fElapsedTimer += g_fSecondPerFrame;
	if (m_pTexList[m_iCurrentAnimIndex] != nullptr)
	{
		if (m_fElapsedTimer >= m_fOffsetTime)
		{
			m_iCurrentAnimIndex++;
			if (m_iCurrentAnimIndex >= m_pTexList.size())
			{
				m_iCurrentAnimIndex = 0;
			}
			m_fElapsedTimer -= m_fOffsetTime;
		}		
	}
	return true;
}
bool TSpriteTexture::Render()
{
	PreRender();
	m_pTexList[m_iCurrentAnimIndex]->Apply(m_pImmediateContext, 0);
	PostRender();
	return true;
}
bool TSpriteTexture::Release()
{
	TPlaneObj::Release();
	return true;
}

TSpriteTexture::TSpriteTexture()
{
	Init();
}
TSpriteTexture::~TSpriteTexture()
{
	Release();
}

void TSpriteUV::SetUVFrame(int iNumRow, int iNumColumn)
{
	TUVRect tRt;
	TVector2 uv;

	// 4x4
	float fOffsetX = 1.0f / iNumColumn;
	float fOffsetY = 1.0f / iNumRow;
	for (int row = 0; row < iNumRow; row++)
	{
		uv.y = fOffsetY * row;
		for (int column = 0; column < iNumColumn; column++)
		{
			uv.x = fOffsetX * column;
			tRt.m_Min = uv;
			tRt.m_Max.x = uv.x + fOffsetX;
			tRt.m_Max.y = uv.y + fOffsetY;
			m_pUVList.push_back(tRt);
		}
	}
	m_fOffsetTime = m_fAnimTimer / m_pUVList.size();
}
bool TSpriteUV::Init()
{
	TPlaneObj::Init();
	return true;
}
bool TSpriteUV::Frame()
{
	TPlaneObj::Frame();
	m_fElapsedTimer += g_fSecondPerFrame;
	if (m_fElapsedTimer >= m_fOffsetTime)
	{
		m_iCurrentAnimIndex++;
		if (m_iCurrentAnimIndex >= m_pUVList.size())
		{
			m_iCurrentAnimIndex = 0;
		}
		m_fElapsedTimer -= m_fOffsetTime;
	}	
	return true;
}
bool TSpriteUV::Render()
{		
	TUVRect uv = m_pUVList[m_iCurrentAnimIndex];
	m_VertexList[0].t = uv.m_Min;
	m_VertexList[1].t.x = uv.m_Max.x; 
	m_VertexList[1].t.y = uv.m_Min.y;
	
	m_VertexList[2].t.x = uv.m_Min.x; 
	m_VertexList[2].t.y = uv.m_Max.y;
	m_VertexList[3].t = m_VertexList[2].t;
	
	m_VertexList[4].t= m_VertexList[1].t;
	m_VertexList[5].t= uv.m_Max;

	m_pImmediateContext->UpdateSubresource(		
		m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0), 0, 0);

	PreRender();
	PostRender();
	return true;
}
bool TSpriteUV::Release()
{
	TPlaneObj::Release();
	return true;
}

TSpriteUV::TSpriteUV()
{
	Init();
}
TSpriteUV::~TSpriteUV()
{
	Release();
}