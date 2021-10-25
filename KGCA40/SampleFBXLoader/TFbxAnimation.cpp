#include "TFbxObj.h"
#include "TTimer.h"
#include <algorithm>
bool Compare(const pair<int, int>& a, const pair<int, int>& b)
{
	if (a.first == b.first)
		return a.second < b.second;
	return a.first < b.first;
}

int  TFbxObj::GetFindInedx(FbxNode* pNode)
{
	for (int iNode = 0; iNode < m_pFbxNodeList.size(); iNode++)
	{
		if (m_pFbxNodeList[iNode] == pNode)
		{
			return iNode;
		}
	}
	return -1;
}
bool TFbxObj::ParseMeshSkinning(FbxMesh* pFbxMesh, TMesh* pMesh, TSkinData* pSkindata)
{
	int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iNumDeformer == 0)
	{
		return false;
	}
	int iNumVertexCount = pFbxMesh->GetControlPointsCount();
	pSkindata->m_VertexList.resize(iNumVertexCount);

	for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
	{
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		int iNumCluster = pSkin->GetClusterCount();
		// 영향을 미치는 행렬이 iNumCluster 있다.
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);

			FbxAMatrix matXBindPose;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			FbxAMatrix matInitPostion;
			pCluster->GetTransformMatrix(matInitPostion);
			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() *
				matXBindPose;
			TMatrix matBinePos = 
				DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
			// 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
			int iNumVertex = pCluster->GetControlPointIndicesCount();
			
			FbxNode* pLinkNode = pCluster->GetLink();
			pSkindata->m_MatrixList.push_back(pLinkNode);
			int iBone = GetFindInedx(pLinkNode);
			_ASSERT(iBone>=0);
			pMesh->m_iBoneList.push_back(iBone);
			D3DXMatrixInverse(&matBinePos, NULL, &matBinePos);
			m_matBindPoseList[iBone] = matBinePos;

			int iMatrixIndex = pSkindata->m_MatrixList.size() - 1;
			//ControlPoint(제어점) 정점리스트
			int* iIndex = pCluster->GetControlPointIndices();
			// 가중치리스트
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iMatrixIndex);
				pSkindata->m_VertexList[iIndex[i]].m_WegihtList.push_back(pWeight[i]);
				//iIndex[i] 정점은  iMatrixIndex행렬이 pWeight[i]=1 가중치로 영향을 미친다.				
			}
		}
	}
	return true;
}
void    TFbxObj::ParseAnimStack(FbxString* szData)
{
	// Frame, Tick
	// 1Frame = 160Tick
	// frameSpeed = 1Sec Per 30Frame
	// 1SecTick = 30 * 160 = 4800Tick
	// 0 ~ 100Frame(16000/4800) = 3.3333Sec
	FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;
	
	if (pTakeInfo)
	{
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}
void	TFbxObj::ParseAnimation()
{
	FbxArray<FbxString*> AnimStackNameArray;
	m_pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimStackCount; iStack++)
	{
		ParseAnimStack(AnimStackNameArray.GetAt(iStack));
	}
}
void	TFbxObj::ParseAnimationNode(	FbxNode* pNode, 
										TMesh* pMesh)
{
	// 에니메이션 데이터 저장
	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{		
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);	
		for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
		{
			TMesh* pMesh = m_pMeshList[iMesh];
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			TMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
		}
		fCurrentTime += m_fSampleTime;
	}	
	/*for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		float fCurrentTime = m_fStartTime;
		TMesh* pMesh = m_pMeshList[iMesh];
		while (fCurrentTime < m_fEndTime)
		{
			FbxTime time;
			time.SetSecondDouble(fCurrentTime);
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			TMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
			fCurrentTime += m_fSampleTime;
		}
	}*/
}