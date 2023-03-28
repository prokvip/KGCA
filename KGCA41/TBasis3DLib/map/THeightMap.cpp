#include "THeightMap.h"

float THeightMap::GetHeightmap(int row, int col)
{
	return m_fHeightList[row * m_iNumRows + col] * m_MapDesc.fScaleHeight;
}
bool THeightMap::CreateHeightMap(const TCHAR* strHeightMapTex)
{
	HRESULT hr;
	//D3DX11_IMAGE_INFO ImageInfo;
	///*if( FAILED( D3DX11GetImageInfoFromFile( strHeightMapTex,NULL, &ImageInfo,&hr ) ) )
	//{
	//return false;
	//}	*/

	//D3DX11_IMAGE_LOAD_INFO loadInfo;
	//ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	//loadInfo.Usage = D3D11_USAGE_STAGING;
	//loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	////loadInfo.Format = DXGI_FORMAT_FROM_FILE;
	//loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//loadInfo.pSrcInfo = &ImageInfo;

	//ID3D11Resource *pTexture = NULL;
	ID3D11ShaderResourceView* pSRV = nullptr;
	ComPtr<ID3D11Resource> pTexture;
	//DDS_ALPHA_MODE alphaMode;

	size_t maxsize = 0;

	//_In_ D3D11_USAGE usage,
	//_In_ unsigned int bindFlags,
	//_In_ unsigned int cpuAccessFlags,
	//_In_ unsigned int miscFlags,
	//_In_ unsigned int loadFlags,


	if (FAILED(hr = CreateWICTextureFromFileEx(	m_pd3dDevice,
												strHeightMapTex,
												maxsize,
												D3D11_USAGE_STAGING, 
												0, 
												D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 
												0,
												WIC_LOADER_DEFAULT,
												pTexture.GetAddressOf(), nullptr)))
	{
		return false;
	}
	ID3D11Texture2D *pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize(desc.Height*desc.Width);

	if (pTexture2D )
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					m_fHeightList[row * desc.Width + col] = (float)uRed;	/// DWORDÀÌ¹Ç·Î pitch/4	
				}
			}
			m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;

	SAFE_RELEASE(pTexture2D);
	return true;
}
float THeightMap::GetHeightOfVertex(UINT Index)
{
	return m_fHeightList[Index] * m_MapDesc.fScaleHeight;
};
TVector4 THeightMap::GetColorOfVertex(UINT Index)
{
	return TVector4(1, 1, 1, 1.0f);
};
THeightMap::THeightMap(void)
{
}

THeightMap::~THeightMap(void)
{
}
