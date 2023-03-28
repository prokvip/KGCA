#pragma once
#include "TBoxObj.h"
class TSkyObj : public TBoxObj
{
public:
	void SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	TSkyObj(void);
	virtual ~TSkyObj(void);
};
