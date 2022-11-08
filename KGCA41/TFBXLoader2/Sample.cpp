#include "Sample.h"
void Sample::ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext)
{
	// Unbind all objects from the immediate context
	if (pd3dDeviceContext == NULL) return;

	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV = NULL;
	ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	// Shaders
	//pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->HSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->DSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
	//pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	//// IA clear
	//pd3dDeviceContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
	//pd3dDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
	//pd3dDeviceContext->IASetInputLayout(NULL);

	//// Constant buffers
	//pd3dDeviceContext->VSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->HSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->DSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->GSSetConstantBuffers(0, 14, pBuffers);
	//pd3dDeviceContext->PSSetConstantBuffers(0, 14, pBuffers);

	// Resources
	pd3dDeviceContext->VSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->HSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->DSSetShaderResources(0, 16, pSRVs);
	//pd3dDeviceContext->GSSetShaderResources(0, 16, pSRVs);
	pd3dDeviceContext->PSSetShaderResources(0, 16, pSRVs);

	//// Samplers
	//pd3dDeviceContext->VSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->HSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->DSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->GSSetSamplers(0, 16, pSamplers);
	//pd3dDeviceContext->PSSetSamplers(0, 16, pSamplers);

	//// Render targets
	//pd3dDeviceContext->OMSetRenderTargets(8, pRTVs, pDSV);

	//// States
	//FLOAT blendFactor[4] = { 0,0,0,0 };
	//pd3dDeviceContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
	//pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	//pd3dDeviceContext->RSSetState(NULL);
}
bool	Sample::Init()
{
	/*TFbxFile* pFbxLoaderC = new TFbxFile;
	if (pFbxLoaderC->Init())
	{
		pFbxLoaderC->Load("../../data/fbx/MultiCameras.fbx");
	}
	m_fbxList.push_back(pFbxLoaderC);*/

	/*TFbxFile* pFbxLoaderA = new TFbxFile;
	if (pFbxLoaderA->Init())
	{
		if (pFbxLoaderA->Load("../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx"))
		{
			pFbxLoaderA->CreateConstantBuffer(m_pd3dDevice.Get());
		}
	}
	m_fbxList.push_back(pFbxLoaderA);*/

	/*TFbxFile* pFbxLoaderB = new TFbxFile;
	if (pFbxLoaderB->Init())
	{
		pFbxLoaderB->Load("../../data/fbx/sm_rock.fbx");
	}
	m_fbxList.push_back(pFbxLoaderB);*/

	TFbxFile* pFbxLoaderD = new TFbxFile;
	if (pFbxLoaderD->Init())
	{
		if (pFbxLoaderD->Load("../../data/fbx/Man.FBX"))
		{
			pFbxLoaderD->CreateConstantBuffer(m_pd3dDevice.Get());
		}
	}
	m_fbxList.push_back(pFbxLoaderD);

	W_STR szDefaultDir = L"../../data/fbx/";
	std::wstring shaderfilename = L"Skinning.txt";

	for (auto fbx : m_fbxList)
	{
		for (int iObj = 0; iObj < fbx->m_pDrawObjList.size(); iObj++)
		{
			TFbxObject* pObj = fbx->m_pDrawObjList[iObj];
			std::wstring  szLoad = szDefaultDir + pObj->m_szTextureName;
			pObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),	shaderfilename, szLoad);			
		}
	}

	m_pMainCamera = new TCameraDebug;
	m_pMainCamera->CreateViewMatrix(TVector3(0, 0, -100), TVector3(0, 0, 0), TVector3(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 10000.0f, T_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);

	return true;
}
bool	Sample::Frame() 
{
	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	m_pMainCamera->Frame();
	for (auto fbxfile : m_fbxList)
	{
		fbxfile->UpdateFrame(m_pImmediateContext.Get());
	}
	return true;
}
bool	Sample::Render() 
{
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	 
	TVector3 vLight(0, 0, 1);
	TMatrix matRotation;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);
	//vLight = vLight * matRotation;
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&vLight, &vLight);
	for (int iFbxFile=0; iFbxFile < m_fbxList.size(); iFbxFile++)
	{		
		m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_fbxList[iFbxFile]->m_pConstantBufferBone);
		for (int iObj = 0; iObj < m_fbxList[iFbxFile]->m_pDrawObjList.size(); iObj++)
		{
			TFbxObjectSkinning* pObj = m_fbxList[iFbxFile]->m_pDrawObjList[iObj];	

			TMatrix matControlWorld;
			//D3DXMatrixRotationY(&matControlWorld, g_fGameTimer);
			pObj->m_cbData.x = vLight.x;
			pObj->m_cbData.y = vLight.y;
			pObj->m_cbData.z = vLight.z;
			pObj->SetMatrix(&matControlWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			pObj->Render();
		}
	}
	m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);
	return true;
}
bool	Sample::Release() 
{
	for (auto fbx : m_fbxList)
	{
		fbx->Release();
	}
	return true;
}
GAME_RUN(TFBXLoader, 800, 600)