#include "TCore.h"
bool  TCore::Init() { return true; }
bool  TCore::Frame() { return true; }
bool  TCore::Render() { return true; }
bool  TCore::Release() { return true; }
bool  TCore::EngineInit()
{
    TDevice::Init();
   
    m_GameTimer.Init();    
    TInput::GetInstance().Init();
    m_MainCamera.Init();
	Init();    
	return true;
}
bool  TCore::EngineFrame()
{
    m_GameTimer.Frame();
    TInput::GetInstance().Frame();
    m_MainCamera.Frame();
    TDevice::Frame();
	Frame();
	return true;
}
bool  TCore::EngineRender()
{
    TDevice::PreRender();
	Render();

    m_MainCamera.Render();
    m_GameTimer.Render();
    TInput::GetInstance().Render();
    TDevice::PostRender();
	return true;
}
bool  TCore::EngineRelease()
{
	Release();
    m_GameTimer.Release();
    TInput::GetInstance().Release();
    m_MainCamera.Release();
    TDevice::Release();
	return true;
}
bool TCore::Run()
{
    EngineInit();
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 게임로직을 처리
            if (!EngineFrame() || !EngineRender())
            {
                break;
            }
        }
    }
    EngineRelease();
    return true;
}