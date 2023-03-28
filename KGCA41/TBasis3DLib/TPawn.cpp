#include "TPawn.h"
#include "TObjMgr.h"
bool TPawn::Set(ID3D11Device* pd3dDevice, int iIndex)
{
	Init();
	TActor* const pModel = I_ObjMgr.GetPtr(iIndex);
	if (pModel == NULL) return false;
	m_cbData.Color = TVector4(1, 1, 1, 1);
	m_dxobj.g_pConstantBuffer.Attach(DX::CreateConstantBuffer(pd3dDevice, &m_cbData, 1, sizeof(VS_CONSTANT_BUFFER)));

	// 멀티 스레드로 처리되기 때문에 오브젝트 단위로 에미메이션 및 최종 행렬(인덱스)을 별도로 저장해 둔다. 
	shared_ptr<TObjWM> pWM = make_shared<TObjWM>((int)pModel->m_pMesh.size());
	for (DWORD dwObject = 0; dwObject < pModel->m_pMesh.size(); dwObject++)
	{
		pWM->m_iParent[dwObject] = -1;
		if (pModel->m_pMesh[dwObject]->m_pParent)
		{
			pWM->m_iParent[dwObject] = pModel->m_pMesh[dwObject]->m_pParent->m_iIndex;
		}
	}
	pWM->m_pModel = pModel;
	pWM->m_Scene = pModel->m_Scene;
	m_pModelList.push_back(pWM);
	return true;
}
void TPawn::Add(ID3D11Device* pDevice, shared_ptr<TPawn> pObj)
{
	m_pSubObjList.push_back(pObj);
}
void TPawn::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	TActor::SetMatrix(pWorld, pView, pProj);
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		m_pModelList[iModel]->m_pModel->SetMatrix(pWorld, pView, pProj);
	}
	//for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	//{
	//	m_pSubObjList[iObj]->SetMatrix(pWorld, pView, pProj);		
	//}
}

