#include "TFbxObj.h"

//https://www.gamedev.net/articles/programming/graphics/how-to-work-with-fbx-sdk-r3582
FbxManager* TFbxObj::g_pSDKManager = nullptr;
void TFbxObj::TransformMatrix(XMFLOAT4X4* pDestMatrix, const XMFLOAT4X4* pSrcMatrix)
{
	XMFLOAT4X4 SrcMatrix;
	if (pSrcMatrix == pDestMatrix)
	{
		memcpy(&SrcMatrix, pSrcMatrix, sizeof(XMFLOAT4X4));
		pSrcMatrix = &SrcMatrix;
	}
	memcpy(pDestMatrix, pSrcMatrix, sizeof(XMFLOAT4X4));
}

void TFbxObj::AddKey(AnimationScanNode& asn, const AnimationScanNode* pParent, FbxAMatrix& matFBXGlobal, float fTime)
{
	XMFLOAT4X4 matGlobal = (XMFLOAT4X4)DxConvertMatrix(ConvertMatrixA(matFBXGlobal));
	asn.matGlobal = matGlobal;
	XMFLOAT4X4 matLocal = matGlobal;
	if (pParent)
	{
		XMMATRIX m = XMLoadFloat4x4(&pParent->matGlobal);
		XMMATRIX matInvParentGlobal = XMMatrixInverse(nullptr, m);

		m = XMLoadFloat4x4(&matGlobal);
		m = XMMatrixMultiply(m, matInvParentGlobal);

		XMStoreFloat4x4(&matLocal, m);
	}

	XMMATRIX matLocalFinal;
	TransformMatrix(reinterpret_cast<XMFLOAT4X4*>(&matLocalFinal), &matLocal);

	XMVECTOR vScale;
	XMVECTOR qRotation;
	XMVECTOR vTranslation;
	XMMatrixDecompose(&vScale, &qRotation, &vTranslation, matLocalFinal);

	XMFLOAT3 scale;
	XMStoreFloat3(&scale, vScale);

	XMFLOAT4 rot;
	XMStoreFloat4(&rot, qRotation);

	XMFLOAT3 trans;
	XMStoreFloat3(&trans, vTranslation);

	asn.pTrack->TransformTrack.AddKey(fTime, trans, rot, scale);
}
void TFbxObj::ParseNodeA(FbxNode* pNode, ScanList& scanlist, DWORD dwFlags, INT iParentIndex, bool bIncludeNode)
{
	INT iCurrentIndex = iParentIndex;
	const CHAR* strNodeName = pNode->GetName();

	iCurrentIndex = static_cast<INT>(scanlist.size());

	// add node to anim list
	AnimationScanNode asn = {};
	asn.iParentIndex = iParentIndex;
	asn.pNode = pNode;
	asn.dwFlags = dwFlags;
	scanlist.push_back(asn);

	DWORD dwChildCount = pNode->GetChildCount();
	for (DWORD i = 0; i < dwChildCount; ++i)
	{
		ParseNodeA(pNode->GetChild(i), scanlist, dwFlags, iCurrentIndex, bIncludeNode);
	}
}

