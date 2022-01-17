#pragma once
#include "TThread.h"
class TAccepter :   public TThread
{
public:
	bool		Run() override;
public:
	TAccepter();
	TAccepter(LPVOID value);
};

