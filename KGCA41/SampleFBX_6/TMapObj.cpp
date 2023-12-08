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
		obj->PostRender();
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
////bool	TMapObj::CreateInputLayout()
////{
////	//TDxObject::CreateInputLayout();
////
////	// 정점쉐이더의 결과를 통해서 정점레이아웃을 생성한다.	
////	// 정점버퍼의 각 정점의 어떤 성분을 정점쉐이더에 전달할 거냐
////	D3D11_INPUT_ELEMENT_DESC layout[] =
////	{
////		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////		{"COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////
////		{"INDEX",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////		{"WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////		{"TANGENT",0, DXGI_FORMAT_R32G32B32_FLOAT, 1,32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
////	};
////
////	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
////	HRESULT hr = m_pDevice->CreateInputLayout(
////		layout,
////		NumElements,
////		m_pShader->GetBufferPointer(),
////		m_pShader->GetBufferSize(),
////		&m_pVertexLayout);
////	if (FAILED(hr))
////	{
////		return false;
////	}
////	return true;
////}
////void     TMapObj::LoadTextureArray(W_STR filename)
////{
////	m_TexArray.push_back(I_Tex.Load(filename));
////}
//////bool  TMapObj::Render()
//////{
//////	m_pImmediateContext->UpdateSubresource(m_pModel->m_pBoneCB, 0, NULL,
//////		&m_pModel->m_matBoneArray, 0, 0);
//////	m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pModel->m_pBoneCB);
//////	return true;
//////}
////bool  TMapObj::PreRender()
////{
////	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
////
////	if (m_pTex)
////	{
////		m_pTex->Apply(m_pImmediateContext, 0);
////	}
////	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
////	if (m_pShader)
////	{
////		m_pShader->Apply(m_pImmediateContext, 0);
////	}
////
////	//UINT stride = sizeof(PNCT_Vertex);
////	//UINT offset = 0;
////	//m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
////	m_pImmediateContext->IASetPrimitiveTopology(
////		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
////	return true;
////}
/////*bool   TMapObj::PostRender()
////{
////	for (int isubMtrl = 0; isubMtrl < m_ptMesh->m_TriangleList.size(); isubMtrl++)
////	{
////		if (m_TexArray.size() && m_TexArray[isubMtrl])
////		{
////			m_TexArray[isubMtrl]->Apply(m_pImmediateContext, 0);
////		}
////		if (m_pIndexBuffer == nullptr)
////		{
////			m_pImmediateContext->Draw(m_ptMesh->m_TriangleList[isubMtrl].size(),
////									  m_ptMesh->m_TriangleOffsetList[isubMtrl]);
////		}
////	}
////	return true;
////}*/
////bool	TMapObj::PostRender()
////{
////	UINT StartSlot;
////	UINT NumBuffers;
////	UINT Strides[2] = { sizeof(PNCT_Vertex), sizeof(TVertexIW) };
////	UINT Offsets[2] = { 0, };
////
////	ID3D11Buffer* buffer[2] = { m_pVertexBuffer, m_pVBWeightList };
////	m_pImmediateContext->IASetVertexBuffers(0, 2, buffer, Strides, Offsets);
////
////	//m_pImmediateContext->UpdateSubresource(m_pModel->m_pBoneCB, 0, NULL, 
////	//	&m_pModel->m_matBoneArray, 0, 0);
////	//m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pModel->m_pBoneCB);
////
////	for (int iMtrl = 0; iMtrl < m_ptMesh->m_TriangleList.size(); iMtrl++)
////	{
////		if (m_TexArray.size() && m_TexArray[iMtrl])
////		{
////			m_TexArray[iMtrl]->Apply(m_pImmediateContext, 0);
////		}
////		if (m_IndexList.size() <= 0)
////			m_pImmediateContext->Draw(m_ptMesh->m_TriangleList[iMtrl].size(),
////				m_ptMesh->m_TriangleOffsetList[iMtrl]);
////		else
////			m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
////	}
////	return true;
////}

