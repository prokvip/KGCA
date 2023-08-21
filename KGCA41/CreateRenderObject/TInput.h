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
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

