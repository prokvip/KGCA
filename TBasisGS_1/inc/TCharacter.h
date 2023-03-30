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
	bool	GetAlive();     // ������ ���ɿ���(�ߺ�üũ��)
	bool	GetAttack();    // ���� ���ɿ���(�ߺ�üũ��)
	bool	GetDamage();	// ������ ���ɿ���(�ߺ�üũ��)	
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

