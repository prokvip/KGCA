#include "TAnimation.h"
// pEndTrack 트랙이 없으면 flase 리턴( 보간할 대상이 없을 때 )
bool TAnimation::GetAnimationTrack(float fFrame, vector<shared_ptr<TAnimTrack>>& pTrackList, TAnimTrack** pStartTrack, TAnimTrack** pEndTrack)
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
TMatrix TAnimation::Interpolate(TMesh* pMesh, TMatrix* matParents, float fFrameTick, TScene tScene)
{

	// TM		= AnimMat * ParentTM;
	// AaniMat	= TM * Inverse(ParentTM)
	TQuaternion qR, qS;
	TMatrix matAnim, matPos, matRotate, matScale, matCalculation;

	D3DXMatrixIdentity(&matCalculation);

	matRotate = pMesh->m_matWorldRotate;
	matPos = pMesh->m_matWorldTrans;
	matScale = pMesh->m_matWorldScale;

	D3DXQuaternionRotationMatrix(&qR, &matRotate);
	D3DXQuaternionRotationMatrix(&qS, &matScale);

	// fFrameTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame + CurFame;
	float fStartTick = (float)(tScene.iFirstFrame * tScene.iTickPerFrame);
	float fEndTick = 0.0f;

	TAnimTrack* pStartTrack = NULL;
	TAnimTrack* pEndTrack = NULL;
	if (pMesh->m_pRotTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pRotTrack, &pStartTrack, &pEndTrack))
		{
			qR = pStartTrack->qRotate;
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = (float)pEndTrack->iTick;
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
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = (float)pEndTrack->iTick;
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
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXVec3Lerp(&vScale, &vScale, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);
		matScale = matInvScaleRot  * matScale * matScaleRot;
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
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fNextAlpha = pEndTrack->vVector.x;
			fEndTick = (float)pEndTrack->iTick;

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
	D3DXMatrixMultiply(&matCalculation, &matAnim, matParents);

	// 인버스 매트릭스 확인 코드.
	TVector3 v0, v1, v2, v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		TMatrix matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&matCalculation, &pMesh->m_matCalculation, &matW);
	}
	return matCalculation;
}
TMatrix TAnimation::Update(float fElapseTime, TMesh* pMesh, TMatrix &matParent, TScene &tScene)
{
	TMatrix mat, matReturn;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixIdentity(&matReturn);

	if (pMesh->m_bUsed != false)
	{
		if (pMesh->m_pParent)
			return Interpolate(pMesh, &matParent, fElapseTime, tScene);
		else
			return Interpolate(pMesh, &mat, fElapseTime, tScene);
	}

	return mat;
}
TAnimation::TAnimation(void)
{
}


TAnimation::~TAnimation(void)
{
}
