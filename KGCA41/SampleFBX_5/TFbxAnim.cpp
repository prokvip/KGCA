#include "TFbxImport.h"
void		TFbxImport::GetAnimation(TFbxObj* fbxObj)
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

	fbxObj->SetAnim(startFrame, endFrame, 30, 160);

	FbxTime time;
	for (int t = startFrame; t < endFrame; t++)
	{
		time.SetFrame(t, TimeMode);
		for (int iNode = 0; iNode < m_pFbxNodeMeshList.size(); iNode++)
		{
			TFbxMesh& tMesh = *(fbxObj->m_tMeshList[iNode].get());
			std::vector< FbxAMatrix>  frameMatrix;

			FbxAMatrix matFbx = m_pFbxNodeMeshList[iNode]->EvaluateGlobalTransform(time);
			TMatrix mat = ConvertAMatrix(matFbx);
			mat = DxConvertMatrix(mat);
			tMesh.m_MatrixArray.push_back(mat);
		}
	}
}