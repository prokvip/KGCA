#include "TCore.h"
bool  TCore::Init() { return true; }
bool  TCore::Frame() { return true; }
bool  TCore::Render() { return true; }
bool  TCore::Release() { return true; }
bool  TCore::EngineInit()
{
	Init();
	return true;
}
bool  TCore::EngineFrame()
{
	Frame();
	return true;
}
bool  TCore::EngineRender()
{
	Render();
	return true;
}
bool  TCore::EngineRelease()
{
	Release();
	return true;
}