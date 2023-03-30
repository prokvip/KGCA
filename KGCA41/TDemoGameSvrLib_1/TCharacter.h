#pragma once
#include "TServerObj.h"
#include "TProtocol.h"
#include "TStreamPacket.h"
#include "TAcceptor.h"
#include "TDebugString.h"
#include "TTargetPointMgr.h"

class TCharacter : public TServerObj
{
private:
	TPACKET_CHARACTER_INFO m_Info;
public:
	tGUID  GetGuid();
	FVector GetPos();
	FVector GetVelocity();
	FVector GetTarget();
	FRotator GetRotation();
	float	GetHealth();
	int     GetType();
	bool	GetAlive();     // 리스폰 가능여부(중복체크용)
	bool	GetAttack();    // 공격 가능여부(중복체크용)
	bool	GetDamage();	// 데미지 가능여부(중복체크용)	
	TPACKET_CHARACTER_INFO& GetInfo();
	//-----------------------------------------------------
	tGUID	MakeGuid();
	void	SetGuid(tGUID tGuid);
	void	SetPos(FVector pos);
	void	SetVelocity(FVector velo);
	void	SetTarget(FVector target);
	void	SetRotation(FRotator rot);
	void	SetHealth(float fHealth);
	void    SetType(int iType);
	void	SetAlive(bool bAlive);
	void	SetAttack(bool bAttack);
	void	SetDamage(bool bDamage);
	void	Set(FVector vPosition, FRotator vRotation, FVector vVelocity);
	void    SetInfo(TPACKET_CHARACTER_INFO info);
	void    Set(tGUID tGuid);
public:
	friend ostream& operator<<(ostream &stream, TCharacter& tChar);
	friend istream& operator>>(istream& stream, TCharacter& tChar);
public:
	TCharacter();
	virtual ~TCharacter();
};

