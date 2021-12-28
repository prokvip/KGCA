#include "Sample.h"
bool	Sample::Init()
{
	for (int iObj = 0; iObj < 3; iObj++)
	{
		float fStep = 100+(iObj*100);
		TDxObject obj;
		obj.Init();

		///0 ~ g_rtClient
		if(obj.Create(m_pd3dDevice, m_pImmediateContext,
			TVector2(100*iObj+fStep, 300), 100, 100))
		{
			m_ObjectList.push_back(obj);
		}
	}
	return true;
}
bool	Sample::Frame()
{	
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].Frame();
	}
	return true;
}
bool	Sample::Render()
{
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].Render();
	}
	return true;
}
bool	Sample::Release()
{	
	for (int iObj = 0; iObj < m_ObjectList.size(); iObj++)
	{
		m_ObjectList[iObj].Release();
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}
RUN();