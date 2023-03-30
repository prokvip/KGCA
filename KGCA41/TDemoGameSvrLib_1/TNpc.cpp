#include "TNpc.h"
#include <thread>
#include "TServer.h"

void TNpc::MoveTo(TCharacter & target)
{
	std::thread t([&]()
	{
		SendMove(target);
		std::this_thread::sleep_for(1s);
	});
	t.detach();	
}
void TNpc::HitPlayer(TCharacter & target)
{
	std::thread t([&]() 
	{
		SetAttack(false);		
		target.SetHealth(target.GetHealth() - m_fHitValue);		
		SendAttack(target);		
		std::this_thread::sleep_for(3s);
		SetAttack(true);
	});
	t.detach();		
}
void TNpc::SendMove(TCharacter& pDesk)
{
	stringstream data;
	data << *this;
	data << pDesk;
	I_Server.SendBroadcastPool(MakePacket(PACKET_NPC_TARGETMOVE, data));
}
void TNpc::SendAttack(TCharacter& pDesk)
{
	stringstream data;
	data << *this;
	data << pDesk;

	I_Server.SendBroadcastPool(MakePacket(PACKET_ATTACK_NPC, data));
	T_LOG("[%s]\r\n", "PACKET_ATTACK_NPC");

	if (pDesk.GetHealth() <= 0.0f )
	{
		stringstream dataDead;
		pDesk.SetAlive(false);
		dataDead <<  pDesk;
		I_Server.SendBroadcastPool(MakePacket(PACKET_DEAD_CHARACTER, dataDead));
		T_LOG("PACKET_DEAD_CHARACTER Send\r\n");
	}	
	else
	{
		stringstream damage;
		damage << pDesk;
		I_Server.SendBroadcastPool(MakePacket(PACKET_DAMAGE_CHARACTER, damage));
		T_LOG("PACKET_DAMAGE_CHARACTER Send\r\n");
	}
}
void TNpc::Damaged(float damage)
{
	SetHealth(GetHealth()-damage);	
}

bool TNpc::MoveChecker(TCharacter & target)
{
	double dist = GetPos().DistanceTo(target.GetPos());
	if (dist < m_fTraceRange)
	{		
		return true;
	}
	return false;
}

bool TNpc::AttackChecker(TCharacter & target)
{
	float fDistance = GetPos().DistanceTo(target.GetPos());
	if (fDistance < m_fHitRange)
	{
		return true;
	}
	return false;
}

TNpc::TNpc()
{
	m_fTraceRange = 700.0f;
	m_fHitRange = 200.0f;
	m_fHitValue = 0.5f;
}


TNpc::~TNpc()
{
}


bool TNpcManager::Init()
{		
	return true;
}
bool TNpcManager::Release()
{
	Lock();
		m_NpcList.clear();
	Unlock();
	return true;
}
void TNpcManager::ReSetTargetPoint()
{
	Lock();
		m_NpcList.clear();
		for (int iNum = 0; iNum < I_Server.GetTargetPointMgr().Get().size(); iNum++)
		{
			Add(I_Server.GetTargetPointMgr().Get()[iNum].vPosition);
		}	
	Unlock();
}
void TNpcManager::Add(FVector vPosition, FRotator vRotation, FVector vVelocity)
{
	Lock();
		TNpc npc;
		npc.Set(vPosition, vRotation, vVelocity);
		npc.SetGuid(npc.MakeGuid());
		m_NpcList[npc.GetGuid()] = npc;
	Unlock();
}

bool TNpcManager::Set()
{
	CreateThread();
	return true;
}
bool TNpcManager::Run()
{
	Init();
	while (true)
	{	
		{
			Lock();
			for (auto & npc : m_NpcList)
			{
				auto & tNpc = npc.second;
				if (!tNpc.GetAlive())
				{
					continue;
				}

				ISessionMgr.Lock();
				for (auto & player : I_Server.GetPCList().Get())
				{
					TCharacter& playCharacter = player.second;
					if (!playCharacter.GetAlive())
					{
						continue;
					}						
					if (tNpc.AttackChecker(playCharacter))
					{
						if (tNpc.GetAttack())
						{
							tNpc.HitPlayer(playCharacter);
							break;
						}								
					}
					else if (tNpc.MoveChecker(playCharacter))
					{
						tNpc.MoveTo(playCharacter);
						break;
					}												
				}
				ISessionMgr.Unlock();
			}
			/*if (m_NpcList.size() > 0)
			{
				SyncNpc(PACKET_SYNC_NPC_LIST);
			}*/
			Unlock();
			
		}
		Sleep(30);
	}
	Release();
	return true;
}

void TNpcManager::SyncNpc(int iPacketType )
{
	std::thread t([&]()
	{
		stringstream data;
		data << *this;
		I_Server.SendBroadcastPool(MakePacket(iPacketType, data));
		std::this_thread::sleep_for(1s);
	});
	t.detach();
	T_LOG("[%s]\r\n", "PACKET_SYNC_NPC_LIST");
}

void TNpcManager::SendAttack(TNpc* pNpc, TCharacter* pDesk)
{
}

void TNpcManager::SendMove(TNpc* pNpc, TCharacter* pDesk) 
{
	if (m_pTargetChar != nullptr) return;
	m_pTargetChar = pDesk;
	stringstream data;
	data << *pNpc;
	data << *pDesk;
	I_Server.SendBroadcastPool(MakePacket(PACKET_NPC_TARGETMOVE, data));
}
void TNpcManager::Lock()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}
void  TNpcManager::Unlock()
{
	ReleaseMutex(m_hMutex);
}
int TNpcManager::GetNpcCount() 
{
	return (int)m_NpcList.size();
}
TNpcManager::TNpcManager()
{
	m_pTargetChar = nullptr;
	m_hMutex = CreateMutex(NULL, FALSE, _T("TNpcManager"));
}
TNpcManager::~TNpcManager()
{
	CloseHandle(m_hMutex);
	m_hMutex = 0;
}

ostream& operator<<(ostream &stream, TNpcManager& npcList)
{
	{
		npcList.Lock();
		stream << npcList.m_NpcList.size();
		for (auto& npc : npcList.m_NpcList)
		{
			stream << npc.first;
			stream << npc.second;
		}
		npcList.Unlock();
	}
	return stream;
}
istream& operator>>(istream& stream, TNpcManager& npcList)
{
	{
		npcList.Lock();
		tGUID tGuid;
		TNpc tNpc;
		size_t iNumNpc = 0;
		stream >> iNumNpc;
		for (int i = 0; i < iNumNpc; i++)
		{
			stream >> tGuid;
			stream >> tNpc;	

			map<tGUID, TNpc, GUIDComparer>::iterator iter;
			iter =npcList.m_NpcList.find(tGuid);
			if (iter != npcList.m_NpcList.end())
			{
				npcList.m_NpcList[tGuid].Set(tNpc.GetPos(), tNpc.GetRotation(), tNpc.GetVelocity());
			}
			else
			{
				T_LOG("[%s]\r\n", "npcList Error");
			}
		}
		npcList.Unlock();
	}
	return stream;
}