#include "Sample.h"
#include "TFsm.h"
LRESULT  Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			TPacket tPacket(PACKET_CHAT_MSG);
			tPacket.m_uPacket.ph.time = timeGetTime();
			tPacket << 999 << "ȫ�浿" << buffer;
			m_Net.SendMsg(m_Net.m_Sock, tPacket.m_uPacket);

			SendMessageA(m_hEdit, WM_SETTEXT, 0, (LPARAM)"");
		}break;
		}
	}break;
	}
	return m_Net.MsgProc(hWnd, msg, wParam, lParam);
}

bool	Sample::Init()
{
	I_Fsm.AddTransition(STATE_STOP, STATE_STOP_TIMER, STATE_MOVE);
	I_Fsm.AddTransition(STATE_STOP, STATE_FIND_TARGET, STATE_ATTACK);

	I_Fsm.AddTransition(STATE_MOVE, STATE_STOP_TIMER, STATE_STOP);
	I_Fsm.AddTransition(STATE_MOVE, STATE_FIND_TARGET, STATE_ATTACK);
	I_Fsm.AddTransition(STATE_MOVE, STATE_LOST_TARGET, STATE_STOP);

	I_Fsm.AddTransition(STATE_ATTACK, STATE_LOST_TARGET, STATE_STOP);

	TShader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(), L"VertexShader.txt", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), L"PixelShader.txt", "PS");

	m_PlayerObj.Init();	
	m_PlayerObj.SetRectSouce({91,1,42,56});
	//m_PlayerObj.SetRectSouce({ 46,63,69,79 });
	m_PlayerObj.SetRectDraw({ 0,0, 42,56 });
	m_PlayerObj.SetPosition(TVector2(400,300));
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;

	if (!m_PlayerObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),		
		nullptr,
		L"../../data/bitmap1.bmp",
		L"../../data/bitmap2.bmp" ))
	{
		return false;
	}

	for (int iNpc = 0; iNpc < 10; iNpc++)
	{	
		std::shared_ptr<TObjectNpc2D> npc = std::make_shared<TObjectNpc2D>();
		npc->Init();
		if (iNpc % 2 == 0)
		{
			npc->SetRectSouce({ 46,63,69,79 });
			npc->SetRectDraw({ 0,0, 69,79 });
		}
		else
		{
			npc->SetRectSouce({ 1,63,42,76 });
			npc->SetRectDraw({ 0,0, 42,76 });
		}
		
		npc->SetPosition(TVector2(50+iNpc*150, 50));

		npc->m_pVShader = pVShader;
		npc->m_pPShader = pPShader;

		if (!npc->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),
			nullptr,
			L"../../data/bitmap1.bmp",
			L"../../data/bitmap2.bmp"))
		{
			return false;
		}
		m_NpcLlist.push_back(npc);
	}

	m_Net.InitNetwork();
	m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, "192.168.0.12");
	return true;
}
bool	Sample::Frame()
{
	m_PlayerObj.Frame();

	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Frame();
		m_NpcLlist[iObj]->m_pStateAction->Process(&m_PlayerObj);
	}

	int iChatCnt = m_Net.m_PlayerUser.m_packetPool.size();
	if (iChatCnt > 0 && m_iChatCnt != iChatCnt)
	{
		m_iChatCnt = iChatCnt;
		SendMessage(m_hListBox, LB_RESETCONTENT, 0, 0);

		std::list<TPacket>::iterator iter;
		if (m_Net.m_PlayerUser.m_packetPool.size() > 20)
		{
			m_Net.m_PlayerUser.m_packetPool.pop_front();
		}
		for (iter = m_Net.m_PlayerUser.m_packetPool.begin();
			iter != m_Net.m_PlayerUser.m_packetPool.end();
			iter++)
		{
			UPACKET& uPacket = (*iter).m_uPacket;
			switch ((*iter).m_uPacket.ph.type)
			{
				case PACKET_LOGIN_ACK:
				{
					DWORD dwCurrent = timeGetTime();
					DWORD dwEnd = 0;
					dwEnd = dwCurrent - uPacket.ph.time;
					TLoginAck ack;
					memcpy(&ack, (*iter).m_uPacket.msg, sizeof(TLoginAck));
					if (ack.iResult == 1)
					{
						int k = 0;
					}
				}break;
				case PACKET_CHAT_MSG:
				{
					DWORD dwCurrent = timeGetTime();
					DWORD dwEnd = 0;
					dwEnd = dwCurrent - uPacket.ph.time;
					if (dwEnd >= 1)
					{
						std::string data = std::to_string(dwEnd);
						data += "\n";
						OutputDebugStringA(data.c_str());
					}
					TChatMsg recvdata;
					ZeroMemory(&recvdata, sizeof(recvdata));
					(*iter) >> recvdata.index >> recvdata.name
						>> recvdata.message;
					SendMessageA(m_hListBox, LB_ADDSTRING, 0,
						(LPARAM)recvdata.message);
				}break;
			}			
			//iter = m_Net.m_PlayerUser.m_packetPool.erase(iter);
			//(*iter).Reset();
		}
		m_Net.m_PlayerUser.m_packetPool.clear();
	}
	return true;
}
bool	Sample::Render()
{	
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Render();
	}
	m_PlayerObj.Render();
	return true;
}
bool	Sample::Release()
{	
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
	}	
	m_PlayerObj.Release();

	m_Net.CloseNetwork();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}
RUN();