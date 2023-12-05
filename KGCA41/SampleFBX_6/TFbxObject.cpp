#include "TFbxObject.h"

void TFbxObj::SetAnim(UINT startFrame, UINT endFrame, UINT frameSpeed, UINT tickForFrame)
{
	m_iStartFrame = startFrame;
	m_iEndFrame = endFrame;
	m_iFrameSpeed = frameSpeed;
	m_iTickForFrame = tickForFrame;
}
UINT    TFbxObj::GetStartFrame()
{
	return m_iStartFrame;
}
UINT    TFbxObj::GetEndFrame()
{
	return m_iEndFrame;
}
UINT    TFbxObj::GetFrameSpeed()
{
	return m_iFrameSpeed;
}
UINT    TFbxObj::GetTickForFrame()
{
	return m_iTickForFrame;
}