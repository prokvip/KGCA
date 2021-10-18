#include "Sample.h"
bool		Sample::Init()
{
	m_FbxObj.LoadObject("../../data/box.fbx");
	return true;
}
bool		Sample::Frame() {
	return true;
}
bool		Sample::Render() 
{
	m_FbxObj.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObj.Render(m_pImmediateContext);	
	return true;
}
bool		Sample::Release() 
{
	m_FbxObj.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)