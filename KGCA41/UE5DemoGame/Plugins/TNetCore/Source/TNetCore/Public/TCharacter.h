#pragma once
#include "CoreMinimal.h"
#include "TStreamPacket.h"

class TNETCORE_API TCharacter
{
private:
	TPACKET_CHARACTER_INFO m_Info;
public:
	tGUID  GetGuid() const;
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
	void	SetGuid(tGUID tGuid);
	void	SetPos(FVector pos);
	void	SetVelocity(FVector velo);
	void	SetTarget(FVector pos);
	void	SetRotation(FRotator rot);
	void	SetHealth(float fHealth);
	void    SetType(int iType);
	void	SetAlive(bool bAlive);
	void	SetAttack(bool bAttack);
	void	SetDamage(bool bDamage);
	void	Set(FVector vPosition, FRotator vRotation, FVector vVelocity);
	void    SetInfo(TPACKET_CHARACTER_INFO info);
	void    Set();
public:
	friend ostream& operator<<(ostream &stream, TCharacter& tChar);
	friend istream& operator>>(istream& stream, TCharacter& tChar);
public:
	TCharacter();
	virtual ~TCharacter();
};

class TNETCORE_API TCharacterMap
{
public:
	map<tGUID, TCharacter, GUIDComparer> tCharMap;
public:
	friend ostream& operator<<(ostream &stream, TCharacterMap& info);
	friend istream &operator>>(istream &stream, TCharacterMap& info);
public:
	TCharacterMap() {};
	virtual ~TCharacterMap() {};
};