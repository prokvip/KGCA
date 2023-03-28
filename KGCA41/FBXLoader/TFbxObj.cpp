#include "TFbxObj.h"

#define MAX_BONE_MATRICES 255
//void TFbxObj::SetBoneFrameMatrices(ID3D11DeviceContext*    pContext, TMesh* pMesh)
//{
//	TMatrix* pMatrices;
//	HRESULT hr = S_OK;
//	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
//	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
//	{
//		pMatrices = (TMatrix*)MappedFaceDest.pData;
//		for (int dwObject = 0; dwObject < m_Scene.iNumMesh; dwObject++)
//		{
//			for (int iFrame = 0; iFrame < m_Scene.iLastFrame; iFrame++)
//			{
//				int iIndex = dwObject * m_Scene.iLastFrame + iFrame;
//				/*pMatrices[dwObject] = 
//					pMesh->m_matBindPoseMap[pMesh->m_InfluenceNames[dwObject]] * 
//					pMesh->m_pMatrixList[dwObject];*/
//
//				pMatrices[iIndex] = m_pBoneFrameMatrix[dwObject][iFrame];
//			}
//		}
//		pContext->Unmap(m_pBoneFrameBuffer.Get(), 0);
//	}
//}
//bool TFbxObj::SetBoneFrameBuffer(ID3D11Device* pd3dDevice)
//{
//	int iFrame = m_Scene.iLastFrame;
//	D3D11_BUFFER_DESC vbdesc =
//	{
//		MAX_BONE_MATRICES * sizeof(TMatrix) * iFrame,
//		D3D11_USAGE_DYNAMIC,
//		D3D11_BIND_SHADER_RESOURCE,
//		D3D11_CPU_ACCESS_WRITE,
//		0
//	};
//	pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pBoneFrameBuffer.GetAddressOf());
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
//	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
//	SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//	SRVDesc.Buffer.ElementOffset = 0;
//	SRVDesc.Buffer.ElementWidth = MAX_BONE_MATRICES * 4* iFrame;
//	pd3dDevice->CreateShaderResourceView(m_pBoneFrameBuffer.Get(), &SRVDesc, 
//		m_pBoneFrameBufferRV.GetAddressOf());
//	return true;
//}
FbxAMatrix TFbxObj::GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

