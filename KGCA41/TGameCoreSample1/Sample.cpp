#include "Sample.h"
bool Sample::Run()
{
	TDevice::Init();
	
	TWindow::Run();
	
	TDevice::Release();
	return true;
}
GAME_RUN(TGameCoreSample1, 800,600)