void TFbxObj::CaptureAnimation(ScanList& scanlist, ExportAnimation* pAnim, FbxScene* pFbxScene)
{
	const float fDeltaTime = pAnim->fSourceSamplingInterval;
	const float fStartTime = pAnim->fStartTime;
	const float fEndTime = pAnim->fEndTime;
	float fCurrentTime = fStartTime;

	size_t dwNodeCount = scanlist.size();

	while (fCurrentTime <= fEndTime)
	{
		FbxTime CurrentTime;
		CurrentTime.SetSecondDouble(fCurrentTime);
		for (size_t i = 0; i < dwNodeCount; ++i)
		{
			AnimationScanNode& asn = scanlist[i];

#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
			auto pAnimEvaluator = pFbxScene->GetAnimationEvaluator();
#else
			auto pAnimEvaluator = pFbxScene->GetEvaluator();
#endif

			auto matGlobal = pAnimEvaluator->GetNodeGlobalTransform(asn.pNode, CurrentTime);
			AnimationScanNode* pParent = nullptr;
			if (asn.iParentIndex >= 0)
				pParent = &scanlist[asn.iParentIndex];

			AddKey(asn, pParent, matGlobal, fCurrentTime - fStartTime);
		}
		fCurrentTime += fDeltaTime;
	}
}
HRESULT TFbxObj::Initialize(const char* pFileName)
{
	if (!g_pSDKManager)
	{
		g_pSDKManager = FbxManager::Create();
		if (!g_pSDKManager)
			return E_FAIL;

		auto ios = FbxIOSettings::Create(g_pSDKManager, IOSROOT);
		if (!ios)
			return E_FAIL;

		g_pSDKManager->SetIOSettings(ios);
	}

	if (!m_pFbxImporter)
	{
		m_pFbxImporter = FbxImporter::Create(g_pSDKManager, "");
		if (!m_pFbxImporter)
			return E_FAIL;
	}

	m_pFBXScene = FbxScene::Create(g_pSDKManager, "");
	if (!m_pFBXScene)
		return E_FAIL;

	assert(g_pSDKManager != nullptr);
	assert(m_pFbxImporter != nullptr);
	assert(m_pFBXScene != nullptr);

	INT iMajorVersion, iMinorVersion, iRevision;
	g_pSDKManager->GetFileFormatVersion(iMajorVersion, iMinorVersion, iRevision);
	INT iFileFormat = -1;
	bool bResult = m_pFbxImporter->Initialize(pFileName, iFileFormat, g_pSDKManager->GetIOSettings());

	if (!bResult)
	{
		return E_FAIL;
	}

	bResult = m_pFbxImporter->Import(m_pFBXScene);

	if (!bResult)
	{
		return E_FAIL;
	}

	m_SceneAxisSystem = m_pFBXScene->GetGlobalSettings().GetAxisSystem();
	// maxY Axis -> Z Axis, maxZ Axis -> -Axis, maxX Axis - > X Axis로 변환
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFBXScene);
	////// up = +y, front = +z, right = +x
	//FbxAxisSystem::Max.ConvertScene(m_pFBXScene);
	/*FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	if (m_SceneAxisSystem != OurAxisSystem)
	{
		OurAxisSystem.ConvertScene(m_pFBXScene);
	}*/

	//////// MAX에서는 UpVector가 Z축이며, Maya는 Y축이다.
	//////// FBX는 오른손 좌표계로 Y축을 상향벡터로 항상 사용한다. 우리는 Y과 Z축을 교환하여 처리 할 것이다.
	////////const FbxAxisSystem  axis= FbxAxisSystem::OpenGL;
	///////*FbxAxisSystem OurAxisSystem = FbxAxisSystem::DirectX;
	//////if (m_SceneAxisSystem != OurAxisSystem)
	//////{
	//////	FbxAxisSystem::DirectX.ConvertScene(m_pFBXScene);
	//////}*/

	m_SceneSystemUnit = m_pFBXScene->GetGlobalSettings().GetSystemUnit();
	////if (m_SceneSystemUnit.GetScaleFactor() != 1.0f)
	////{
	////	FbxSystemUnit::cm.ConvertScene(m_pFBXScene);
	////}
	/////////////////////////////////////////////////////////////////////
	// ** 속도 저하로 주석처리함.
	// Convert mesh, NURBS and patch into triangle mesh
	//FbxGeometryConverter lGeomConverter(g_pSDKManager);
	//lGeomConverter.Triangulate(m_pFBXScene, /*replace*/true);
	////////////////////////////////////////////////////////////////////
	return S_OK;
}


void TFbxObj::ParseAnimation(FbxScene* pFbxScene)
{
	assert(pFbxScene != nullptr);

	FbxArray<FbxString*> AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);

	DWORD dwAnimStackCount = static_cast<DWORD>(AnimStackNameArray.GetCount());
	for (DWORD i = 0; i < dwAnimStackCount; ++i)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
	}
}

void TFbxObj::AddAnimation(ExportAnimation* pAnim)
{
	m_pNodeAnimList.push_back(pAnim);
}

void TFbxObj::ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName)
{
	auto curAnimStack = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (!curAnimStack)
		return;

#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	pFbxScene->GetAnimationEvaluator()->Reset();
#else
	pFbxScene->GetEvaluator()->SetContext(curAnimStack);
#endif

	auto pTakeInfo = pFbxScene->GetTakeInfo(*strAnimStackName);
	
	m_Scanlist.clear();

	auto pAnim = new ExportAnimation();
	pAnim->SetName(strAnimStackName->Buffer());
	pAnim->SetDCCObject(pTakeInfo);

	AddAnimation(pAnim);

	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());

	// 1프레임 시간 계산
	float fFrameTime = (float)FrameTime.GetSecondDouble();
	// 샘플링 구간( 기본= 0.03f )
	float fFrameStep = 1.0f; // 1프레임 간격
	float fSampleTime = fFrameTime * fFrameStep;// (float)g_pScene->Settings().iAnimSampleCountPerFrame;
	assert(fSampleTime > 0);

	float fStartTime, fEndTime;
	if (pTakeInfo)
	{
		fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);

		fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();

	}

	pAnim->fStartTime = fStartTime;
	pAnim->fEndTime = fEndTime;
	pAnim->fSourceSamplingInterval = fSampleTime;

	m_Scene.iFrameSpeed = 30;
	m_Scene.iTickPerFrame = 160;
	m_Scene.iFirstFrame = pAnim->fStartTime*30.0f;
	m_Scene.iLastFrame = pAnim->fEndTime*30.0f;

	bool bIncludeAllNodes = false;
	
	ParseNodeA(pFbxScene->GetRootNode(), m_Scanlist, 0, -1, bIncludeAllNodes);

	size_t dwTrackCount = m_Scanlist.size();
	for (size_t i = 0; i < dwTrackCount; ++i)
	{
		const CHAR* strTrackName = m_Scanlist[i].pNode->GetName();
		auto pTrack = new ExportAnimationTrack();
		pTrack->SetName(strTrackName);
		pAnim->AddTrack(pTrack);
		m_Scanlist[i].pTrack = pTrack;
	}

	CaptureAnimation(m_Scanlist, pAnim, pFbxScene);
	pAnim->Optimize();
}