bool TFbxObj::UpdateBuffer()
{
	if (!CombineBuffer(m_dxobj.g_pVertexBuffer.Get(), m_dxobj.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}
bool TFbxObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
{
	size_t dstOffset = 0;
	size_t vbOffset = 0;
	size_t dstibOffset = 0;
	size_t ibOffset = 0;
	{
		for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
		{
			auto pData = m_pData[dwObject].get();
			auto ptMesh = m_pMesh[dwObject].get();
			if (ptMesh->m_pSubMesh.size() > 0)
			{
				for (DWORD dwSub = 0; dwSub < ptMesh->m_pSubMesh.size(); dwSub++)
				{
					auto pSubData = pData->m_pSubMesh[dwSub].get();
					auto ptSubMesh = ptMesh->m_pSubMesh[dwSub].get();
					if (pSubData->m_VertexArray.size() < 3) continue;

					ptSubMesh->m_dxobj.m_iNumVertex = pSubData->m_VertexArray.size();
					ptSubMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

					ptSubMesh->m_dxobj.m_BoxVB.left = dstOffset;
					ptSubMesh->m_dxobj.m_BoxVB.right = dstOffset + ptSubMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize;
					ptSubMesh->m_dxobj.m_BoxVB.top = 0; ptSubMesh->m_dxobj.m_BoxVB.bottom = 1;
					ptSubMesh->m_dxobj.m_BoxVB.front = 0; ptSubMesh->m_dxobj.m_BoxVB.back = 1;

					g_pImmediateContext->UpdateSubresource(
						pVB, 0,
						&ptSubMesh->m_dxobj.m_BoxVB,
						(uint8_t*)&pSubData->m_VertexArray.at(0),
						0, 0);


					/*g_pImmediateContext->CopySubresourceRegion(
					m_dxobj.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
					(void*)&pSubMesh->m_VertexArray.at(0),
					0, &pSubMesh->m_dxobj.m_BoxVB);*/

					ptSubMesh->m_dxobj.m_iBeginVB = vbOffset;
					vbOffset += ptSubMesh->m_dxobj.m_iNumVertex;
					dstOffset = ptSubMesh->m_dxobj.m_BoxVB.right;

					ptSubMesh->m_dxobj.m_iNumIndex = pSubData->m_IndexArray.size();
					ptSubMesh->m_dxobj.m_BoxIB.left = dstibOffset;
					ptSubMesh->m_dxobj.m_BoxIB.right = dstibOffset + ptSubMesh->m_dxobj.m_iNumIndex * sizeof(DWORD);
					ptSubMesh->m_dxobj.m_BoxIB.top = 0;	ptSubMesh->m_dxobj.m_BoxIB.bottom = 1;
					ptSubMesh->m_dxobj.m_BoxIB.front = 0;	ptSubMesh->m_dxobj.m_BoxIB.back = 1;

					g_pImmediateContext->UpdateSubresource(pIB, 0,
						&ptSubMesh->m_dxobj.m_BoxIB, (void*)&pSubData->m_IndexArray.at(0), 0, 0);

					ptSubMesh->m_dxobj.m_iBeginIB = ibOffset;
					ibOffset += ptSubMesh->m_dxobj.m_iNumIndex;
					dstibOffset = ptSubMesh->m_dxobj.m_BoxIB.right;

					//texture
					if (ptSubMesh->m_iDiffuseTex <= 0) continue;
					ptSubMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(ptSubMesh->m_iDiffuseTex)->m_pTextureSRV;
				}
			}
			else
			{
				if (pData->m_VertexArray.size() < 3) continue;
				ptMesh->m_dxobj.m_iNumVertex = pData->m_VertexArray.size();
				ptMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

				ptMesh->m_dxobj.m_BoxVB.left = dstOffset;
				ptMesh->m_dxobj.m_BoxVB.right = dstOffset + ptMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize;
				ptMesh->m_dxobj.m_BoxVB.top = 0; ptMesh->m_dxobj.m_BoxVB.bottom = 1;
				ptMesh->m_dxobj.m_BoxVB.front = 0; ptMesh->m_dxobj.m_BoxVB.back = 1;

				g_pImmediateContext->UpdateSubresource(pVB, 0, &ptMesh->m_dxobj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);

				ptMesh->m_dxobj.m_iBeginVB = vbOffset;
				vbOffset += ptMesh->m_dxobj.m_iNumVertex;
				dstOffset = ptMesh->m_dxobj.m_BoxVB.right;

				ptMesh->m_dxobj.m_iNumIndex = pData->m_IndexArray.size();
				ptMesh->m_dxobj.m_BoxIB.left = dstibOffset;
				ptMesh->m_dxobj.m_BoxIB.right = dstibOffset + ptMesh->m_dxobj.m_iNumIndex * sizeof(DWORD);
				ptMesh->m_dxobj.m_BoxIB.top = 0; ptMesh->m_dxobj.m_BoxIB.bottom = 1;
				ptMesh->m_dxobj.m_BoxIB.front = 0; ptMesh->m_dxobj.m_BoxIB.back = 1;

				g_pImmediateContext->UpdateSubresource(pIB, 0, &ptMesh->m_dxobj.m_BoxIB, (void*)&pData->m_IndexArray.at(0), 0, 0);
				ptMesh->m_dxobj.m_iBeginIB = ibOffset;
				ibOffset += ptMesh->m_dxobj.m_iNumIndex;
				dstibOffset = ptMesh->m_dxobj.m_BoxIB.right;

				//texture
				if (ptMesh->m_iDiffuseTex <= 0) continue;
				ptMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(ptMesh->m_iDiffuseTex)->m_pTextureSRV;
			}
		}
	}
	return true;
}
HRESULT TFbxObj::LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				TMesh* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iDiffuseTex <= 0) continue;
				pSubMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pSubMesh->m_iDiffuseTex)->m_pTextureSRV;

			}
		}
		else
		{
			if (pMesh->m_iDiffuseTex <= 0) continue;
			pMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureSRV;

		}
	}
	return hr;
}
HRESULT TFbxObj::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INDEX",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
		m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool TFbxObj::Convert(ID3D11Device* pDevice)
{
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		auto pData = m_pData[dwObject].get();

		//영향을 미치는 본 및 바이패드 갯수
		pMesh->m_pMatrixList.resize(pMesh->m_InfluenceNames.size());

		pMesh->m_iIndex = dwObject;
		pMesh->m_iNumFace = pData->m_iNumFace;
		pMesh->m_strNodeName = pMesh->m_szName;

		if (pMesh->m_pParent != NULL)
		{
			pMesh->m_strParentName = pMesh->m_pParent->m_szName;
			pMesh->m_pParent->m_pChildMesh.push_back(pMesh);
		}

		if (pMesh->m_ClassType == CLASS_GEOM)//pMesh->m_iMtrlRef
		{
			fbxMtrlList pMtrl = m_pMaterialList[pMesh->m_iMtrlRef];
			if (pMesh->m_iNumFace > 0 && pMtrl.empty() == false)
			{
				if (pMtrl.size() == 1)
				{
					TFbxMaterial* pSubMtrl = pMtrl[0];
					// 2번 인자값=-1 이면 Face Count(_countof )를 계산하지 않는다.
					pData->SetUniqueBuffer(pData->m_TriList, -1, 0);
					pMesh->m_iDiffuseTex = -1;

					if (pSubMtrl->m_Parameters.size() > 0)
					{
						wstring name;
						name = mtw(pSubMtrl->m_Parameters[0].ValueString.c_str());

						pMesh->m_iDiffuseTex = I_Texture.Add(
							g_pd3dDevice, name.c_str(), m_szDirName.c_str());
					}
					m_iMaxVertex += pData->m_VertexArray.size();
					m_iMaxIndex += pData->m_IndexArray.size();
				}
				else
				{
					int iAddCount = 0;
					for (int iSub = 0; iSub < pMtrl.size(); iSub++)
					{
						auto pSubMesh = pMesh->m_pSubMesh[iSub].get();
						auto pSubData = pData->m_pSubMesh[iSub].get();

						// 2번 인자값=-1 이면 Face Count(_countof )를 계산하지 않는다.
						pSubData->SetUniqueBuffer(pSubData->m_TriList, -1, 0);
						pSubMesh->m_iDiffuseTex = -1;

						TFbxMaterial* pSubMtrl = pMtrl[iSub];
						if (pSubMtrl->m_Parameters.size() > 0)
						{
							wstring name;
							name = mtw(pSubMtrl->m_Parameters[0].ValueString.c_str());
							pSubMesh->m_iDiffuseTex = I_Texture.Add(
								g_pd3dDevice, name.c_str(), m_szDirName.c_str());
						}
						m_iMaxVertex += pSubData->m_VertexArray.size();
						m_iMaxIndex += pSubData->m_IndexArray.size();

						pSubData->m_iNumFace = pSubData->m_IndexArray.size() / 3;
						pSubMesh->m_iNumFace = pSubData->m_IndexArray.size() / 3;

						/*if (pSubData->m_VertexArray.size() > 0)
						{
						pMesh->m_pSubMesh.push_back(pSubMesh);
						pData->m_pSubMesh.push_back(pSubData);
						}*/
					}
				}
			}
		}

		// m_pNodeAnimList는 모든 에니메이션 트랙을 저장하고 있다.
		// 오브젝트의 해당 노드의 트랙을 찾아서 저장한다.

		ExportAnimationTrack* pDeskTrack = 0;
		if (m_pNodeAnimList.size() > 0)
		{
			for (int iNode = 0; iNode < m_pNodeAnimList[0]->GetTrackCount(); iNode++)
			{
				ExportAnimationTrack* pTrack = m_pNodeAnimList[0]->GetTrack(iNode);
				std::string name = m_pNodeAnimList[0]->GetTrack(iNode)->GetName();
				if (name == wtm(pMesh->m_szName))
				{
					pDeskTrack = pTrack;
					break;
				}
			}
			if (pDeskTrack == NULL)
			{
				assert(pDeskTrack);
			}
			// 에니메이션 트랙 복사		
			TAnimTrack* pPrevTrack = NULL;
			PositionKeyList posTrack = pDeskTrack->TransformTrack.PositionKeys;
			for (int iTrack = 0; iTrack < pDeskTrack->TransformTrack.GetPositionKeyCount(); iTrack++)
			{
				auto pTrack = make_shared<TAnimTrack>();
				pTrack->iTick = posTrack[iTrack].fTime*30.0f*160.0f;
				pTrack->vVector.x = posTrack[iTrack].Position.x;
				pTrack->vVector.y = posTrack[iTrack].Position.y;
				pTrack->vVector.z = posTrack[iTrack].Position.z;
				pMesh->m_pPosTrack.push_back(pTrack);
			}

			pPrevTrack = NULL;
			OrientationKeyList rotTrack = pDeskTrack->TransformTrack.OrientationKeys;
			for (int iTrack = 0; iTrack < rotTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<TAnimTrack>();
				pTrack->iTick = rotTrack[iTrack].fTime*30.0f*160.0f;
				pTrack->qRotate.x = rotTrack[iTrack].Orientation.x;
				pTrack->qRotate.y = rotTrack[iTrack].Orientation.y;
				pTrack->qRotate.z = rotTrack[iTrack].Orientation.z;
				pTrack->qRotate.w = rotTrack[iTrack].Orientation.w;
				pMesh->m_pRotTrack.push_back(pTrack);
			}

			pPrevTrack = NULL;
			ScaleKeyList sclTrack = pDeskTrack->TransformTrack.ScaleKeys;
			for (int iTrack = 0; iTrack < sclTrack.size(); iTrack++)
			{
				auto pTrack = make_shared<TAnimTrack>();
				pTrack->iTick = sclTrack[iTrack].fTime*30.0f*160.0f;
				pTrack->vVector.x = sclTrack[iTrack].Scale.x;
				pTrack->vVector.y = sclTrack[iTrack].Scale.y;
				pTrack->vVector.z = sclTrack[iTrack].Scale.z;
				pMesh->m_pSclTrack.push_back(pTrack);
			}
		}
	}
	//// 사용되지 않은 오브젝트 제외
	//for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	//{
	//	if (m_pMesh[dwObject]->m_pChildMesh.size() <= 0 &&
	//		(m_pMesh[dwObject]->m_ClassType == CLASS_BONE || m_pMesh[dwObject]->m_ClassType == CLASS_DUMMY))
	//	{
	//		m_pMesh[dwObject]->m_bUsed = false;
	//	}
	//}
	m_Scene.iNumMesh = m_pMesh.size();
	SetBuffer(pDevice);
	return true;
}
bool TFbxObj::SetBuffer(ID3D11Device* pd3dDevice)
{
	SAFE_NEW_ARRAY_CLEAR(m_pMatrix, TMatrix, m_Scene.iNumMesh);

	// Create a bone matrix buffer
	// It will be updated more than once per frame (in a typical game) so make it dynamic
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(TMatrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	pd3dDevice->CreateBuffer(&vbdesc, NULL, &m_pBoneBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	// Again, we need a resource view to use it in the shader
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.ElementOffset = 0;
	SRVDesc.Buffer.ElementWidth = MAX_BONE_MATRICES * 4;
	pd3dDevice->CreateShaderResourceView(m_pBoneBuffer, &SRVDesc, &m_pBoneBufferRV);
	return true;
}
void TFbxObj::SetBoneMatrices(ID3D11DeviceContext*    pContext, TMesh* pMesh)
{
	TMatrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	TMatrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixInverse(&matWorld, NULL, &pMesh->m_matBindPoseMap[pMesh->m_InfluenceNames[0]]);
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (TMatrix*)MappedFaceDest.pData;
		for (DWORD dwObject = 0; dwObject < pMesh->m_InfluenceNames.size(); dwObject++)
		{
			DxMatrixMap::iterator iter = pMesh->m_matBindPoseMap.find(pMesh->m_InfluenceNames[dwObject]);
			if (iter == pMesh->m_matBindPoseMap.end())
			{
				assert(0);
			}
			pMatrices[dwObject] = //pMesh->m_matXFormToWorld;// *
				pMesh->m_matBindPoseMap[pMesh->m_InfluenceNames[dwObject]] *pMesh->m_pMatrixList[dwObject];
		}
		pContext->Unmap(m_pBoneBuffer, 0);
	}
}
void TFbxObj::SetBoneMatrices(ID3D11DeviceContext*    pContext, TMesh* pMesh, TFbxObj* pAnimMesh)
{
	TMatrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (TMatrix*)MappedFaceDest.pData;
		for (DWORD dwObject = 0; dwObject < pMesh->m_InfluenceNames.size(); dwObject++)
		{
			bool bFind = false;
			for (int i = 0; i < pAnimMesh->m_pMesh.size(); i++)
			{
				auto matMesh = pAnimMesh->m_pMesh[i].get();
				if (matMesh->m_szName == pMesh->m_InfluenceNames[dwObject])
				{
					pMatrices[dwObject] = //pMesh->m_matXFormToWorld*matMesh->m_matCalculation;
						pMesh->m_matBindPoseMap[pMesh->m_InfluenceNames[dwObject]] * matMesh->m_matCalculation;

					bFind = true;
					break;
				}
			}
			if (bFind == false)
			{
				assert(0);
			}
		}
		pContext->Unmap(m_pBoneBuffer, 0);
	}
}
bool TFbxObj::Frame()
{
	m_fElapseTime += 1.0f* TBASIS::g_fSecPerFrame * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame;
	if (m_fElapseTime >= m_Scene.iLastFrame* m_Scene.iTickPerFrame)
	{
		m_fElapseTime = m_Scene.iFirstFrame * m_Scene.iTickPerFrame;
	}
	if (m_pSkeletonMesh)
	{
		m_pSkeletonMesh->Frame();
	}

	TMatrix mat;
	D3DXMatrixIdentity(&mat);

	DWORD dwNumObject = m_pData.size();
	for (DWORD dwObject = 0; dwObject < dwNumObject; dwObject++)
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_bUsed == false) continue;
		DxMatrixMap::iterator iter = m_matFrameMatrixMap.find(pMesh->m_szName);
		// lod group 등이 해당된다.
		if (iter == m_matFrameMatrixMap.end())
		{
			continue;
		}
		if (pMesh->m_pParent)
			Interpolate(pMesh, &pMesh->m_pParent->m_matCalculation, m_fElapseTime);
		else
			Interpolate(pMesh, &mat, m_fElapseTime);

		iter->second = pMesh->m_matCalculation;
	}

	for (DWORD dwObject = 0; dwObject < dwNumObject; dwObject++)
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_bUsed == false)
		{
			continue;
		}
		TMatrix matWorld;
		D3DXMatrixIdentity(&matWorld);
		for (int ifluence = 0; ifluence < pMesh->m_InfluenceNames.size(); ifluence++)
		{
			// 주의 SK_Mannequin.fbx 에서만 사용한다.
			if (pMesh->m_InfluenceNames[ifluence] == L"upperarm_twist_01_l")//, upperarm_l
			{
				pMesh->m_InfluenceNames[ifluence] = L"upperarm_l";
			}
			DxMatrixMap::iterator iter = m_matFrameMatrixMap.find(pMesh->m_InfluenceNames[ifluence]);
			if (iter != m_matFrameMatrixMap.end())
			{
				pMesh->m_pMatrixList[ifluence] = iter->second;
				//T_STR name = iter->first.c_str();
				//name += '\n';				
				////OutputDebugString(name.c_str());
				//name.clear();
				//name = pMesh->m_InfluenceNames[ifluence].c_str();
				//name += '\n';
				//OutputDebugString(name.c_str());
			}
			else
			{
				assert(iter == m_matFrameMatrixMap.end());
			}
		}
	}
	return true;
}
bool TFbxObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
bool TFbxObj::Draw(ID3D11DeviceContext*    pContext, TActor* pParent)
{
	//CStopwatch stopwatch;
	//TVector3 vLight(0,0,1);
	//D3DXVec3Normalize(&m_vLight, &m_vLight);
	m_cbData.Color = TVector4(m_vLight.x, m_vLight.y, m_vLight.z, 1);

	DWORD dwNumObject = m_pData.size();
	for (DWORD dwObject = 0; dwObject < dwNumObject; dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_ClassType == CLASS_BIPED || pMesh->m_ClassType == CLASS_BONE)
			continue;

		if (_tcsstr(pMesh->m_szName.c_str(), L"LOD") != nullptr)// != L"SK_Mannequin_LOD0")
		{
			if (_tcsstr(pMesh->m_szName.c_str(), L"LOD0") == nullptr)// != L"SK_Mannequin_LOD0")
			{
				continue;
			}
		}

		//TMatrix matWorld = m_matControlWorld;
		//D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranspose(&m_cbData.matWorld, &m_matControlWorld);
		UpdateConstantBuffer(pContext, pParent);

		if (m_pSkeletonMesh)
			SetBoneMatrices(pContext, pMesh, m_pSkeletonMesh);
		else
			SetBoneMatrices(pContext, pMesh);

		//pContext->VSSetConstantBuffers(1, 1, &m_pBoneBuffer);
		ID3D11ShaderResourceView* aRViews[1] = { m_pBoneBufferRV };
		pContext->VSSetShaderResources(1, 1, aRViews);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1) continue;
				if (pSubMesh->m_dxobj.g_pTextureSRV == nullptr)
				{
					pContext->PSSetShaderResources(0, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
				}
				else
				{
					pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
				}
				/*if (dwSub == 1)
				{
					pContext->PSSetShaderResources(1, 1, I_Texture.m_pDefaultBlackTexture->m_pTextureSRV.GetAddressOf());
				}
				else
				{
					pContext->PSSetShaderResources(1, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
				}*/
				pContext->PSSetShaderResources(1, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
				pContext->PSSetShaderResources(2, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_dxobj.m_iNumIndex,
					pSubMesh->m_dxobj.m_iBeginIB,
					pSubMesh->m_dxobj.m_iBeginVB);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;
			if (pMesh->m_dxobj.g_pTextureSRV == nullptr)
			{
				pContext->PSSetShaderResources(0, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
			}
			else
			{
				pContext->PSSetShaderResources(0, 1, pMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
			}
			
			pContext->PSSetShaderResources(1, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
			pContext->PSSetShaderResources(2, 1, I_Texture.m_pDefaultWhiteTexture->m_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_dxobj.m_iNumIndex,
				pMesh->m_dxobj.m_iBeginIB,
				pMesh->m_dxobj.m_iBeginVB);
		}
	}
	//stopwatch.Output(L"\nTTbsObj::Draw");
	return true;
}
TMatrix TFbxObj::Interpolate(TMesh* pMesh, TMatrix* matParents, float fFrameTick)
{
	// TM		= AnimMat * ParentTM;
	// AaniMat	= TM * Inverse(ParentTM)
	TQuaternion qR, qS;
	D3DXMatrixIdentity(&pMesh->m_matCalculation);
	TMatrix matAnim, matPos, matRotate, matScale;

	matRotate = pMesh->m_matWorldRotate;
	matPos = pMesh->m_matWorldTrans;
	matScale = pMesh->m_matWorldScale;

	D3DXQuaternionRotationMatrix(&qR, &matRotate);
	D3DXQuaternionRotationMatrix(&qS, &matScale);

	// fFrameTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame + CurFame;
	float fStartTick = (float)(m_Scene.iFirstFrame * m_Scene.iTickPerFrame);
	float fEndTick = 0.0f;

	TAnimTrack* pStartTrack = NULL;
	TAnimTrack* pEndTrack = NULL;
	if (pMesh->m_pRotTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pRotTrack, &pStartTrack, &pEndTrack))
		{
			qR = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXQuaternionSlerp(&qR, &qR, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qR);
	}

	pStartTrack = NULL;
	pEndTrack = NULL;

	TVector3 Trans(matPos._41, matPos._42, matPos._43);
	if (pMesh->m_pPosTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pPosTrack, &pStartTrack, &pEndTrack))
		{
			Trans = pStartTrack->vVector;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp(&Trans, &Trans, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}

		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);
	}


	pStartTrack = NULL;
	pEndTrack = NULL;

	TMatrix matScaleRot, matInvScaleRot;
	TVector3 vScale(matScale._11, matScale._22, matScale._33);
	if (pMesh->m_pSclTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pSclTrack, &pStartTrack, &pEndTrack))
		{
			vScale = pStartTrack->vVector;
			qS = pStartTrack->qRotate;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = pEndTrack->iTick;
			D3DXVec3Lerp(&vScale, &vScale, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);
		matScale = matInvScaleRot * matScale * matScaleRot;
	}

	pStartTrack = NULL;
	pEndTrack = NULL;

	float fCurAlpha, fNextAlpha, fOffSet;
	fCurAlpha = 0.0f;
	fNextAlpha = 0.0f;
	if (pMesh->m_pVisTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pVisTrack, &pStartTrack, &pEndTrack))
		{
			fCurAlpha = pStartTrack->vVector.x;
			fStartTick = pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fNextAlpha = pEndTrack->vVector.x;
			fEndTick = pEndTrack->iTick;

			fOffSet = (fFrameTick - fStartTick) / (fEndTick - fStartTick);
			fNextAlpha = (fNextAlpha - fCurAlpha)*fOffSet;
		}
		pMesh->m_fVisibility = (fCurAlpha + fNextAlpha);
	}
	else
	{
		pMesh->m_fVisibility = 1.0f;
	}

	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;
	// 최종 에미메이션 행렬을 완성한다.	
	D3DXMatrixMultiply(&pMesh->m_matCalculation, &matAnim, matParents);

	//////// 인버스 매트릭스 확인 코드.
	TVector3 v0, v1, v2, v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		TMatrix matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&pMesh->m_matCalculation, &pMesh->m_matCalculation, &matW);
	}
	return pMesh->m_matCalculation;
}
// pEndTrack 트랙이 없으면 flase 리턴( 보간할 대상이 없을 때 )
bool TFbxObj::GetAnimationTrack(float fFrame, vector<shared_ptr<TAnimTrack>> pTrackList, TAnimTrack** pStartTrack, TAnimTrack** pEndTrack)
{
	for (DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++)
	{
		TAnimTrack *pTrack = pTrackList[dwTrack].get();
		_ASSERT(pTrack);
		// fFrame 보다 큰 Tick 트랙이 있다면 이전 트랙을 넘겨 주어야 하기 때문에 break한다.
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// 현프레임보다 큰값이 없다면. 가장 시간경과가 오래된 것을 사용한다.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;
}

void TFbxObj::SetSkeletonMesh(TFbxObj* pSkeleton)
{
	m_pSkeletonMesh = pSkeleton;
}
TFbxObj::TFbxObj()
{
	m_szDirName = L"../../data/fbx/";
	m_pMatrix = NULL;
	m_pBoneBuffer = NULL;
	m_pBoneBufferRV = NULL;
	m_pSkeletonMesh = nullptr;
	m_pFbxImporter = nullptr;
}
TFbxObj::TFbxObj(TFbxObj& obj)
{
	m_pFbxImporter = nullptr;
}
TFbxObj::TFbxObj(TFbxObj&& obj)
{
	m_pFbxImporter = nullptr;
}
TFbxObj::~TFbxObj()
{
	SAFE_DELETE_ARRAY(m_pMatrix);
	SAFE_RELEASE(m_pBoneBuffer);
	SAFE_RELEASE(m_pBoneBufferRV);
}
