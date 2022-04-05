#define  _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"
bool	TFbxObj::CreateConstantBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TBoneWorld);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(hr = pDevice->CreateBuffer(&bd, NULL,&m_pBoneCB)))
	{
		return false;
	}
	return true;
}
bool	TFbxObj::Init()
{	
	return true;
}
bool	TFbxObj::Frame()
{
	m_fTime += g_fSecPerFrame * m_Scene.iFrameSpeed * m_fDir * m_fSpeed;
	if (m_fTime >= m_Scene.iEnd)
	{
		m_fDir *= -1.0f;
	}
	if (m_fTime <= m_Scene.iStart)
	{
		m_fDir *= -1.0f;
	}
	int iFrame = m_fTime;
	iFrame = max(0, min(m_Scene.iEnd, iFrame));
	for (int iObj = 0; iObj < m_pImporter->m_TreeList.size(); iObj++)
	{
		TFbxModel* pObject = m_pImporter->m_TreeList[iObj];
		if (pObject->m_AnimTrack.size() > 0)
		{
			m_matBoneArray.matBoneWorld[iObj] = pObject->m_AnimTrack[iFrame].matTrack;
		}
		T::D3DXMatrixTranspose(
			&m_matBoneArray.matBoneWorld[iObj],
			&m_matBoneArray.matBoneWorld[iObj]);
	}

	m_pContext->UpdateSubresource(m_pBoneCB, 0, NULL,&m_matBoneArray, 0, 0);
	return true;
}
bool	TFbxObj::Render()
{	
	m_pContext->VSSetConstantBuffers(2, 1, &m_pBoneCB);		
	for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
	{
		TFbxModel* pFbxObj = m_DrawList[iObj];
		T::TVector3 vLight(cosf(g_fGameTimer) * 100.0f,	100,sinf(g_fGameTimer) * 100.0f);
		T::D3DXVec3Normalize(&vLight, &vLight);
		vLight = vLight * -1.0f;
		pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
		pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
		pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
		pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;
		pFbxObj->Render();
	}
	return true;
}
bool	TFbxObj::Release()
{
	if(m_pBoneCB)m_pBoneCB->Release();
	m_pBoneCB = nullptr;
	for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
	{
		m_DrawList[iObj]->Release();
		delete m_DrawList[iObj];
		m_DrawList[iObj] = nullptr;
	}
	return true;
}