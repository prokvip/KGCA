#define  _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "TFbxObj.h"
namespace TBasisFBX
{
	bool	TFbx::Init()
	{
		m_fTime = 61;
		return true;
	}
	bool	TFbx::Frame(float fSecPerFrame, float fGameTimer)
	{
		TObject3D::Frame(fSecPerFrame, fGameTimer);

		TFbxImporter* pAnimImp = nullptr;
		if (m_pAnimImporter != nullptr)
		{
			pAnimImp = m_pAnimImporter;
		}
		else
		{
			pAnimImp = m_pMeshImp;
		}
		if (pAnimImp)
		{
			m_fTime += fSecPerFrame * pAnimImp->m_Scene.iFrameSpeed * m_fDir * 0.33f;// m_fSpeed;
			if (m_fTime >= pAnimImp->m_Scene.iEnd)
			{
				//m_fDir *= -1.0f;
				m_fTime = pAnimImp->m_Scene.iStart;
			}

			//int iFrame = m_fTime;
			//iFrame = max(0, min(pAnimImp->m_Scene.iEnd - 1, iFrame));

			// 스킨(메쉬)와 바인드포즈(에니메이션행렬)의 노드개수가 다른 수 있다.
			TBoneWorld matAnimation;
			for (int inode = 0; inode < m_pMeshImp->m_TreeList.size(); inode++)
			{
				TFbxModel* pMeshModel = m_pMeshImp->m_TreeList[inode];
				auto model = pAnimImp->m_pFbxModelMap.find(pMeshModel->m_csName);
				if (model == pAnimImp->m_pFbxModelMap.end())
				{
					continue;
				}
				TFbxModel* pAnimModel = model->second;
				matAnimation.matBoneWorld[inode] = Interplate(pAnimImp, pAnimModel, m_fTime);
			}

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
					for (auto data : pFbxObj->m_dxMatrixBindPoseMap)
					{
						std::wstring name = data.first;
						TMatrix matBindPose = data.second;
						auto model = m_pMeshImp->m_pFbxModelMap.find(name);
						if (model == m_pMeshImp->m_pFbxModelMap.end())
						{
							continue;
						}
						int iIndex = model->second->m_iIndex;
						pFbxObj->m_matBoneArray.matBoneWorld[iIndex] = matBindPose * matAnimation.matBoneWorld[iIndex];
						TBasisFBX::D3DXMatrixTranspose(&pFbxObj->m_matBoneArray.matBoneWorld[iIndex],
							&pFbxObj->m_matBoneArray.matBoneWorld[iIndex]);
					}
				}
				else
				{
					for (int inode = 0; inode < m_pMeshImp->m_TreeList.size(); inode++)
					{
						TBasisFBX::D3DXMatrixTranspose(&pFbxObj->m_matBoneArray.matBoneWorld[inode],
							&matAnimation.matBoneWorld[inode]);
					}
				}
			}
		}
		return true;
	}
	TBasisFBX::TMatrix TFbx::Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fTime)
	{
		TBasisFBX::TMatrix matAnim;
		if (pModel->m_AnimTrack.size() <= 0)
		{
			return matAnim;
		}
		TScene tScene = pAnimImp->m_Scene;
		int iStart = max(tScene.iStart, fTime);
		int iEnd = min(tScene.iEnd, fTime + 1);
		// 보간 = A ~ 7.5f ~ B
		//       9.5f <=10   ~     20 -> 20.1
		TTrack A = pModel->m_AnimTrack[iStart];
		TTrack B = pModel->m_AnimTrack[iEnd];
		float s = fTime - (float)iStart; // 0~1
		TBasisFBX::TVector3 pos;
		TBasisFBX::D3DXVec3Lerp(&pos, &A.t, &B.t, s);
		TBasisFBX::TVector3 scale;
		TBasisFBX::D3DXVec3Lerp(&scale, &A.s, &B.s, s);
		TBasisFBX::TQuaternion rotation;
		TBasisFBX::D3DXQuaternionSlerp(&rotation, &A.r, &B.r, s);
		TBasisFBX::TMatrix matScale;
		TBasisFBX::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
		TBasisFBX::TMatrix matRotation;
		TBasisFBX::D3DXMatrixRotationQuaternion(&matRotation, &rotation);

		matAnim = matScale * matRotation;
		matAnim._41 = pos.x;
		matAnim._42 = pos.y;
		matAnim._43 = pos.z;
		//TBasisFBX::TMatrix matTrans;
		//TBasisFBX::D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
		//matAnim = pModel->m_AnimTrack[(int)fTime].matTrack;
		return matAnim;
	}
	bool	TFbx::Render()
	{
		if (m_pMeshImp == nullptr) return true;
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
			if (_tcsstr(pFbxObj->m_csName.c_str(), L"UCX") != nullptr)// != L"SK_Mannequin_LOD0")
			{
				continue;
			}
			m_pContext->UpdateSubresource(m_pMeshImp->m_pBoneCB, 0, NULL, &pFbxObj->m_matBoneArray, 0, 0);
			m_pContext->VSSetConstantBuffers(2, 1, &m_pMeshImp->m_pBoneCB);

			//TBasisFBX::TVector3 vLight(cosf(m_fGameTimer) * 100.0f, 500, sinf(m_fGameTimer) * 100.0f);
			TBasisFBX::TVector3 vLight(100.0f, 500, 0);
			TBasisFBX::D3DXVec3Normalize(&vLight, &vLight);
			vLight = vLight * -1.0f;
			pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
			pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
			pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
			pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;
			pFbxObj->m_LightConstantList.vCameraDir.x = m_CameraInfo.m_vLook.x;
			pFbxObj->m_LightConstantList.vCameraDir.y = m_CameraInfo.m_vLook.y;
			pFbxObj->m_LightConstantList.vCameraDir.z = m_CameraInfo.m_vLook.z;
			pFbxObj->m_LightConstantList.vCameraDir.w = 1.0f;
			pFbxObj->m_LightConstantList.vCameraPos.x = m_CameraInfo.m_vCamera.x;
			pFbxObj->m_LightConstantList.vCameraPos.y = m_CameraInfo.m_vCamera.y;
			pFbxObj->m_LightConstantList.vCameraPos.z = m_CameraInfo.m_vCamera.z;
			pFbxObj->m_LightConstantList.vCameraPos.w = 1.0f;

			pFbxObj->m_ConstantList.Timer.x = m_iShadowID;
			pFbxObj->m_ConstantList.Color = m_vShadowColor;
			pFbxObj->m_LightConstantList.matLight = m_LightConstantList.matLight;
			pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);

			pFbxObj->m_bAlphaBlend = m_bAlphaBlend;
			pFbxObj->m_pVShader = m_pVShader;
			pFbxObj->m_pPShader = m_pPShader;
			pFbxObj->PreRender();
			pFbxObj->Draw();
			pFbxObj->PostRender();
		}
		return true;
	}
	bool	TFbx::RenderShadow(TShader* pShader)
	{
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
			m_pContext->UpdateSubresource(m_pMeshImp->m_pBoneCB, 0, NULL, &m_matBoneArray, 0, 0);
			m_pContext->VSSetConstantBuffers(2, 1, &m_pMeshImp->m_pBoneCB);

			TBasisFBX::TVector3 vLight(cosf(m_fGameTimer) * 100.0f, 100, sinf(m_fGameTimer) * 100.0f);
			TBasisFBX::D3DXVec3Normalize(&vLight, &vLight);
			vLight = vLight * -1.0f;
			pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
			pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
			pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
			pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;
			pFbxObj->m_LightConstantList.vCameraDir.x = m_CameraInfo.m_vLook.x;
			pFbxObj->m_LightConstantList.vCameraDir.y = m_CameraInfo.m_vLook.y;
			pFbxObj->m_LightConstantList.vCameraDir.z = m_CameraInfo.m_vLook.z;
			pFbxObj->m_LightConstantList.vCameraDir.w = 1.0f;
			pFbxObj->m_LightConstantList.vCameraPos.x = m_CameraInfo.m_vCamera.x;
			pFbxObj->m_LightConstantList.vCameraPos.y = m_CameraInfo.m_vCamera.y;
			pFbxObj->m_LightConstantList.vCameraPos.z = m_CameraInfo.m_vCamera.z;
			pFbxObj->m_LightConstantList.vCameraPos.w = 1.0f;


			pFbxObj->m_ConstantList.Timer.x = m_iShadowID;
			pFbxObj->m_ConstantList.Color = m_vShadowColor;
			pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
			if (pShader != nullptr)
			{
				pFbxObj->m_pPShader = pShader;
			}
			pFbxObj->PreRender();
			pFbxObj->Draw();
			auto bAlphaBlend = pFbxObj->m_bAlphaBlend;
			pFbxObj->m_bAlphaBlend = true;
			pFbxObj->PostRender();
			pFbxObj->m_bAlphaBlend = bAlphaBlend;
		}
		return true;
	}
	bool	TFbx::Release()
	{
		return true;
	}

	void        TFbx::GenAABB()
	{
		// aabb 
		m_BoxCollision.vMin = TBasisFBX::TVector3(100000, 100000, 100000);
		m_BoxCollision.vMax = TBasisFBX::TVector3(-100000, -100000, -100000);
		for (int i = 0; i < m_VertexList.size(); i++)
		{
			if (m_BoxCollision.vMin.x > m_VertexList[i].p.x)
			{
				m_BoxCollision.vMin.x = m_VertexList[i].p.x;
			}
			if (m_BoxCollision.vMin.y > m_VertexList[i].p.y)
			{
				m_BoxCollision.vMin.y = m_VertexList[i].p.y;
			}
			if (m_BoxCollision.vMin.z > m_VertexList[i].p.z)
			{
				m_BoxCollision.vMin.z = m_VertexList[i].p.z;
			}

			if (m_BoxCollision.vMax.x < m_VertexList[i].p.x)
			{
				m_BoxCollision.vMax.x = m_VertexList[i].p.x;
			}
			if (m_BoxCollision.vMax.y < m_VertexList[i].p.y)
			{
				m_BoxCollision.vMax.y = m_VertexList[i].p.y;
			}
			if (m_BoxCollision.vMax.z < m_VertexList[i].p.z)
			{
				m_BoxCollision.vMax.z = m_VertexList[i].p.z;
			}
		}

		// 4      5
		// 6      7

		// 0     1
		// 2     3
		m_BoxCollision.vList[0] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMin.z);
		m_BoxCollision.vList[1] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMin.z);
		m_BoxCollision.vList[2] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMin.z);
		m_BoxCollision.vList[3] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMin.z);

		m_BoxCollision.vList[4] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMax.z);
		m_BoxCollision.vList[5] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMax.z);
		m_BoxCollision.vList[6] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMax.z);
		m_BoxCollision.vList[7] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMax.z);
	}
};