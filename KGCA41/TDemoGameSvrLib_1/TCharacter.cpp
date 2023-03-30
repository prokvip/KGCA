#include "TCharacter.h"
FVector TCharacter::GetTarget()
{
	return m_Info.vTarget;
};
void TCharacter::SetTarget(FVector target)
{
	m_Info.vTarget = target;
};
tGUID  TCharacter::GetGuid() 
{
	return m_Info.tGuid;
};
FVector TCharacter::GetPos()
{ 
	return m_Info.vPosition; 
};
FVector TCharacter::GetVelocity() 
{ 
	return m_Info.vVelocity; 
};
FRotator TCharacter::GetRotation() 
{ 
	return m_Info.vRotation; 
};
float	TCharacter::GetHealth()
{ 
	return m_Info.fHealth; 
};
int     TCharacter::GetType() 
{ 
	return m_Info.iType; 
}
bool	TCharacter::GetAlive() 
{ 
	return m_Info.bAlive; 
}     // 리스폰 가능여부(중복체크용)
bool	TCharacter::GetAttack() 
{ 
	return m_Info.bAttack; 
}    // 공격 가능여부(중복체크용)
bool	TCharacter::GetDamage() 
{ 
	return m_Info.bDamage; 
}	// 데미지 가능여부(중복체크용)
TPACKET_CHARACTER_INFO& TCharacter::GetInfo()
{ 
	return m_Info; 
}
//-------------------------------------------------
tGUID  TCharacter::MakeGuid()
{
	tGUID tGuid;
	RPC_STATUS ret_val = ::UuidCreate(&tGuid);
	if(ret_val == RPC_S_OK)
		m_Info.tGuid = tGuid;
	return m_Info.tGuid;
}
void  TCharacter::SetGuid(tGUID tGuid)
{
	m_Info.tGuid = tGuid;
};
void TCharacter::SetPos(FVector pos)
{
	m_Info.vPosition = pos;
};
void TCharacter::SetVelocity(FVector velo)
{
	m_Info.vVelocity= velo;
};
void TCharacter::SetRotation(FRotator rot)
{
	m_Info.vRotation = rot;
};
void TCharacter::SetHealth(float fHealth)
{
	m_Info.fHealth = fHealth;
};
void     TCharacter::SetType(int iType)
{
	m_Info.iType=iType;
}
void	TCharacter::SetAlive(bool bAlive)
{
	m_Info.bAlive = bAlive;
}   
void	TCharacter::SetAttack(bool bAttack)
{
	m_Info.bAttack = bAttack;
}   
void	TCharacter::SetDamage(bool bDamage)
{
	m_Info.bDamage = bDamage;
}	
void TCharacter::SetInfo(TPACKET_CHARACTER_INFO info)
{
	m_Info = info;
}
void	TCharacter::Set(FVector vPosition, FRotator vRotation, FVector vVelocity)
{
	SetPos(vPosition);
	SetRotation(vRotation);
	SetVelocity(vVelocity);
}
void	TCharacter::Set(tGUID tGuid)
{
	ZeroMemory(&m_Info, sizeof(TPACKET_CHARACTER_INFO));
	SetGuid(tGuid);
	m_Info.fHealth = 1.0f;
	m_Info.bAlive = true;
	m_Info.bAttack = true;
	m_Info.bDamage = true;
}
TCharacter::TCharacter()
{
	ZeroMemory(&m_Info, sizeof(TPACKET_CHARACTER_INFO));
	m_Info.fHealth = 1.0f;
	m_Info.bAlive = true;
	m_Info.bAttack = true;
	m_Info.bDamage = true;
}


TCharacter::~TCharacter()
{
}

ostream& operator<<(ostream &stream, TCharacter& tChar)
{
	stream << tChar.m_Info;
	return stream;
}
istream& operator>>(istream& stream, TCharacter& tChar)
{
	stream >> tChar.m_Info;
	return stream;
}