////bool	TMapObj::Frame(float fSecPerFrame, float fGameTimer)
////{
////	TObject3D::Frame(fSecPerFrame, fGameTimer);
////
////	TFbxImporter* pAnimImp = nullptr;
////	if (m_pAnimImporter != nullptr)
////	{
////		pAnimImp = m_pAnimImporter;
////	}
////	else
////	{
////		pAnimImp = m_pMeshImp;
////	}
////	if (pAnimImp)
////	{
////		m_fTime += fSecPerFrame * pAnimImp->m_Scene.iFrameSpeed * m_fDir * 0.33f;// m_fSpeed;
////		if (m_fTime >= pAnimImp->m_Scene.iEnd)
////		{
////			//m_fDir *= -1.0f;
////			m_fTime = pAnimImp->m_Scene.iStart;
////		}
////
////		//int iFrame = m_fTime;
////		//iFrame = max(0, min(pAnimImp->m_Scene.iEnd - 1, iFrame));
////
////		// 스킨(메쉬)와 바인드포즈(에니메이션행렬)의 노드개수가 다른 수 있다.
////		TBoneWorld matAnimation;
////		for (int inode = 0; inode < m_pMeshImp->m_TreeList.size(); inode++)
////		{
////			TFbxModel* pMeshModel = m_pMeshImp->m_TreeList[inode];
////			auto model = pAnimImp->m_pFbxModelMap.find(pMeshModel->m_csName);
////			if (model == pAnimImp->m_pFbxModelMap.end())
////			{
////				continue;
////			}
////			TFbxModel* pAnimModel = model->second;
////			matAnimation.matBoneWorld[inode] = Interplate(pAnimImp, pAnimModel, m_fTime);
////		}
////
////		for (int iObj = 0; iObj < m_pMeshImp->m_DrawList.size(); iObj++)
////		{
////			TFbxModel* pFbxObj = m_pMeshImp->m_DrawList[iObj];
////			if (_tcsstr(pFbxObj->m_csName.c_str(), L"LOD") != nullptr)// != L"SK_Mannequin_LOD0")
////			{
////				if (_tcsstr(pFbxObj->m_csName.c_str(), L"LOD0") == nullptr)// != L"SK_Mannequin_LOD0")
////				{
////					continue;
////				}
////			}
////
////			if (pFbxObj->m_bSkinned)
////			{
////				for (auto data : pFbxObj->m_dxMatrixBindPoseMap)
////				{
////					std::wstring name = data.first;
////					TMatrix matBindPose = data.second;
////					auto model = m_pMeshImp->m_pFbxModelMap.find(name);
////					if (model == m_pMeshImp->m_pFbxModelMap.end())
////					{
////						continue;
////					}
////					int iIndex = model->second->m_iBoneIndex;
////					pFbxObj->m_matBoneArray.matBoneWorld[iIndex] = matBindPose * matAnimation.matBoneWorld[iIndex];
////					TBasisFBX::D3DXMatrixTranspose(&pFbxObj->m_matBoneArray.matBoneWorld[iIndex],
////						&pFbxObj->m_matBoneArray.matBoneWorld[iIndex]);
////				}
////			}
////			else
////			{
////				for (int inode = 0; inode < m_pMeshImp->m_TreeList.size(); inode++)
////				{
////					TBasisFBX::D3DXMatrixTranspose(&pFbxObj->m_matBoneArray.matBoneWorld[inode],
////						&matAnimation.matBoneWorld[inode]);
////				}
////			}
////		}
////	}
////	return true;
////}
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


bool TMapObjSkinning::Render()
{	
	
	TMatrix matWorld;
	auto tFbxMeshList = m_pModel->m_DrawList;
	for (int iSub = 0; iSub < tFbxMeshList.size(); iSub++)
	{
		TFbxObj* obj = tFbxMeshList[iSub].get();

		//// 메쉬단위로 영향 행렬이 다르다. 50개
		for (auto data : m_pModel->m_pFbxNodeMap )
		{
			auto model = obj->m_dxMatrixBindPoseMap.find(data.first);
			if (model == obj->m_dxMatrixBindPoseMap.end())
			{
				continue;
			}
			TMatrix matBindPose = model->second;
			int iIndex = data.second;
			m_matBoneArray.matBoneWorld[iIndex] = matBindPose *
				m_matBoneArray.matBoneWorld[iIndex];
			D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[iIndex],
				&m_matBoneArray.matBoneWorld[iIndex]);
		}		
		m_pModel->m_pImmediateContext->UpdateSubresource(m_pBoneCB, 0, NULL,
			&m_matBoneArray, 0, 0);

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
		
		obj->PostRender();
	}
	return true;
}
bool  TMapObjSkinning::Frame()
{
	m_fCurrentAnimTime += m_pModel->GetFrameSpeed() * g_fSecondPerFrame *0.0f;
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