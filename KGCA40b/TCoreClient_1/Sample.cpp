#include "Sample.h"
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
			tPacket << 999 << "홍길동" << buffer;
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
	/*DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE ;
	m_hEdit = CreateWindow(L"edit", NULL, style, 
		0, g_rtClient.bottom-50, 300, 50,
		m_hWnd, (HMENU)100, m_hInstance, NULL);
	style = WS_CHILD | WS_VISIBLE;
	m_hButton = CreateWindow(L"button", L"전송", style,
		310, g_rtClient.bottom - 50, 50, 50,
		m_hWnd, (HMENU)200, m_hInstance, NULL);
	m_hListBox = CreateWindow(L"listbox", NULL, style,
		0, 0, 300, g_rtClient.bottom - 70,
		m_hWnd, (HMENU)300, m_hInstance, NULL);

	SendMessageA(m_hListBox, LB_ADDSTRING, 0, (LPARAM)"체팅시작!");*/
	
	/*for (int iObj = 0; iObj < 12; iObj++)
	{
		TDxObject obj;
		obj.Init();

		///0 ~ g_rtClient
		if (obj.Create(m_pd3dDevice, m_pImmediateContext,
			TVector2(-100 * iObj, iObj * 50), 400, 30))
		{
			m_ObjectList.push_back(obj);
		}
	}*/

	TDxObject obj;
	obj.Init();
	///0 ~ g_rtClient
	if (obj.Create(m_pd3dDevice, m_pImmediateContext,
		TVector2(400, 300), 400, 300))
	{
		m_ObjectList.push_back(obj);
	}

	HRESULT hr;
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		L"../../data/bitmap1.bmp", // 불투명알파 텍스처
		//L"../../data/bitmap1Alpha.bmp",// 투명알파 텍스처		
		//L"../../data/wireframe.dds",
		&m_pTexture0,
		&m_pSRV0);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			m_pd3dDevice,
			L"../../data/bitmap1.bmp", // 불투명알파 텍스처
			//L"../../data/bitmap1Alpha.bmp",// 투명알파 텍스처		
			//L"../../data/wireframe.dds",
			&m_pTexture0,
			&m_pSRV0);
	}
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		L"../../data/bitmap2.bmp",
		&m_pTexture1,
		&m_pSRV1);

	// (소스컬러*D3D11_BLEND_SRC_ALPHA) 
	//                  + 
	// (대상컬러*D3D11_BLEND_INV_SRC_ALPHA)
	// 컬러   =  투명컬러값 = (1,1,1,1)
	// 마스크 =  1.0 - 투명컬러값 = (1,1,1,1)

	// FinalColor = SrcColor*SrcAlpha + DestColor*(1.0f- SrcAlpha) 	    
	// if SrcAlpha == 0 완전투명
	//           FinalColor() = SrcColor*0 + DestColor*(1-0)
	//                FinalColor = DestColor;
	// if SrcAlpha == 1 완전불투명
	//           FinalColor() = SrcColor*1 + DestColor*(1-1)
	//                FinalColor = SrcColor;
	// 혼합상태 = 소스(지금드로우객체 픽셀) (연산) 대상(백버퍼 객체:픽셀)
	// 혼합상태 = 픽셀쉐이더 출력 컬러  (연산:사칙연산) 출력버퍼의 컬러
	D3D11_BLEND_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	/*bd.AlphaToCoverageEnable;
	bd.IndependentBlendEnable;*/
	bd.RenderTarget[0].BlendEnable = TRUE;	
	bd.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend	= D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
	//// A 연산 저장
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha= D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 
					D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pd3dDevice->CreateBlendState(&bd, &m_AlphaBlend);


	m_Net.InitNetwork();
	m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, "192.168.0.12");

	return true;
}
bool	Sample::Frame()
{
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].Frame();
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
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSRV0);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_pSRV1);
	m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].Render();
	}
	return true;
}
bool	Sample::Release()
{	
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].Release();
	}

	m_pTexture0->Release();
	m_pSRV0->Release();
	m_pTexture1->Release();
	m_pSRV1->Release();

	m_Net.CloseNetwork();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}
RUN();