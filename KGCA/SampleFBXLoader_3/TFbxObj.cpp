#define  _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"
bool	TFbx::Init()
{	
	return true;
}
bool	TFbx::Frame()
{
	m_fTime += g_fSecPerFrame * m_pImporter->m_Scene.iFrameSpeed * m_fDir * m_fSpeed;
	if (m_fTime >= m_pImporter->m_Scene.iEnd)
	{
		m_fDir *= -1.0f;
	}
	if (m_fTime <= m_pImporter->m_Scene.iStart)
	{
		m_fDir *= -1.0f;
	}
	int iFrame = m_fTime;
	iFrame = max(0, min(m_pImporter->m_Scene.iEnd, iFrame));
	for (int iObj = 0; iObj < m_pImporter->m_TreeList.size(); iObj++)
	{
		TFbxModel* pObject = m_pImporter->m_TreeList[iObj];
		if (pObject->m_AnimTrack.size() > 0)
		{
			m_matBoneArray.matBoneWorld[iObj] = pObject->m_AnimTrack[iFrame].matTrack;
		}
		T::D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[iObj],&m_matBoneArray.matBoneWorld[iObj]);
	}	
	return true;
}
bool	TFbx::Render()
{	
	m_pContext->UpdateSubresource(m_pImporter->m_pBoneCB, 0, NULL, &m_matBoneArray, 0, 0);
	m_pContext->VSSetConstantBuffers(2, 1, &m_pImporter->m_pBoneCB);
	for (int iObj = 0; iObj < m_pImporter->m_DrawList.size(); iObj++)
	{
		TFbxModel* pFbxObj = m_pImporter->m_DrawList[iObj];

		T::TVector3 vLight(cosf(g_fGameTimer) * 100.0f,	100,sinf(g_fGameTimer) * 100.0f);
		T::D3DXVec3Normalize(&vLight, &vLight);
		vLight = vLight * -1.0f;
		pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
		pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
		pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
		pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;

		pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pFbxObj->Render();
	}
	return true;
}
bool	TFbx::Release()
{
	return true;
}