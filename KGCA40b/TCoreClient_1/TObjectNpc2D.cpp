#include "TObjectNpc2D.h"
bool	TObjectNpc2D::Frame()
{
	AddPosition(TVector2(0,0));
	return true;
}