#include "TFbxObject.h"

bool	TFbxObj::CreateConstantBuffer()
{
	TDxObject::CreateConstantBuffer();

	HRESULT hr;
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TBoneWorld);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	//D3D11_SUBRESOURCE_DATA sd;
	//ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//sd.pSysMem = &m_matBoneArray;

	if (FAILED(hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pBoneCB)))
	{
		return false;
	}

	
	return true;
}
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