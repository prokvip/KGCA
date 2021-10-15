#include "Sample.h"
bool		Sample::Init()
{
	m_Texture.LoadTexture(L"../../data/bitmap1.BMP");
	m_FbxObj.LoadObject("../../data/box.fbx");

	m_pImmediateContext->PSSetSamplers(0, 1, &m_Texture.m_pSampler);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Texture.m_pTextureSRV);
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
	m_Texture.Release();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
TBASIS_RUN(SampleLibrary)