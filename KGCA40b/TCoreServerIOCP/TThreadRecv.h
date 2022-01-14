#pragma once
#include "TThread.h"
class TThreadRecv :   public TThread
{
public:
	bool		Run() override;
public:
	TThreadRecv();
	TThreadRecv(LPVOID value);
};

