#include "pch.h"
#include "TFbxFile.h"
HRESULT	TFbxFile::CreateConstantBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	for (int iBone = 0; iBone < 255; iBone++)
	{
		D3DXMatrixIdentity(&m_cbDataBone.matBone[iBone]);
	}

	D3D11_BUFFER_DESC       bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CONSTANT_BONE_BUFFER) * 1; // 바이트 용량
	// GPU 메모리에 할당
	bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA  sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_cbDataBone;
	hr = pDevice->CreateBuffer(
		&bd, // 버퍼 할당
		&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
		&m_pAnimBoneCB);
	return hr;
}
bool TFbxFile::UpdateFrame(ID3D11DeviceContext* pContext)
{
	m_fAnimFrame = m_fAnimFrame +
		g_fSecondPerFrame * m_fAnimSpeed *	m_AnimScene.fFrameSpeed * m_fAnimInverse;
	if (m_fAnimFrame > m_AnimScene.iEndFrame ||
		m_fAnimFrame < m_AnimScene.iStartFrame)
	{
		m_fAnimFrame = min(m_fAnimFrame, m_AnimScene.iEndFrame);
		m_fAnimFrame = max(m_fAnimFrame, m_AnimScene.iStartFrame);
		m_fAnimInverse *= -1.0f;
	}

	// object + skinning
	std::vector<TMatrix> matCurrentAnimList;
	for (int iBone = 0; iBone < m_pObjectList.size(); iBone++)
	{
		TMatrix matAnimation = m_pObjectList[iBone]->Interplate(m_fAnimFrame, m_AnimScene);		
		D3DXMatrixTranspose(&m_cbDataBone.matBone[iBone], &matAnimation);
		matCurrentAnimList.push_back(matAnimation);
	}
	pContext->UpdateSubresource(m_pAnimBoneCB, 0, nullptr,&m_cbDataBone, 0, 0);

	// skinning
	for (int iDraw = 0; iDraw < m_pDrawObjList.size(); iDraw++)
	{
		if (m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.size())
		{
			for (int iBone = 0; iBone < m_pObjectList.size(); iBone++)
			{
				auto iter = m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.find(iBone);
				if (iter != m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.end())
				{
					TMatrix matBind = iter->second;
					TMatrix matAnim = matBind * matCurrentAnimList[iBone];
					D3DXMatrixTranspose(&m_cbDataBone.matBone[iBone], &matAnim);
				}
			}
			pContext->UpdateSubresource(
				m_pDrawObjList[iDraw]->m_pSkinBoneCB, 0, nullptr,
				&m_cbDataBone, 0, 0);
		}				
	}	
	return true;
}
void TFbxFile::UpdateSkeleton(ID3D11DeviceContext* pContext,
			float fTime, 
			VS_CONSTANT_BONE_BUFFER& cbData)
{
	for (int iBone = 0; iBone < m_pObjectList.size(); iBone++)
	{
		TMatrix matAnimation = m_pObjectList[iBone]->Interplate(fTime, m_AnimScene);
		//D3DXMatrixTranspose(&cbData.matBone[iBone], &matAnimation);
		cbData.matBone[iBone] = matAnimation;		
	}	
}
void TFbxFile::UpdateSkinning(ID3D11DeviceContext* pContext, 
							VS_CONSTANT_BONE_BUFFER& cbInputData,
	std::vector< VS_CONSTANT_BONE_BUFFER>& cbOutputData)
{
	// skinning
	for (int iDraw = 0; iDraw < m_pDrawObjList.size(); iDraw++)
	{
		if (m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.size())
		{
			for (int iBone = 0; iBone < m_pObjectList.size(); iBone++)
			{
				auto iter = m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.find(iBone);
				if (iter != m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.end())
				{
					TMatrix matBind = iter->second;
					TMatrix matAnim = matBind * cbInputData.matBone[iBone];
					D3DXMatrixTranspose(&cbOutputData[iDraw].matBone[iBone], &matAnim);
				}
			}		
		}
	}	
}
bool TFbxFile::Render(ID3D11DeviceContext* pContext)
{
	pContext->VSSetConstantBuffers(1, 1, &m_pAnimBoneCB);	
	for (auto obj : m_pDrawObjList)
	{			
		obj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		obj->Render(pContext);
	}
	return true;
}
void	TFbxFile::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
	if (matWorld != nullptr)
	{
		m_matWorld = *matWorld;
	}
	if (matView != nullptr)
	{
		m_matView = *matView;
	}
	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}
}