bool TPawn::Init()
{
	return true;
}
bool TPawn::Load(ID3D11Device* pd3dDevice,
	const TCHAR* strFileName,
	const TCHAR* strShaderName, bool bThread)
{
	m_iObjectIndex = I_ObjMgr.Load(pd3dDevice, strFileName, strShaderName, bThread);
	if (m_iObjectIndex < 0)
	{
		I_ObjMgr.Delete(m_iObjectIndex);
		return false;
	}
	return Set(pd3dDevice, m_iObjectIndex);
}
bool TPawn::ObjectFrame()
{
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		TObjWM* pUnit = m_pModelList[iModel].get();
		pUnit->m_fElapseTime += g_fSecPerFrame * pUnit->m_Scene.iFrameSpeed * pUnit->m_Scene.iTickPerFrame;
		if (pUnit->m_fElapseTime >= pUnit->m_Scene.iLastFrame* pUnit->m_Scene.iTickPerFrame)
		{
			pUnit->m_fElapseTime = (float)(pUnit->m_Scene.iFirstFrame * pUnit->m_Scene.iTickPerFrame);
		}

		for (int iMesh = 0; iMesh < pUnit->m_Scene.iNumMesh; iMesh++)
		{
			D3DXMatrixIdentity(&pUnit->m_matAnim[iMesh]);
			D3DXMatrixIdentity(&pUnit->m_matCalculation[iMesh]);
			int iParent = pUnit->m_iParent[iMesh];
			TMatrix mat;
			if (iParent >= 0)
			{
				pUnit->m_matAnim[iMesh] = m_Animation.Update(pUnit->m_fElapseTime,
					pUnit->m_pModel->m_pMesh[iMesh].get(),
					pUnit->m_matAnim[iParent],
					pUnit->m_Scene);
			}
			else
			{
				pUnit->m_matAnim[iMesh] = m_Animation.Update(pUnit->m_fElapseTime,
					pUnit->m_pModel->m_pMesh[iMesh].get(),
					mat,
					pUnit->m_Scene);
			}
			D3DXMatrixMultiply(&pUnit->m_matCalculation[iMesh], &pUnit->m_matAnim[iMesh], &m_matControlWorld);
		}
	}	
	return true;
}
bool TPawn::ObjectRender(ID3D11DeviceContext*    pContext, bool bCommand)
{	
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		TObjWM* pUnit = m_pModelList[iModel].get();
		PreDraw(pContext, pUnit, bCommand);
		PostDraw(pContext, pUnit, bCommand);
	}
	return true;
}
bool TPawn::PreDraw(ID3D11DeviceContext*    pContext, TObjWM* pUnit, bool bCommand)
{
	pUnit->m_pModel->PreRender(pContext);
	return true;
}
bool TPawn::PostDraw(ID3D11DeviceContext*    pContext, TObjWM* pUnit, bool bCommand)
{
	Draw(pContext, pUnit, bCommand);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void TPawn::SetConstantBuffers(ID3D11DeviceContext* pContext, TObjWM* pUint, int iMesh)
{
	D3DXMatrixMultiply(&pUint->m_matCalculation[iMesh], &pUint->m_matAnim[iMesh], &m_matControlWorld);
	if (m_bDefferedMultiThread)
	{
		// m_pModel은 공용 메쉬의 상수버퍼이기 때문에 동일한 오브젝트를 사용할 경우 멀티쓰래드에서 문제가 생긴다.
		// 그렇기 때문에 오브젝트 단위의 상수버퍼를 사용한다.		
		SetMatrix(&pUint->m_matCalculation[iMesh], &m_matView, &m_matProj);
		pContext->UpdateSubresource(m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
		pContext->VSSetConstantBuffers(0, 1, m_dxobj.g_pConstantBuffer.GetAddressOf());
	}
	else
	{
		pUint->m_pModel->SetMatrix(&pUint->m_matCalculation[iMesh], &m_matView, &m_matProj);
		UpdateConstantBuffer(pContext, pUint->m_pModel);
	}
}
bool TPawn::Draw(ID3D11DeviceContext* pContext, TObjWM* pUint, bool bCommand)
{
	//CStopwatch stopwatch;	
	TActor* pModel = pUint->m_pModel;
	for (DWORD iMesh = 0; iMesh < pModel->m_pMesh.size(); iMesh++)
	{
		auto pMesh = pModel->m_pMesh[iMesh].get();	
		if (bCommand == false)
		{
			SetConstantBuffers(pContext, pUint, iMesh);
		}
		//D3DXMatrixMultiply(&m_pObjWM->m_matCalculation[iMesh], &m_pObjWM->m_matAnim[iMesh], &m_matWorld);
	/*	SetMatrix(&m_pObjWM->m_matCalculation[iMesh], &m_matView, &m_matProj);
		pContext->UpdateSubresource(m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
		pContext->VSSetConstantBuffers(0, 1, m_dxobj.g_pConstantBuffer.GetAddressOf());	*/	

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_dxobj.m_iNumIndex,
					pSubMesh->m_dxobj.m_iBeginIB,
					pSubMesh->m_dxobj.m_iBeginVB);
				if (bCommand) 	SetCommmandList(pContext, pSubMesh, true);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;
			pContext->PSSetShaderResources(0, 1, pMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_dxobj.m_iNumIndex,
				pMesh->m_dxobj.m_iBeginIB,
				pMesh->m_dxobj.m_iBeginVB);
			if (bCommand) 	SetCommmandList(pContext, pMesh, true);
		}
	}
	//stopwatch.Output(L"\nTTbsObj::Draw");
	return true;
}
//
//bool TPawn::DrawGroup(ID3D11DeviceContext* pContext, TActor* pObj, int iObject,  bool bCommand)
//{
//	/*if (pObj->m_pMesh[iObject]->m_pSubMesh.size() > 0)
//	{
//		for (DWORD dwSubCount = 0; dwSubCount < pObj->m_pMesh[iObject]->m_pSubMesh.size(); dwSubCount++)
//		{
//			TMesh* pSubMesh = (TMesh*)pObj->m_pMesh[iObject]->m_pSubMesh[dwSubCount];
//			if (DrawMesh(pContext, pObj, pSubMesh, pApply))
//			{
//				if (bCommand) 	SetCommmandList(pContext, pSubMesh, true);
//			}
//		}
//	}
//	else
//	{
//		if (DrawMesh(pContext, pObj, pObj->m_pMesh[iObject], pApply))
//		{
//			if (bCommand) 	SetCommmandList(pContext, pObj->m_pMesh[iObject], true);
//		}
//	}*/
//	return true;
//}
bool TPawn::Frame()
{
	ObjectFrame();
	//for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	//{
	//	m_pSubObjList[iObj]->ObjectFrame();
	//}	
	return true;
}
bool TPawn::Render(ID3D11DeviceContext*    pContext)
{
	ObjectRender(pContext);
	/*for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	{
		m_pSubObjList[iObj]->ObjectRender( pContext);
	}*/
	return true;
}
bool TPawn::Release()
{
	m_pSubObjList.clear();
	return true;
}
bool TPawn::ResetResource()
{
	return true;
}
void TPawn::SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd)
{	
	m_iStartFrame = dwStrat;
	m_iLastFrame = dwEnd;
	m_fElapseTime = (float)dwStrat;
	for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	{
		m_pSubObjList[iObj]->m_iStartFrame = dwStrat;
		m_pSubObjList[iObj]->m_iLastFrame = dwEnd;
		m_pSubObjList[iObj]->m_fElapseTime = (float)dwStrat;		
	}	
}
//////////////////////////////////////////////////////////////////////////
// 명령목록 생성
//////////////////////////////////////////////////////////////////////////
bool TPawn::SetCommandRender(ID3D11DeviceContext* pContext)
{
	ObjectRender(pContext, true);
	/*for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	{
		m_pSubObjList[iObj]->ObjectRender(pContext, true);
	}*/
	return true;
}
//////////////////////////////////////////////////////////////////////////
// 명령목록 실행
//////////////////////////////////////////////////////////////////////////
bool TPawn::CommandRender(ID3D11DeviceContext* pContext)
{
	for (int iModel = 0; iModel < m_pModelList.size(); iModel++)
	{
		TObjWM* pUnit = m_pModelList[iModel].get();
		CommandDraw(pContext, pUnit);
	}
	
	/*for (int iObj = 0; iObj < m_pSubObjList.size(); iObj++)
	{
		TPawn* pObj = m_pSubObjList[iObj].get();
		_ASSERT(pObj);
		pObj->CommandRender(pContext);
	}*/
	return true;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool TPawn::CommandDraw(ID3D11DeviceContext* pContext, TObjWM* pUnit)
{
	TActor* pModel = pUnit->m_pModel;
	for (DWORD iMesh = 0; iMesh < pModel->m_pMesh.size(); iMesh++)
	{
		SetConstantBuffers(pContext, pUnit, iMesh);

		if (pModel->m_pMesh[iMesh]->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSubCount = 0; dwSubCount < pModel->m_pMesh[iMesh]->m_pSubMesh.size(); dwSubCount++)
			{
				TMesh* pSubMesh = (TMesh*)pModel->m_pMesh[iMesh]->m_pSubMesh[dwSubCount].get();
				if (pSubMesh->m_iNumFace <= 0) continue;
				ExecuteCommandList(pContext, pSubMesh, false);
			}
		}
		else
		{
			if (pModel->m_pMesh[iMesh]->m_iNumFace <= 0) continue;
			ExecuteCommandList(pContext, pModel->m_pMesh[iMesh].get(), false);
		}
	}

	return true;
}
HRESULT TPawn::SetCommmandList(ID3D11DeviceContext* pContext, TMesh* pSubMesh, bool bSave)
{
	HRESULT hr = S_OK;
	if (pContext)
	{
		return pContext->FinishCommandList(bSave, &pSubMesh->m_pd3dCommandList);
	}
	return E_FAIL;
}
void TPawn::ExecuteCommandList(ID3D11DeviceContext* pContext, TMesh* pMesh, bool bClear)
{
	assert(pMesh && pMesh->m_pd3dCommandList!=nullptr);
	pContext->ExecuteCommandList(pMesh->m_pd3dCommandList.Get(), !bClear);

	if (bClear)
	{
		pMesh->m_pd3dCommandList.ReleaseAndGetAddressOf();
	}
}
TPawn::TPawn(void)
{
	m_iObjectIndex = -1;
	m_fElapseTime = 0.0f;
	m_bDefferedMultiThread = false;
}
TPawn::TPawn(int iIndex)
{
	m_iObjectIndex = -1;
	m_fElapseTime = 0.0f;
	m_bDefferedMultiThread = false;
}
TPawn::TPawn(TPawn &obj)
{
	m_fElapseTime = 0.0f;
	m_bDefferedMultiThread = obj.m_bDefferedMultiThread;
}
TPawn::~TPawn(void)
{
}