#include "Sample.h"

bool	Sample::Init()
{        
    return true;
}
bool		Sample::Frame()
{
/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x3A - 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
    if( I_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
    {
        INT K = 0;
    }
    if (I_Input.GetKey(VK_LBUTTON) == KEY_UP)
    {
        INT K = 0;
    }
    if (I_Input.GetKey('W') == KEY_PUSH)
    {
        INT K = 0;
    }
    if (I_Input.GetKey('W') == KEY_UP)
    {
        INT K = 0;
    }
    if (I_Input.GetKey('A') == KEY_PUSH)
    {
        INT K = 0;
    }
    if (I_Input.GetKey('S') == KEY_PUSH)
    {
        INT K = 0;
    }
    if (I_Input.GetKey('D') == KEY_PUSH)
    {
        INT K = 0;
    }
    return true;
}
bool		Sample::Render()
{
 
    return true;
}
bool		Sample::Release()
{   
    return true;
}


GAME_RUN(TGameCore, 1024, 768)
