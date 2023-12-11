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

	// frame 0 :  bone0 ~~~~~ bone60
	// frame 1 :  bone0 ~~~~~ bone60
	// frame 2 :  bone0 ~~~~~ bone60
	// frame 3 :  bone0 ~~~~~ bone60
	// frame 4 :  bone0 ~~~~~ bone60
	FbxTime time;
	for (int t = startFrame; t < endFrame; t++)
	{
		time.SetFrame(t, TimeMode);
		std::vector<TMatrix> matArray;
		for (int iNode = 0; iNode < fbxObj->m_TreeList.size(); iNode++)
		{
			FbxAMatrix matFbx = m_pFbxNodeList[iNode]->EvaluateGlobalTransform(time);
			TMatrix mat = ConvertAMatrix(matFbx);
			mat = DxConvertMatrix(mat);
			matArray.push_back(mat);			
		}
		fbxObj->m_MatrixArray.push_back(matArray);
	}
}