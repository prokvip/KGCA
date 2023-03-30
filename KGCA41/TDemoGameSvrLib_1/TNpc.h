#pragma once
#include "TCharacter.h"

class TNpc : public TCharacter
{
	float	m_fTraceRange;		// 추격 범위
	float	m_fHitRange;		// 타격 범위
	float	m_fHitValue;		// 타격 포인트	
public:	
	void SendMove(TCharacter& pDesk);
	void SendAttack(TCharacter& pDesk);
	void MoveTo( TCharacter& target);
	void HitPlayer(TCharacter& target);
	void Damaged(float damage);
	bool MoveChecker( TCharacter& target);
	bool AttackChecker( TCharacter& target);
public:
	TNpc();
	virtual ~TNpc();
};

class TNpcManager : public TThread
{
private:
	map<tGUID, TNpc, GUIDComparer>	m_NpcList;
	TTargetPointMgr					m_tpMgr;
	TCharacter*						m_pTargetChar;
	HANDLE							m_hMutex;
public:
	int GetNpcCount();
	void  Lock();
	void  Unlock();
	bool Init();
	bool Run();
	bool Set();
	bool Release();
	void SyncNpc(int iPacketType);
	void ReSetTargetPoint();
	void SendAttack(TNpc* pNpc, TCharacter* pDesk);
	void SendMove(TNpc* pNpc, TCharacter* pDesk);
	void Add(FVector vPosition, FRotator vRotation = FRotator::ZeroRotator, FVector vVelocity= FVector::ZeroVector);
public:
	friend ostream& operator<<(ostream &stream, TNpcManager& npcList);
	friend istream& operator>>(istream& stream, TNpcManager& npcList);
public:
	TNpcManager();
	virtual ~TNpcManager();
};
