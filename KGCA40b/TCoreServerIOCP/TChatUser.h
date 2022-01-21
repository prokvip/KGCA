#pragma once
#include "TNetUser.h"
class TChatUser :   public TNetUser, 
					public TObjectPool<TChatUser>
{
};

