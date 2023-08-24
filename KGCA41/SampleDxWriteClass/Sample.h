#pragma once
#include "TCore.h"
#include "TWriter.h"
class Sample : public TCore
{
	TWriter    m_font;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

