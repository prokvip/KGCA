// FSM_Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "TFsm.h"
#include "TNpc.h"
// 현재상태         이벤트				전이상태
// STAND         EVENT_TIMEMOVE           MOVE
// STAND         EVENT_POINTMOVE          MOVE
// STAND         EVENT_FINDTARGET         ATTACK
// MOVE          EVENT_STOPMOVE           STAND
// ATTACK        EVENT_LOSTTARGET         STAND
int main()
{
    TFsm fsm;
    fsm.AddTransition(STATE_STAND,EVENT_TIMEMOVE,STATE_MOVE);
    fsm.AddTransition(STATE_STAND,EVENT_FINDTARGET, STATE_ATTACK);
    fsm.AddTransition(STATE_MOVE,EVENT_STOPMOVE,STATE_STAND);
    fsm.AddTransition(STATE_ATTACK,EVENT_LOSTTARGET,STATE_STAND);
    std::cout << "Hello World!\n";

    TNpc npc(&fsm);
    while (1)
    {
        npc.Process();
        Sleep(1000);
    }
}
