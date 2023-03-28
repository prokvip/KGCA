#pragma once
#include "TObjStd.h"
#include "TMesh.h"

class TAnimation
{
public:
	TMatrix	Interpolate(TMesh* pMesh, TMatrix* matParents, float fFrameTick, TScene tScene);
	TMatrix	Update(float fElapseTime, TMesh* pMesh, TMatrix &matWorld, TScene &tScene);
	bool		GetAnimationTrack(float fFrame, vector<shared_ptr<TAnimTrack>>& pTrackList, TAnimTrack** pStartTrack, TAnimTrack** pEndTrack);
public:
	TAnimation(void);
	virtual ~TAnimation(void);
};

