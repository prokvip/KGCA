#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TDebugCamera.h"
#include "TQuadtree.h"
#include "TModelMgr.h"

class TMapObject : public TObject
{
public:
	TFbxObj* m_pFbxObject; // �𵨸Ŵ����� ����� ������ 
public:
	float   m_fCurrentAnimTime = 0.0f;
	UINT	m_iStartFrame = 0;
	UINT	m_iEndFrame = 0;
	UINT	m_iFrameSpeed = 30;
	UINT	m_iTickForFrame = 160;
public:
	TFbxMesh* m_ptMesh;
	std::vector<const TTexture*>			m_TexArray;
	std::vector<std::shared_ptr<TMapObject>>m_pSubObject;
public:
	void     LoadTextureArray(W_STR filename)
	{
		m_TexArray.push_back(I_Tex.Load(filename));
	}	
	bool   PostRender()
	{
		for (int isubMtrl = 0; isubMtrl < m_ptMesh->m_TriangleList.size(); isubMtrl++)
		{
			if (m_TexArray.size() && m_TexArray[isubMtrl])
			{
				m_TexArray[isubMtrl]->Apply(m_pImmediateContext, 0);
			}
			if (m_pIndexBuffer == nullptr)
			{
				m_pImmediateContext->Draw(m_ptMesh->m_TriangleList[isubMtrl].size(), 
									      m_ptMesh->m_TriangleOffsetList[isubMtrl]);
			}			
		}
		return true;
	}
};


class Sample : public TCore
{
	using NEW_OBJECT = std::shared_ptr<TMapObject>;
	NEW_OBJECT		m_MapObj;
	const TTexture* m_pTex;
	std::shared_ptr<TDebugCamera>  m_pDebugCamera;	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool  DeleteDxResource() override;
	bool  CreateDxResource() override;
};

