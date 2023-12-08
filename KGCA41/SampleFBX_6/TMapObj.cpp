#include "TMapObj.h"
#include "ICoreStd.h"

bool TMapObj::Render()
{
	
	auto tFbxMeshList = m_pModel->m_DrawList;
	for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
	{
		TFbxObj* obj = tFbxMeshList[iSub].get();
		TMatrix matWorld = m_pModel->m_MatrixArray[m_fCurrentAnimTime][obj->m_iBoneIndex]* m_matControl;
		obj->SetMatrix(&matWorld,
			&ICore::g_pMainCamera->m_matView,
			&ICore::g_pMainCamera->m_matProj);
		obj->PreRender();
		//obj->PostRender();
		for (int isubMtrl = 0; isubMtrl < obj->m_TriangleList.size(); isubMtrl++)
		{
			if (obj->m_TexArray.size() && obj->m_TexArray[isubMtrl])
			{
				obj->m_TexArray[isubMtrl]->Apply(obj->m_pImmediateContext, 0);
			}
			if (obj->m_pIndexBuffer == nullptr)
			{
				obj->m_pImmediateContext->Draw(obj->m_TriangleList[isubMtrl].size(),
					obj->m_TriangleOffsetList[isubMtrl]);
			}
		}
	}	
	return true;
}
bool  TMapObj::Frame()
{
	m_fCurrentAnimTime +=		m_pModel->GetFrameSpeed() * g_fSecondPerFrame * 0.5f;
	if (m_fCurrentAnimTime >=	m_pModel->GetEndFrame())
	{
		m_fCurrentAnimTime =	m_pModel->GetStartFrame();
	}
	return true;
}
bool	TMapObj::CreateBoneBuffer()
{	
	return true;
}

bool TMapObjSkinning::Render()
{	
	
	TMatrix matWorld;
	auto tFbxMeshList = m_pModel->m_DrawList;
	for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
	{
		TFbxObj* obj = tFbxMeshList[iSub].get();
		////// 메쉬단위로 영향 행렬이 다르다. 50개
		for (auto data : m_pModel->m_pFbxNodeMap )
		{
			auto model = obj->m_dxMatrixBindPoseMap.find(data.first);
			if (model == obj->m_dxMatrixBindPoseMap.end())
			{
				continue;
			}
			TMatrix matBindPose = model->second;
			int iIndex = data.second;
			m_matMeshBoneArray.matBoneWorld[iIndex] = matBindPose *
				m_matBoneArray.matBoneWorld[iIndex];	

			// 이전 메쉬의 전치 행렬을 다음 메쉬에서 사용하는 것이 문제였다.
			D3DXMatrixTranspose(&m_matMeshBoneArray.matBoneWorld[iIndex],
				&m_matMeshBoneArray.matBoneWorld[iIndex]);
		}				

		m_pModel->m_pImmediateContext->UpdateSubresource(m_pBoneCB, 0, NULL,
			&m_matMeshBoneArray, 0, 0);

		m_pModel->m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pBoneCB);

		obj->SetMatrix(&m_matControl,
			&ICore::g_pMainCamera->m_matView,
			&ICore::g_pMainCamera->m_matProj);

		obj->PreRender();
		
		UINT StartSlot;
		UINT NumBuffers;
		UINT Strides[2] = { sizeof(PNCT_Vertex), sizeof(TVertexIW) };
		UINT Offsets[2] = { 0, };

		ID3D11Buffer* buffer[2] = { obj->m_pVertexBuffer, obj->m_pVBWeightList };
		obj->m_pImmediateContext->IASetVertexBuffers(0, 2, buffer, Strides, Offsets);
		
		for (int isubMtrl = 0; isubMtrl < obj->m_TriangleList.size(); isubMtrl++)
		{
			if (obj->m_TexArray.size() && obj->m_TexArray[isubMtrl])
			{
				obj->m_TexArray[isubMtrl]->Apply(obj->m_pImmediateContext, 0);
			}
			if (obj->m_pIndexBuffer == nullptr)
			{
				obj->m_pImmediateContext->Draw(obj->m_TriangleList[isubMtrl].size(),
												obj->m_TriangleOffsetList[isubMtrl]);
			}
		}
	}
	return true;
}
bool  TMapObjSkinning::Frame()
{
	m_fCurrentAnimTime += m_pModel->GetFrameSpeed() * g_fSecondPerFrame * 1.0f;
	if (m_fCurrentAnimTime >= m_pModel->GetEndFrame())
	{
		m_fCurrentAnimTime = m_pModel->GetStartFrame();
	}
	//  0   b0 ~ b4
	//  1   b0 ~ b4
	// 스킨(메쉬)와 바인드포즈(에니메이션행렬)의 노드개수가 다른 수 있다.
	TBoneWorld matAnimation;
	for (int inode = 0; inode < m_pModel->m_TreeList.size(); inode++)
	{
		TFbxObj* pFbxNode = m_pModel->m_TreeList[inode].get();
		m_matBoneArray.matBoneWorld[inode] = m_pModel->m_MatrixArray[m_fCurrentAnimTime][inode];
	}
	return true;
}
bool	TMapObjSkinning::CreateBoneBuffer()
{
	if (m_pModel == nullptr) return true;
	HRESULT hr;
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TBoneWorld);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_matBoneArray.matBoneWorld;

	if (FAILED(hr = m_pModel->m_pDevice->CreateBuffer(&bd, NULL, &m_pBoneCB)))
	{
		return false;
	}
	return true;
}

