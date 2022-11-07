#include "TFbxFile.h"
void TFbxFile::InitAnimation()
{
	FbxAnimStack* stackAnim = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	FbxLongLong s = 0;
	FbxLongLong n = 0;
	FbxTime::EMode TimeMode;
	if (stackAnim)
	{
		FbxString takeName = stackAnim->GetName();
		FbxTakeInfo* take = m_pFbxScene->GetTakeInfo(takeName);
		FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
		TimeMode = FbxTime::GetGlobalTimeMode();
		FbxTimeSpan localTimeSpan = take->mLocalTimeSpan;
		FbxTime start = localTimeSpan.GetStart();
		FbxTime end = localTimeSpan.GetStop();
		FbxTime Duration = localTimeSpan.GetDirection();
		s = start.GetFrameCount(TimeMode);
		n = end.GetFrameCount(TimeMode);
	}
	m_AnimScene.iStartFrame = s;
	m_AnimScene.iEndFrame = n;
	m_AnimScene.fFrameSpeed = 30.0f;
	m_AnimScene.fTickPerFrame = 160;
	m_AnimScene.TimeMode = TimeMode;
}
void TFbxFile::LoadAnimation(TFbxObjectSkinning* pObj)
{
	FbxNode* pNode = pObj->m_pFbxNode;	
	FbxTime time;
	for (FbxLongLong t = m_AnimScene.iStartFrame; t <= m_AnimScene.iEndFrame; t++)
	{
		time.SetFrame(t, m_AnimScene.TimeMode);
		TAnimTrack track;
		track.iFrame = t;
		FbxAMatrix fbxMatrix = pNode->EvaluateGlobalTransform(time);
		track.matAnim = DxConvertMatrix(fbxMatrix);
		TBASIS_EX::D3DXMatrixDecompose(&track.s, &track.r, &track.t, &track.matAnim);
		pObj->m_AnimTracks.push_back(track);
	}
}
TMatrix TFbxFile::ConvertMatrix(FbxAMatrix& fbxMatrix)
{
	TMatrix mat;
	float* tArray = (float*)(&mat);
	double* fbxArray = (double*)(&fbxMatrix);
	for (int i = 0; i < 16; i++)
	{
		tArray[i] = fbxArray[i];
	}
	return mat;
}
TMatrix TFbxFile::DxConvertMatrix(FbxAMatrix& fbxMatrix)
{
	TMatrix m = ConvertMatrix(fbxMatrix);
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}