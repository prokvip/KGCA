#pragma once
#include "TFileIO.h"
#include "TLinkedlist.h"

//printf("\n���(0), ����(1), �ε�(2), �ʱⰪ(3), ����(9)");
enum work { Print = 0, Save, Load, Create, Find, Exit };