void TMapObjSkinning::AllNodeRender()
{
	for (int inode = 0; inode < m_pModel->m_TreeList.size(); inode++)
	{
		TFbxObj* pFbxNode = m_pModel->m_TreeList[inode].get();
		if (pFbxNode->m_pTex == nullptr) continue;
		// pFbxObj->m_matBoneArray.matBoneWorld[inode] = InvBondMatrix * AnimationMatrix[time];
		m_matBoneArray.matBoneWorld[inode] = m_pModel->m_MatrixArray[m_fCurrentAnimTime][inode];
		D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[inode],
			&m_matBoneArray.matBoneWorld[inode]);
				pFbxNode->SetMatrix(&m_matBoneArray.matBoneWorld[inode],
				&ICore::g_pMainCamera->m_matView,
				&ICore::g_pMainCamera->m_matProj);

		pFbxNode->PreRender();
		pFbxNode->PostRender();
	}
}


//TBasisFBX::TMatrix TMapObj::Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fTime)
//{
//	TBasisFBX::TMatrix matAnim;
//	if (pModel->m_AnimTrack.size() <= 0)
//	{
//		return matAnim;
//	}
//	TScene tScene = pAnimImp->m_Scene;
//	int iStart = max(tScene.iStart, fTime);
//	int iEnd = min(tScene.iEnd, fTime + 1);
//	// 보간 = A ~ 7.5f ~ B
//	//       9.5f <=10   ~     20 -> 20.1
//	TTrack A = pModel->m_AnimTrack[iStart];
//	TTrack B = pModel->m_AnimTrack[iEnd];
//	float s = fTime - (float)iStart; // 0~1
//	TBasisFBX::TVector3 pos;
//	TBasisFBX::D3DXVec3Lerp(&pos, &A.t, &B.t, s);
//	TBasisFBX::TVector3 scale;
//	TBasisFBX::D3DXVec3Lerp(&scale, &A.s, &B.s, s);
//	TBasisFBX::TQuaternion rotation;
//	TBasisFBX::D3DXQuaternionSlerp(&rotation, &A.r, &B.r, s);
//	TBasisFBX::TMatrix matScale;
//	TBasisFBX::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
//	TBasisFBX::TMatrix matRotation;
//	TBasisFBX::D3DXMatrixRotationQuaternion(&matRotation, &rotation);
//
//	matAnim = matScale * matRotation;
//	matAnim._41 = pos.x;
//	matAnim._42 = pos.y;
//	matAnim._43 = pos.z;
//	//TBasisFBX::TMatrix matTrans;
//	//TBasisFBX::D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
//	//matAnim = pModel->m_AnimTrack[(int)fTime].matTrack;
//	return matAnim;
//}

