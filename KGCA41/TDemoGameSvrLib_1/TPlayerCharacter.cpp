#include "TPlayerCharacter.h"
#include "TServer.h"

/* you need the following member variables in your class
private:
  float SlowRadius;     // The max distance from an end point before slowing down

  float GoalThreshhold; // How close to and end point to get before switching (maybe necessary?)
						// Must be between 0 and 1. Make it small like 0.00001f

  float MaxSpeed;       // The maximum speed the platform can move
  float Speed;          // The speed at which the platform currently moves
---------------------------------------------------------------- */
// 물리이동 공식 참고
// https://forums.unrealengine.com/development-discussion/blueprint-visual-scripting/59667-physics-movement-calculations
void TPlayerCharacter::Update(float DeltaTime)
{
	for (TCharacterIterator iter = m_List.begin(); iter != m_List.end(); iter++)
	{
		TCharacter& tChar = (TCharacter)(iter->second);
		FVector Location = tChar.GetPos();
		FVector EndLocation = tChar.GetPos();
		FVector StartLocation = tChar.GetPos();
		float MaxSpeed = 600;
		float GoalThreshhold = 200;
		float SlowRadius = 400;

		float PathLength = (EndLocation - StartLocation).Size();
		float PathTravelled = (Location - StartLocation).Size();

		FVector NearestLocation = PathTravelled > (PathLength / 2.0f) ? EndLocation : StartLocation;
		float RadiiFromNearest = (NearestLocation - Location).Size() / SlowRadius;

		// Assuming FMath::Sin() uses degrees, i don't remember; use 1.57f for radians
		float Speed = RadiiFromNearest > 1.0f ? MaxSpeed : MaxSpeed * sinf(90.0f*RadiiFromNearest);

		if (RadiiFromNearest <= GoalThreshhold)
		{
			FVector SwapTemp = StartLocation;
			StartLocation = EndLocation;
			EndLocation = SwapTemp;
		}

		FVector Direction = EndLocation - StartLocation;
		Direction.Normalize(0.0f);
		Location +=  Direction * Speed * DeltaTime;
		tChar.SetPos(Location);
	}
}
//void AMainPlatform::UpdatePlatformBob(float DeltaTime)
//{
//	if (!PlatformBobs) { return; }
//
//	FVector Location = GetActorLocation();
//
//	float PathLength = (EndLocation - StartLocation).Size();
//	float PathTravelled = (Location - StartLocation).Size();
//
//	FVector NearestLocation = PathTravelled > (PathLength / 2.0f) ? EndLocation : StartLocation;
//	float RadiiFromNearest = (NearestLocation - Location).Size() / SlowRadius;
//
//	// Assuming FMath::Sin() uses degrees, i don't remember; use 1.57f for radians
//	Speed = RadiiFromNearest > 1.0f ? MaxSpeed : MaxSpeed * FMath::Sin(90.0f*RadiiFromNearest);
//
//	if (RadiiFromNearest <= GoalThreshhold)
//	{
//		FVector SwapTemp = StartLocation;
//		StartLocation = EndLocation;
//		EndLocation = SwapTemp;
//	}
//
//	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
//	Location += Speed * DeltaTime * Direction;
//	SetActorLocation(Location);
//}
TCharacterList& TPlayerCharacter::Get()
{
	return m_List;
}
void  TPlayerCharacter::ReleaseAll()
{
	{
		TLock lock(this);
		m_List.clear();
	}
}
void TPlayerCharacter::Delete(tGUID tGuid)
{
	{
		TLock lock(this);
		Get().erase(tGuid);
	}
}
void TPlayerCharacter::SpawnCharacters(TCharacter& tChar)
{
	TCharacterIterator itor = m_List.find(tChar.GetGuid());
	if (itor != m_List.end())
	{
		tChar.SetAlive(true);
		tChar.SetHealth(1.0f);
		tChar.SetDamage(true);
		tChar.SetAttack(true);
		m_List[tChar.GetGuid()] = tChar;
		if (tChar.GetType() == 1)
		{
			m_List[tChar.GetGuid()].SetType(1);
		}		
	}
}
void TPlayerCharacter::SyncCharacters(TCharacter& tChar)
{
	{
		TLock sync(this);
		TCharacterIterator itor = m_List.find(tChar.GetGuid());
		if (itor != m_List.end())
		{
			m_List[tChar.GetGuid()].Set(tChar.GetPos(), tChar.GetRotation(), tChar.GetVelocity());
		}
	}
}
void TPlayerCharacter::MoveTo(TCharacter& tChar)
{
	{
		TLock sync(this);
		TCharacterIterator itor = m_List.find(tChar.GetGuid());
		if (itor != m_List.end())
		{
			m_List[tChar.GetGuid()].Set(
				tChar.GetPos(), 
				tChar.GetRotation(), 
				tChar.GetVelocity());
			m_List[tChar.GetGuid()].SetTarget(tChar.GetTarget());
		}
	}
}
void TPlayerCharacter::ChangeSuperUser(tGUID tGuid)
{
	{
		TLock lock(this);
		TCharacterIterator itor = m_List.find(tGuid);
		if (itor != Get().end())
		{
			if (itor->second.GetType() != 1) return;
		}

		for (TCharacterIterator iter = m_List.begin(); iter != m_List.end(); iter++)
		{
			TCharacter& superUser = (TCharacter)(iter->second);
			if (superUser.GetType() != 1)
			{
				superUser.SetType(1);
				break;
			}
		}
	}
}
bool TPlayerCharacter::ReqZoneEntry(TCharacter& tChar)
{
	{
		TLock lock(this);
		if (m_List.size() == 0)
		{
			tChar.SetType(1);
			m_List[tChar.GetGuid()] = tChar;
			return true;
		}
	}
	return false;
}
void TPlayerCharacter::AckZoneEntry(TCharacter& tChar)
{
	{
		TLock lock(this);
		tChar.SetPos(tChar.GetPos() + FVector(rand() / 300, rand() / 300, 0.0f));		
		m_List[tChar.GetGuid()] = tChar;
	}
}

TPlayerCharacter::TPlayerCharacter()
{
}


TPlayerCharacter::~TPlayerCharacter()
{
}

ostream& operator<<(ostream &stream, TPlayerCharacter& info)
{
	TLock sync(&info);
	stream << info.m_List.size();
	for (auto& data : info.m_List)
	{
		stream << data.first;
		stream << data.second;
	}
	return stream;
}

istream &operator>>(istream &stream, TPlayerCharacter& info)
{
	TLock sync(&info);
	info.m_List.clear();
	size_t nPlayers = 0;
	tGUID tGuid;
	TCharacter tChar;
	stream >> nPlayers;
	for (int i = 0; i < nPlayers; i++)
	{
		stream >> tGuid;
		stream >> tChar;
		info.m_List.insert(make_pair(tGuid, tChar));
	}
	return stream;
}