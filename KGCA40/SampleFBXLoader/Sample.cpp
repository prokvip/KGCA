#include "Sample.h"
bool		Sample::Init()
{
	//m_FbxObj.LoadObject("../../data/object/box.fbx");
	//m_FbxObj.LoadObject("../../data/object/ship.fbx");
	m_FbxObjA.LoadObject("../../data/object/multiCamera.fbx");
	m_FbxObjB.LoadObject("../../data/object/Turret.fbx");
	m_Camera.CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool		Sample::Frame() {
	return true;
}
bool		Sample::Render() 
{
	m_FbxObjA.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObjA.Render(m_pImmediateContext);	
	m_FbxObjB.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_FbxObjB.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release() 
{
	m_FbxObjA.Release();
	m_FbxObjB.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)