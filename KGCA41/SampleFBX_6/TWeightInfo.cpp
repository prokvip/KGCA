#include "TFbxImport.h"
bool	TFbxImport::ParseMeshSkinning(FbxMesh* pFbxMesh, TFbxObj* pObject)
{
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	// ������ ������ �����Ѵ�.
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	pObject->m_WeightList.resize(iVertexCount);

	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		// dwClusterCount�� ����� ��ü ������ ������ �־���.
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			////
			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			int  iBoneIndex = 0;
			TMatrix matInvBindPos = ConvertMatrix(matBindPose);
			matInvBindPos = DxConvertMatrix(matInvBindPos);
			matInvBindPos = matInvBindPos.Invert();

			iBoneIndex = m_pFbxNodeMap.find(pCluster->GetLink())->second;
			std::wstring name = mtw(pCluster->GetLink()->GetName());// m_TreeList[iBoneIndex]->m_csName;
			pObject->m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();
			// ������ �޴� �������� �ε���
			int* pVertexIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			// iBoneIndex�� ������ �޴� �������� dwClusterSize�� �ִ�.
			for (int i = 0; i < dwClusterSize; i++)
			{
				// n�� ����(pIndices[i])�� iBoneIndex�� ��Ŀ� 
				// pWeights[i]�� ����ġ�� ����Ǿ���.
				int iVertexIndex = pVertexIndices[i];
				float fWeight = pWeights[i];
				pObject->m_WeightList[iVertexIndex].InsertWeight(iBoneIndex, fWeight);
			}
		}
	}
	return true;
}