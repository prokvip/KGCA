#include "TTextureMgr.h"
#include <wincodec.h>
#include <wincodecsdk.h>
#include "ScreenGrab.h"
bool	TTexture::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice,
		filename.c_str(),
		(ID3D11Resource**)m_pTexture.GetAddressOf(),
		m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice,
			filename.c_str(),
			(ID3D11Resource**)m_pTexture.GetAddressOf(),
			m_pSRV.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_pTexture->GetDesc(&m_TextureDesc);
	return true;
}
bool	TTexture::Init()
{
	return true;
}
bool	TTexture::Frame()
{	
	return true;
}
bool	TTexture::Render()
{
	return true;
}
bool	TTexture::Release()
{
	return true;
}
TTexture::TTexture()
{

}
TTexture::~TTexture()
{

}


HRESULT TTextureMgr::SaveFile(ID3D11DeviceContext* pContext, ID3D11Texture2D* pRes, T_STR name)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> tex = pRes;
	GUID fileformat = GUID_ContainerFormatPng;
	WCHAR ext[_MAX_EXT];
	_wsplitpath_s(name.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
	T_STR tExt = ext;
	if (tExt == L".dds" || tExt.empty())
	{
		if (tExt.empty())
		{
			hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), (name + L".dds").c_str());
		}
		else
		{
			hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), name.c_str());
		}
	}
	else
	{
		if (tExt == L".bmp") fileformat = GUID_ContainerFormatBmp;
		if (tExt == L".png") fileformat = GUID_ContainerFormatPng;
		if (tExt == L".ico") fileformat = GUID_ContainerFormatIco;
		if (tExt == L".jpg") fileformat = GUID_ContainerFormatJpeg;
		if (tExt == L".tif") fileformat = GUID_ContainerFormatTiff;
		hr = DirectX::SaveWICTextureToFile(pContext, tex.Get(), fileformat, name.c_str());
	}

	if (pRes == nullptr)
	{
		tex->Release();
	}

	return hr;
}
HRESULT TTextureMgr::SaveFile(ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain, T_STR name)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> tex;
	//time_t t;
	//struct tm *ts;
	//t = time(NULL);
	//ts = localtime(&t);
	//TCHAR buffer[80] = { 0, };
	//wcsftime(buffer, 80, L"%F:%T", ts);
	//name += buffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)tex.GetAddressOf());

	if (SUCCEEDED(hr))
	{
		WCHAR ext[_MAX_EXT];
		_wsplitpath_s(name.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		GUID fileformat = GUID_ContainerFormatPng;
		T_STR tExt = ext;
		if (tExt == L".dds" || tExt.empty())
		{
			if (tExt.empty())
			{
				hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), (name + L".dds").c_str());
			}
			else
			{
				hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), name.c_str());
			}
		}
		else
		{
			if (tExt == L".bmp") fileformat = GUID_ContainerFormatBmp;
			if (tExt == L".png") fileformat = GUID_ContainerFormatPng;
			if (tExt == L".ico") fileformat = GUID_ContainerFormatIco;
			if (tExt == L".jpg") fileformat = GUID_ContainerFormatJpeg;
			if (tExt == L".tif") fileformat = GUID_ContainerFormatTiff;
			hr = DirectX::SaveWICTextureToFile(pContext, tex.Get(), fileformat, name.c_str());
		}
	}
	return hr;
}

