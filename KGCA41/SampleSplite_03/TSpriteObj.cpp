#include "TSpriteObj.h"
bool  TSpriteObj::Load(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext,
	TSpriteInfo info)
{
	Set(pDevice, pContext);
	SetPos(info.p);
	SetScale(info.s);
	if (!info.texAlphaFile.empty())
	{
		this->m_pAlphaTex = I_Tex.Load(info.texAlphaFile);
	}
	return Create(info.texFile, info.shaderFile);
}
bool TSpriteTexture::Init()
{
	TPlaneObj::Init();

	const TTexture* pTex = I_Tex.Load(L"../../res/ui/0.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/1.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/2.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/3.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/4.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/5.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/6.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/7.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/8.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/9.png");
	m_pTexList.push_back(pTex);

	m_fOffsetTime = 3.0f / m_pTexList.size();
	return true;
}
bool TSpriteTexture::Frame()
{
	TPlaneObj::Frame();	
	m_fAnimTimer += g_fSecondPerFrame;
	if (m_pTexList[m_iCurrentAnimIndex] != nullptr)
	{
		if (m_fAnimTimer >= m_fOffsetTime)
		{
			m_iCurrentAnimIndex++;
			if (m_iCurrentAnimIndex >= m_pTexList.size())
			{
				m_iCurrentAnimIndex = 0;
			}
			m_fAnimTimer -= m_fOffsetTime;
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


bool TSpriteUV::Init()
{
	TPlaneObj::Init();
	TUVRect tRt;
	TVector2 uv;

	// 4x4
	float fOffset = 1.0f / 4;
	for (int i = 0; i < 4; i++)
	{
		uv.x = fOffset * i;
		for (int j = 0; j < 4; j++)
		{
			uv.y = fOffset * j;
			tRt.m_Min = uv;
			tRt.m_Max.x = uv.x + fOffset;
			tRt.m_Max.y = uv.y + fOffset;
			m_pUVList.push_back(tRt);
		}
	}	
	m_fOffsetTime = 3.0f / m_pUVList.size();
	return true;
}
bool TSpriteUV::Frame()
{
	TPlaneObj::Frame();
	m_fAnimTimer += g_fSecondPerFrame;	
	if (m_fAnimTimer >= m_fOffsetTime)
	{
		m_iCurrentAnimIndex++;
		if (m_iCurrentAnimIndex >= m_pUVList.size())
		{
			m_iCurrentAnimIndex = 0;
		}
		m_fAnimTimer -= m_fOffsetTime;
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