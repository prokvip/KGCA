#pragma once
#include "TCore.h"
#include "TFbxObj.h"
#include "TShape.h"
class TMiniMap : public TPlaneShape
{
public:  
    bool		LoadTexture()
    {
        m_Tex.LoadTexture(L"../../data/kgcabk.bmp");
    }
    bool CreateVertexData() 
    {
        m_pVertexList.resize(4);

        m_pVertexList[0].pos = TVector3(-1.0f, 1.0f, 0.0f);
        m_pVertexList[0].normal = TVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[0].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[0].tex = TVector2(0.0f, 0.0f);

        m_pVertexList[1].pos = TVector3(-0.5f, 1.0f, 0.0f);
        m_pVertexList[1].normal = TVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[1].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[1].tex = TVector2(1.0f, 0.0f);


        m_pVertexList[2].pos = TVector3(-1.0f, 0.5f, 0.0f);
        m_pVertexList[2].normal = TVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[2].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[2].tex = TVector2(0.0f, 1.0f);


        m_pVertexList[3].pos = TVector3(-0.5f, 0.5f, 0.0f);
        m_pVertexList[3].normal = TVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[3].color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[3].tex = TVector2(1.0f, 1.0f);
        return true;
    }
};
class Sample :   public TCore
{
    TMiniMap	m_MiniMap;
    TPlaneShape	m_MapObj;
	TViewRT     m_Rt;
	TVector3	m_vMoePos;
	std::vector<TFbxObj*> m_pObjectList;
	TFbxObj		m_FbxObjA;
	TFbxObj		m_FbxObjB;		
	TMatrix		m_matShadow;
	ID3D11PixelShader* m_pPSShadow = nullptr;
    ID3D11PixelShader* m_pPSShadowColor = nullptr;
public:
	ID3D11DepthStencilState* m_pDsvState;
	HRESULT     CreateDepthStencilState();
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

