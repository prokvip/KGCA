#pragma once
#include "TThread.h"
class TThreadSend :   public TThread
{
public:
	bool		Run() override;
public:
	TThreadSend();
	TThreadSend(LPVOID value);
};

