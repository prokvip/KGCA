#include "pch.h"
#include "TCharacter.h"
HRESULT	TCharacter::CreateConstantBuffer(ID3D11Device* pDevice)
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

	m_pSkinBoneCB.resize(m_pFbxFile->m_pDrawObjList.size());
	m_cbDrawGeom.resize(m_pFbxFile->m_pDrawObjList.size());
	for (int iMesh = 0; iMesh < m_pFbxFile->m_pDrawObjList.size(); iMesh++)
	{
		D3D11_SUBRESOURCE_DATA  sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &m_cbDataBone;
		hr = pDevice->CreateBuffer(
			&bd, // 버퍼 할당
			&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
			&m_pSkinBoneCB[iMesh]);
	}

	return hr;
}
bool TCharacter::UpdateFrame(ID3D11DeviceContext* pContext)
{
	m_fAnimFrame = m_fAnimFrame +
		g_fSecondPerFrame * m_fAnimSpeed *
		m_AnimScene.fFrameSpeed;// *m_fAnimInverse;
	if (m_fAnimFrame > m_ActionCurrent.iEndFrame ||
		m_fAnimFrame < m_ActionCurrent.iStartFrame)
	{
		m_fAnimFrame = min(m_fAnimFrame, m_ActionCurrent.iEndFrame);
		m_fAnimFrame = max(m_fAnimFrame, m_ActionCurrent.iStartFrame);
		//m_fAnimInverse *= -1.0f;
	}

	if (m_ActionCurrent.bLoop)
	{
		if (m_fAnimFrame >= m_ActionCurrent.iEndFrame)
		{
			m_fAnimFrame = m_ActionCurrent.iStartFrame;
		}
	}
	else
	{
		if (m_fAnimFrame >= m_ActionCurrent.iEndFrame)
		{
			//m_ActionCurrent = m_ActionList.find(L"walking")->second;
			m_fAnimFrame = m_ActionCurrent.iStartFrame;
		}
	}
	
	if (m_pAnionFbxFile)
	{
		m_pAnionFbxFile->UpdateSkeleton(pContext, m_fAnimFrame, m_cbDataBone);
		m_pFbxFile->UpdateSkinning(pContext, m_cbDataBone, m_cbDrawGeom);
	}
	else
	{
		m_pFbxFile->UpdateSkeleton(pContext, m_fAnimFrame, m_cbDataBone);
		m_pFbxFile->UpdateSkinning(pContext, m_cbDataBone, m_cbDrawGeom);
	}
	for (int ibone = 0; ibone < m_pSkinBoneCB.size(); ibone++)
	{
		pContext->UpdateSubresource(
			m_pSkinBoneCB[ibone], 0, nullptr,
			&m_cbDrawGeom[ibone], 0, 0);
	}

	for (int iBone = 0; iBone < m_pFbxFile->m_pObjectList.size(); iBone++)
	{
		D3DXMatrixTranspose(&m_cbDataBone.matBone[iBone], &m_cbDataBone.matBone[iBone]);
	}
	pContext->UpdateSubresource(m_pAnimBoneCB, 0, nullptr, &m_cbDataBone, 0, 0);

	return true;
}
void	TCharacter::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
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
bool TCharacter::Render(ID3D11DeviceContext* pContext)
{
	pContext->VSSetConstantBuffers(1, 1, &m_pAnimBoneCB);
	for (int iMesh = 0; iMesh < m_pFbxFile->m_pDrawObjList.size(); iMesh++)
	{
		if (m_pFbxFile->m_pDrawObjList[iMesh]->m_bSkinned)
		{
			pContext->VSSetConstantBuffers(1, 1, &m_pSkinBoneCB[iMesh]);
		}
		m_pFbxFile->m_pDrawObjList[iMesh]->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		m_pFbxFile->m_pDrawObjList[iMesh]->Render();
	}
	return true;
}
bool TCharacter::Release()
{
	if (m_pAnimBoneCB) m_pAnimBoneCB->Release();
	for (auto bone : m_pSkinBoneCB)
	{
		bone->Release();
	}
	return true;
}