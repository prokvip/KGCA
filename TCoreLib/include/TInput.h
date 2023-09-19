#pragma once
#include "TStd.h"
enum KeyState
{
    KEY_FREE = 0,
    KEY_UP = 1,
    KEY_PUSH = 2,
    KEY_HOLD = 3,
};
class TInput
{
public:
    TVector3  m_vOffset = { 0,0,0 };
    POINT  m_MousePos;
    POINT  m_BeforeMousePos;
    DWORD m_dwKeyState[256];
    DWORD GetKey(DWORD key)
    {
        return m_dwKeyState[key];
    }
    static TInput& GetInstance()
    {
        static TInput input;
        return input;
    }
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
    TVector3 GetWorldPos(TVector2 vWindow, TVector3 vCamera);
private:
    TInput() {};
public:
    ~TInput() {}
};

#define I_Input TInput::GetInstance()