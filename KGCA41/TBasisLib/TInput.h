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
    POINT m_MousePos;
    DWORD m_dwKeyState[256];
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
private:
    TInput() {};
public:
    ~TInput() {}
};

#define I_Input TInput::GetInstance()