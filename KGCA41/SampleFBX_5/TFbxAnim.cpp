#include "TFbxImport.h"
void		TFbxImport::GetAnimation()
{
	// 에니메니션 정보 얻기
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	if (stack == nullptr) return;
	FbxString TakeName = stack->GetName();
	FbxTakeInfo* takeinfo = m_pFbxScene->GetTakeInfo(TakeName);
	FbxTimeSpan ts = takeinfo->mLocalTimeSpan;
	FbxTime startTime = ts.GetStart();
	FbxTime endTime = ts.GetStop();

	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong startFrame = startTime.GetFrameCount(TimeMode);
	FbxLongLong endFrame = endTime.GetFrameCount(TimeMode);

	m_iStartFrame = startFrame;
	m_iEndFrame = endFrame;
	m_iFrameSpeed = 30;
	m_iTickForFrame = 160;

	FbxTime time;
	for (int t = startFrame; t < endFrame; t++)
	{
		time.SetFrame(t, TimeMode);
		for (int iNode = 0; iNode < m_pNodeMeshList.size(); iNode++)
		{
			TFbxMesh& tMesh = m_tMeshList[iNode];
			std::vector< FbxAMatrix>  frameMatrix;

			FbxAMatrix matFbx = m_pNodeMeshList[iNode]->EvaluateGlobalTransform(time);
			TMatrix mat = ConvertAMatrix(matFbx);
			mat = DxConvertMatrix(mat);
			tMesh.m_MatrixArray.push_back(mat);
		}
	}
}