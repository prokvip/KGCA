#pragma once
#include "TNpc.h"


typedef std::map<tGUID, TCharacter, GUIDComparer> TCharacterList;

class TPlayerCharacter : public TServerObj
{
private:
	TCharacterList  m_List;
public:
	TCharacterList& Get();
	void  Update(float DeltaTime);
	void  Delete(tGUID tGuid);
	void  ReleaseAll();
	void  ChangeSuperUser(tGUID tGuid);
	void  MoveTo(TCharacter& tChar);
	void  SyncCharacters(TCharacter& tChar);
	void  SpawnCharacters(TCharacter& tChar);
	bool  ReqZoneEntry(TCharacter& tChar);
	void  AckZoneEntry(TCharacter& tChar);
public:
	TPlayerCharacter();
	virtual ~TPlayerCharacter();
	friend ostream& operator<<(ostream &stream, TPlayerCharacter& info);
	friend istream &operator>>(istream &stream, TPlayerCharacter& info);
};

