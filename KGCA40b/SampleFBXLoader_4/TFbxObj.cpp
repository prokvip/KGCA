#define  _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"
bool	TFbx::Init()
{	
	m_fTime = 61;
	return true;
}
bool	TFbx::Frame()
{
	/*m_fTime += g_fSecPerFrame * m_pMeshImp->m_Scene.iFrameSpeed * m_fDir * m_fSpeed;
	if (m_fTime >= m_pMeshImp->m_Scene.iEnd)
	{
		m_fDir *= -1.0f;
	}
	if (m_fTime <= m_pMeshImp->m_Scene.iStart)
	{
		m_fDir *= -1.0f;
	}
	int iFrame = m_fTime;
	iFrame = max(0, min(m_pMeshImp->m_Scene.iEnd, iFrame));*/
	/*for (int iObj = 0; iObj < m_pMeshImp->m_TreeList.size(); iObj++)
	{
		TFbxModel* pObject = m_pMeshImp->m_TreeList[iObj];
		if (pObject->m_AnimTrack.size() > 0)
		{
			auto binepose = pObject->m_matBindPoseMap.find(pObject->m_iIndex);
			if (binepose != pObject->m_matBindPoseMap.end())
			{
				TMatrix matInverseBindpose = binepose->second;
				m_matBoneArray.matBoneWorld[iObj] =
					matInverseBindpose *
					pObject->m_AnimTrack[iFrame].matTrack;
			}
			else
			{
				m_matBoneArray.matBoneWorld[iObj] =					
					pObject->m_AnimTrack[iFrame].matTrack;
			}
		}
		T::D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[iObj],&m_matBoneArray.matBoneWorld[iObj]);
	}	*/
	return true;
}
T::TMatrix TFbx::Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fTime)
{
	T::TMatrix matAnim;
	TScene tScene = pAnimImp->m_Scene;
	int iStart = max(tScene.iStart, fTime);
	int iEnd = min(tScene.iEnd, fTime + 1);
	// º¸°£ = A ~ 7.5f ~ B
	//       9.5f <=10   ~     20 -> 20.1
	TTrack A = pModel->m_AnimTrack[iStart];
	TTrack B = pModel->m_AnimTrack[iEnd];
	float s = fTime- (float)iStart; // 0~1
	T::TVector3 pos;
	T::D3DXVec3Lerp(&pos, &A.t, &B.t, s);
	T::TVector3 scale;
	T::D3DXVec3Lerp(&scale, &A.s, &B.s, s);
	T::TQuaternion rotation;
	T::D3DXQuaternionSlerp(&rotation, &A.r, &B.r, s);
	T::TMatrix matScale;
	T::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	T::TMatrix matRotation;
	T::D3DXMatrixRotationQuaternion(&matRotation, &rotation);
	
	matAnim = matScale * matRotation;
	matAnim._41 = pos.x;
	matAnim._42 = pos.y;
	matAnim._43 = pos.z;
	//T::TMatrix matTrans;
	//T::D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
	//matAnim = pModel->m_AnimTrack[(int)fTime].matTrack;
	return matAnim;
}
bool	TFbx::Render()
{		
	TFbxImporter* pAnimImp = nullptr;
	if (m_pAnimImporter!=nullptr)
	{
		pAnimImp = m_pAnimImporter;
	}
	else
	{
		pAnimImp = m_pMeshImp;
	}
	m_fTime += g_fSecPerFrame * pAnimImp->m_Scene.iFrameSpeed * m_fDir * 0.33f;// m_fSpeed;
	if (m_fTime >= pAnimImp->m_Scene.iEnd)
	{
		//m_fDir *= -1.0f;
		m_fTime = pAnimImp->m_Scene.iStart;
	}

	int iFrame = m_fTime;
	iFrame = max(0, min(pAnimImp->m_Scene.iEnd-1, iFrame));

	for (int iObj = 0; iObj < m_pMeshImp->m_DrawList.size(); iObj++)	
	{
		TFbxModel* pFbxObj = m_pMeshImp->m_DrawList[iObj];	
		if (_tcsstr(pFbxObj->m_csName.c_str(), L"LOD") != nullptr)// != L"SK_Mannequin_LOD0")
		{
			if (_tcsstr(pFbxObj->m_csName.c_str(), L"LOD0") == nullptr)// != L"SK_Mannequin_LOD0")
			{
				continue;
			}
		}

		if (pFbxObj->m_bSkinned)
		{			
			for( auto data : pAnimImp->m_pFbxModelMap)
			{
				std::wstring name = data.first;
				TFbxModel* pAnimModel = data.second;
				auto model = m_pMeshImp->m_pFbxModelMap.find(name);
				if (model == m_pMeshImp->m_pFbxModelMap.end())
				{
					continue; // error
				}
				TFbxModel* pTreeModel = model->second;
				if (pTreeModel == nullptr)
				{
					continue; // error
				}
				auto binepose = pFbxObj->m_dxMatrixBindPoseMap.find(name);
				if (binepose != pFbxObj->m_dxMatrixBindPoseMap.end()&& pAnimModel)
				{
					TMatrix matInverseBindpose = binepose->second;
					m_matBoneArray.matBoneWorld[pTreeModel->m_iIndex] =
						matInverseBindpose *				
						Interplate(pAnimImp, pAnimModel, m_fTime);
						//pAnimModel->m_AnimTrack[iFrame].matTrack;
				}
				T::D3DXMatrixTranspose( &m_matBoneArray.matBoneWorld[pTreeModel->m_iIndex],
										&m_matBoneArray.matBoneWorld[pTreeModel->m_iIndex]);
			}
		}
		else
		{
			for (int inode = 0; inode < m_pMeshImp->m_TreeList.size(); inode++)
			{		
				TFbxModel* pFbxModel = m_pMeshImp->m_TreeList[inode];
				if (pFbxModel->m_AnimTrack.size() > 0)
				{					
					m_matBoneArray.matBoneWorld[inode] =
						Interplate(pAnimImp, pFbxModel, m_fTime);
						//pFbxModel->m_AnimTrack[iFrame].matTrack;
					
				}
				T::D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[inode],
					&m_matBoneArray.matBoneWorld[inode]);
			}
		}

		m_pContext->UpdateSubresource(m_pMeshImp->m_pBoneCB, 0, NULL, &m_matBoneArray, 0, 0);
		m_pContext->VSSetConstantBuffers(2, 1, &m_pMeshImp->m_pBoneCB);

		T::TVector3 vLight(cosf(g_fGameTimer) * 100.0f,	100,sinf(g_fGameTimer) * 100.0f);
		T::D3DXVec3Normalize(&vLight, &vLight);
		vLight = vLight * -1.0f;
		pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
		pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
		pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
		pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;
		pFbxObj->m_LightConstantList.vCameraDir.x = 
			m_pMainCamera->m_vLook.x;
		pFbxObj->m_LightConstantList.vCameraDir.y = 
			m_pMainCamera->m_vLook.y;
		pFbxObj->m_LightConstantList.vCameraDir.z = 
			m_pMainCamera->m_vLook.z;
		pFbxObj->m_LightConstantList.vCameraDir.w = 1.0f;
		pFbxObj->m_LightConstantList.vCameraPos.x = 
			m_pMainCamera->m_vCamera.x;
		pFbxObj->m_LightConstantList.vCameraPos.y = 
			m_pMainCamera->m_vCamera.y;
		pFbxObj->m_LightConstantList.vCameraPos.z = 
			m_pMainCamera->m_vCamera.z;
		pFbxObj->m_LightConstantList.vCameraPos.w = 1.0f;
		pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pFbxObj->Render();
	}
	return true;
}
bool	TFbx::Release()
{
	return true;
}