#include "Sample.h"
#include "TParser.h"
std::vector<int>			g_EventList;
std::vector<std::thread>	g_ThreadList;
int   g_iThreadInstanceData[MAX_THREAD] = { 0, };
T::TEventHandler			g_Handler;



Sample* g_pSample = nullptr;
void PerSceneRenderDeferredProc(int& parameter)
{
	int iInstance = parameter;
	ID3D11DeviceContext* pd3dDeferredContext = g_pSample->m_pd3dPerSceneDeferredContext[iInstance];
	ID3D11CommandList*& pd3dComandList = g_pSample->m_pd3dPerSceneCommandList[iInstance];
	while (1)
	{
		// Frame() 통지 대기
		int iRet = g_Handler.WaitForSingleObject(iInstance, 1000);
		if (iRet == 0)
		{
			std::cout << "Run! threadID=" << iInstance << std::endl;
		}
		else if (iRet == -1)
		{
			std::cout << "Timeout! threadID=" << iInstance << std::endl;
			continue;
		}
		else
		{
			break; // error
		}

		//..계산.. commandlist 완성
		g_pSample->m_FbxSelectObj.Frame(g_fSecPerFrame, g_fDurationTime);
		g_pSample->DrawFbxObject(pd3dDeferredContext);
		pd3dDeferredContext->FinishCommandList(true, &pd3dComandList);

		g_Handler.ResetEvent(iInstance);

		// Render() 이벤트 통지
		g_Handler.SetEvent(MAX_THREAD + iInstance);
	}
}

int Sample::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
		if (m_pEditBox != nullptr)
		{
			m_pEditBox->MsgProc(msg, wParam, lParam);
			m_pListBox->MsgProc(msg, wParam, lParam);
		}
	}
	return -1;
}
void Sample::SelectFbxObject(TModelScript& model)
{
	m_FbxSelectObj.m_pMeshImp = TBasisFBX::I_ObjectMgrFBX.Load(model.dataList[0].file);
	m_FbxSelectObj.m_pVShader = TBasisFBX::I_ShaderFBX.CreateVertexShader(g_pd3dDevice, model.dataList[1].file, "VS");
	m_FbxSelectObj.m_pPShader = TBasisFBX::I_ShaderFBX.CreatePixelShader(g_pd3dDevice, model.dataList[1].file, "PS");
	m_albedoTexture = TBasisFBX::I_TextureFBX.Load(model.dataList[2].file);
	m_normalTexture = TBasisFBX::I_TextureFBX.Load(model.dataList[3].file);
	m_roughnessTexture = TBasisFBX::I_TextureFBX.Load(model.dataList[4].file);
	m_metalnessTexture = TBasisFBX::I_TextureFBX.Load(model.dataList[5].file);
	if (m_roughnessTexture == nullptr)
	{
		m_roughnessTexture = m_pDefaultWhiteTexture;
	}
	if (m_metalnessTexture == nullptr)
	{
		m_metalnessTexture = m_pDefaultWhiteTexture;
	}
	m_emissiveTexture = TBasisFBX::I_TextureFBX.Load(model.dataList[6].file);;
	if (m_emissiveTexture == nullptr)
	{
		m_emissiveTexture = m_pDefaultBlackTexture;
	}
	for (int iDraw = 0; iDraw < m_FbxSelectObj.m_pMeshImp->m_DrawList.size(); iDraw++)
	{
		m_FbxSelectObj.m_pMeshImp->m_DrawList[iDraw]->m_pContext = m_pImmediateContext;
	}
}
void Sample::CreateWidget()
{	
	m_pText.reset(TUI::CreateText(m_pTBasisUI, { 0,0,150,30 }, L"고딕", 20, { 0,0,0,1 }));
	m_pText->SetText(L"FBX fileList");
	m_pText->SetPriority(2);
	m_pWidgets.push_back(m_pText.get());

	TUI::TSprite* pSprite = TUI::I_Sprite.GetPtr(L"DXEditBox");
	auto format = TUI::I_Writer.m_pTextFormat.Get();
	m_pComboBox.reset(TUI::CreateComboBox(m_pTBasisUI, pSprite, format, { 0,  30, 200, 30 },10,0));


	std::vector<std::wstring>			fileList;
	TBasisFBX::T::LoadAllPath(L"../../data/model/", fileList);

	TParser parser;
	for (auto data : fileList)
	{
		T_STR name=data.substr(data.find_last_of(L"/\\") + 1);
		std::wstring::size_type const p(name.find_last_of(L"."));
		std::wstring filename = name.substr(0, p);
		std::wstring ext = name.substr(p+1, 3);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if(ext == L"txt")
		{
			TUI::TWidget* newdata = m_pComboBox->Push(filename);
			// std::make_pair(filename, m_pComboBox->Push(filename)));
			// TUI::CreateButton(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXNormalButton"), m_pComboBox->GetNextRect()));
			
			TModelScript model;			
			if (parser.OpenStream(data.c_str()))
			{
				TCHAR	m_pBuffer[MAX_PATH] = { 0, };
				TCHAR	m_pString[MAX_PATH] = { 0, };
				TCHAR	m_pData[MAX_PATH] = { 0, };
				while (!feof(parser.m_pStream))
				{
					TScriptSubData subData;
					_fgetts(m_pBuffer, MAX_PATH, parser.m_pStream);
					_stscanf(m_pBuffer, _T("%s%s"), m_pString, m_pData);
					subData.title = m_pString;
					subData.file = m_pData;
					model.dataList.push_back(subData);
				}
				m_fbxList.push_back(model);
				m_fbxFileList.insert(std::make_pair(filename, model));
			}
		}
		
	}
	m_pComboBox->SetPriority(10);
	m_pWidgets.push_back(m_pComboBox.get());

	/*m_pListBox.reset(TUI::CreateListBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXListBox"), 10, 0, { 650,  0, 150, 300.0f }));
	m_pListBox->Push(L"aaa");
	m_pListBox->Push(TUI::CreateComboBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), m_pListBox->GetNextRect()));
	m_pListBox->Push(TUI::CreateButton(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXNormalButton"), m_pListBox->GetNextRect()));
	m_pListBox->Push(TUI::CreateEditBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), m_pListBox->GetNextRect()));
	m_pListBox->Push(L"bbb");
	m_pListBox->Push(L"ccc");
	m_pListBox->Push(L"ddd");
	m_pListBox->Push(L"eee");
	m_pListBox->Push(TUI::CreateSlider(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXSlider"), m_pListBox->GetNextRect()));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 10, { 1,0,0,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 20, { 0,1,0,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 30, { 0,0,1,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 10, { 1,0,1,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 10, { 0,1,1,1 }));
	m_pListBox->SetPriority(3);

	m_pWidgets.push_back(m_pListBox.get());*/
}
bool Sample::Init()
{
	for (int iThread = 0; iThread < MAX_THREAD; iThread++)
	{
		g_iThreadInstanceData[iThread] = iThread;

		m_pd3dDevice->CreateDeferredContext(0, &m_pd3dPerSceneDeferredContext[iThread]);

		g_EventList.push_back(g_Handler.CreateEvent(std::to_string(iThread)));
		g_ThreadList.emplace_back(PerSceneRenderDeferredProc,std::ref(g_iThreadInstanceData[iThread]));
		g_ThreadList[g_ThreadList.size() - 1].detach();
	}
	g_EventList.push_back(g_Handler.CreateEvent("render_0"));
	//g_EventList.push_back(g_Handler.CreateEvent("render_1"));
	//g_EventList.push_back(g_Handler.CreateEvent("render_2"));

	ID3D11UnorderedAccessView* const nullUAV[] = { nullptr };
	ID3D11Buffer* const nullBuffer[] = { nullptr };

	CreateWidget();
	auto start = std::chrono::system_clock::now();
	TBasisFBX::I_TextureFBX.Set(m_pd3dDevice, m_pImmediateContext);
	TBasisFBX::I_ObjectMgrFBX.Set(m_pd3dDevice, m_pImmediateContext);


	m_pDefaultWhiteTexture= TBasisFBX::I_TextureFBX.Load(L"../../data/white.png");
	m_pDefaultBlackTexture= TBasisFBX::I_TextureFBX.Load(L"../../data/black.png");
	//m_envTextureSrc = TBasisFBX::I_TextureFBX.Load(L"../../data/sky/environment.dds");// grassenvmap1024.dds"); 
	m_envTexture = TBasisFBX::I_TextureFBX.Load(L"../../data/sky/grassenvmap1024.dds"); 
	m_defaultSampler = createSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP);
	m_computeSampler = createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//{
	//	// Unfiltered environment cube map (temporary).
	//	Texture envTextureUnfiltered = createTextureCube(1024, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
	//	createTextureUAV(envTextureUnfiltered, 0);

	//	// Load & convert equirectangular environment map to a cubemap texture.
	//	{
	//		ComputeProgram equirectToCubeProgram = createComputeProgram(compileShader(L"shader/equirect2cube.hlsl", "main", "cs_5_0"));
	//		m_pImmediateContext->CSSetShaderResources(0, 1, &m_envTextureSrc->m_pSRV);
	//		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, envTextureUnfiltered.uav.GetAddressOf(), nullptr);
	//		m_pImmediateContext->CSSetSamplers(0, 1, m_computeSampler.GetAddressOf());
	//		m_pImmediateContext->CSSetShader(equirectToCubeProgram.computeShader.Get(), nullptr, 0);
	//		m_pImmediateContext->Dispatch(envTextureUnfiltered.width / 32, envTextureUnfiltered.height / 32, 6);
	//		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
	//	}

	//	m_pImmediateContext->GenerateMips(envTextureUnfiltered.srv.Get());

	//	// Compute pre-filtered specular environment map.
	//	{
	//		struct SpecularMapFilterSettingsCB
	//		{
	//			float roughness;
	//			float padding[3];
	//		};
	//		ComputeProgram spmapProgram = createComputeProgram(compileShader(L"shader/spmap.hlsl", "main", "cs_5_0"));
	//		SpecularMapFilterSettingsCB data;
	//		ComPtr<ID3D11Buffer> spmapCB = createConstantBuffer(&data, sizeof(SpecularMapFilterSettingsCB));

	//		m_envTexture = createTextureCube(1024, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT,5);

	//		// Copy 0th mipmap level into destination environment map.
	//		for (int arraySlice = 0; arraySlice < 6; ++arraySlice) 
	//		{
	//			const UINT subresourceIndex = D3D11CalcSubresource(0, arraySlice, m_envTexture.levels);
	//			m_pImmediateContext->CopySubresourceRegion(m_envTexture.texture.Get(), subresourceIndex, 0, 0, 0, envTextureUnfiltered.texture.Get(), subresourceIndex, nullptr);
	//		}

	//		m_pImmediateContext->CSSetShaderResources(0, 1, envTextureUnfiltered.srv.GetAddressOf());
	//		m_pImmediateContext->CSSetSamplers(0, 1, m_computeSampler.GetAddressOf());
	//		m_pImmediateContext->CSSetShader(spmapProgram.computeShader.Get(), nullptr, 0);

	//		// Pre-filter rest of the mip chain.
	//		const float deltaRoughness = 1.0f / max<float>(float(m_envTexture.levels - 1), 1.0f);
	//		for (UINT level = 1, size = 512; level < m_envTexture.levels; ++level, size /= 2) {
	//			const UINT numGroups = max<UINT>(1, size / 32);
	//			createTextureUAV(m_envTexture, level);

	//			const SpecularMapFilterSettingsCB spmapConstants = { level * deltaRoughness };
	//			m_pImmediateContext->UpdateSubresource(spmapCB.Get(), 0, nullptr, &spmapConstants, 0, 0);

	//			m_pImmediateContext->CSSetConstantBuffers(0, 1, spmapCB.GetAddressOf());
	//			m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, m_envTexture.uav.GetAddressOf(), nullptr);
	//			m_pImmediateContext->Dispatch(numGroups, numGroups, 6);
	//		}
	//		m_pImmediateContext->CSSetConstantBuffers(0, 1, nullBuffer);
	//		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
	//	}
	//}
	
	// Compute diffuse irradiance cubemap.
	{
		ComputeProgram irmapProgram = createComputeProgram(compileShader(L"shader/irmap.hlsl", "main", "cs_5_0"));

		m_irmapTexture = createTextureCube(32, 32, DXGI_FORMAT_R16G16B16A16_FLOAT, 1);
		createTextureUAV(m_irmapTexture, 0);

		//m_pImmediateContext->CSSetShaderResources(0, 1, m_envTexture.srv.GetAddressOf());
		m_pImmediateContext->CSSetShaderResources(0, 1, m_envTexture->m_pSRV.GetAddressOf());
		m_pImmediateContext->CSSetSamplers(0, 1, m_computeSampler.GetAddressOf());
		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, m_irmapTexture.uav.GetAddressOf(), nullptr);
		m_pImmediateContext->CSSetShader(irmapProgram.computeShader.Get(), nullptr, 0);
		m_pImmediateContext->Dispatch(m_irmapTexture.width / 32, m_irmapTexture.height / 32, 6);
		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
	}

	// Compute Cook-Torrance BRDF 2D LUT for split-sum approximation.
	{
		ComputeProgram spBRDFProgram = createComputeProgram(compileShader(L"shader/spbrdf.hlsl", "main", "cs_5_0"));

		m_spBRDF_LUT = createTexture(256, 256, DXGI_FORMAT_R16G16_FLOAT, 1);
		m_spBRDF_Sampler = createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
		createTextureUAV(m_spBRDF_LUT, 0);

		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, m_spBRDF_LUT.uav.GetAddressOf(), nullptr);
		m_pImmediateContext->CSSetShader(spBRDFProgram.computeShader.Get(), nullptr, 0);
		m_pImmediateContext->Dispatch(m_spBRDF_LUT.width / 32, m_spBRDF_LUT.height / 32, 1);
		m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
	}

	m_FbxSelectObj.Init();
	m_FbxSelectObj.m_pd3dDevice = m_pd3dDevice;
	m_FbxSelectObj.m_pContext = m_pImmediateContext;
	SelectFbxObject(m_fbxList[0]);	

	TBasisFBX::TVector3 vPos = { 0,0, 0 };
	m_FbxSelectObj.SetPosition(vPos);
	

	auto end = std::chrono::system_clock::now();
	auto diff = end - start;
	auto ms = std::chrono::duration<double, milli>(diff).count();
	OutputDebugString(std::to_wstring(ms / 1000).c_str());

	m_pMainCamera = std::make_shared<TCamera>();
	m_pMainCamera->SetViewMatrix(TVector3(0, 0.0f, -200.0f), TVector3(0, 0.0f, 0.0f));
	m_pMainCamera->SetProjMatrix(XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 5000.0f);

	return true;
}
bool Sample::Frame()
{
	for (auto widget : m_pWidgets)
	{
		if (widget->Frame())
		{
			m_DrawWidgets.push(widget);
		}
	}

	TUI::TListBox* pListBtn = (TUI::TListBox*)m_pComboBox->m_pChildList[2];
	if (pListBtn->m_pSelectBtn != nullptr)
	{
		auto iter = m_fbxFileList.find(pListBtn->m_pSelectBtn->m_szName);
		if (iter != m_fbxFileList.end())
		{
			SelectFbxObject(iter->second);			
		}
	}

	m_pMainCamera->Frame();
	TMatrix matRotation;
	TVector3 vLight = { 0, 0, -100};
	D3DXMatrixRotationY(&matRotation, 0);// g_fGameTimer);
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&m_vLightDir, &vLight);

	//m_FbxSelectObj.Frame(g_fSecPerFrame, g_fDurationTime);	
	return true;
}
bool Sample::Render()
{	
	MultiThreadRender();

	while (!m_DrawWidgets.empty())
	{
		//TUI::ApplyRS(m_pImmediateContext, TDxState::g_pRSScissorBackCullSolid);
		TUI::TWidget* widget = m_DrawWidgets.top();
		const D3D11_RECT r = {  (LONG)widget->m_rtCollision.x1, (LONG)widget->m_rtCollision.y1, 
								(LONG)widget->m_rtCollision.x2/2, (LONG)widget->m_rtCollision.y2/2 };
		m_pImmediateContext->RSSetScissorRects(1, &r);
		widget->Render();
		m_DrawWidgets.pop();
	}
	return true;
}
bool Sample::Release()
{	
	g_Handler.CloseAllHandles();

	for (int iThread = 0; iThread < MAX_THREAD; iThread++)
	{
		m_pd3dPerSceneDeferredContext[iThread]->Release();
		if (m_pd3dPerSceneCommandList[iThread])
		{
			m_pd3dPerSceneCommandList[iThread]->Release();
		}
	}
	

	for (auto widget : m_pWidgets)
	{
		widget->Release();
	}
	m_pEditBox.reset();
	m_pListBox.reset();
	m_pComboBox.reset();
	m_pText.reset();
	m_Dlg.reset();

	TBasisFBX::I_ObjectMgrFBX.Release();
	TBasisFBX::I_ShaderFBX.Release();
	TBasisFBX::I_TextureFBX.Release();
	return true;
}
void Sample::DrawFbxObject(ID3D11DeviceContext* pd3dDeferredContext)
{
	m_FbxSelectObj.SetDevice(m_pd3dDevice, pd3dDeferredContext);
	for (int iDraw = 0; iDraw < m_FbxSelectObj.m_pMeshImp->m_DrawList.size(); iDraw++)
	{
		m_FbxSelectObj.m_pMeshImp->m_DrawList[iDraw]->m_pContext = pd3dDeferredContext;
	}

	pd3dDeferredContext->OMSetRenderTargets(1, GetRenderTargetViewAddress(), m_DefaultRT.m_pDepthStencilView.Get());
	pd3dDeferredContext->RSSetViewports(1, &m_DefaultRT.m_vp);
	DX::ApplyDSS(pd3dDeferredContext, DX::TDxState::g_pDSSDepthEnable);
	DX::ApplyBS(pd3dDeferredContext, DX::TDxState::g_pAlphaBlend);
	pd3dDeferredContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType);

	//ApplyRS(m_pImmediateContext, TDxState::g_pRSScissorBackCullSolid);
	//const D3D11_RECT r = { m_DefaultRT.m_vp.TopLeftX, m_DefaultRT.m_vp.TopLeftY, m_DefaultRT.m_vp.Width,m_DefaultRT.m_vp.Height };
	//m_pImmediateContext->RSSetScissorRects(1, &r);

	///*UINT oldNumScissorRects = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	//D3D11_RECT oldScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	//m_pImmediateContext->RSGetScissorRects(&oldNumScissorRects, oldScissorRects);*/


	

	TBasisFBX::TMatrix* matView = (TBasisFBX::TMatrix*)m_pMainCamera->GetViewMatrix();
	TBasisFBX::TMatrix* matProj = (TBasisFBX::TMatrix*)m_pMainCamera->GetProjMatrix();

	pd3dDeferredContext->PSSetShaderResources(1, 1, m_normalTexture->m_pSRV.GetAddressOf());
	pd3dDeferredContext->PSSetShaderResources(2, 1, m_metalnessTexture->m_pSRV.GetAddressOf());
	pd3dDeferredContext->PSSetShaderResources(3, 1, m_roughnessTexture->m_pSRV.GetAddressOf());
	pd3dDeferredContext->PSSetShaderResources(4, 1, m_emissiveTexture->m_pSRV.GetAddressOf());
	//pd3dDeferredContext->PSSetShaderResources(5, 1, m_envTexture.srv.GetAddressOf());
	pd3dDeferredContext->PSSetShaderResources(5, 1, m_envTexture->m_pSRV.GetAddressOf());
	pd3dDeferredContext->PSSetShaderResources(6, 1, m_irmapTexture.srv.GetAddressOf());
	pd3dDeferredContext->PSSetShaderResources(7, 1, m_spBRDF_LUT.srv.GetAddressOf());
	pd3dDeferredContext->PSSetSamplers(0, 1, m_defaultSampler.GetAddressOf());
	pd3dDeferredContext->PSSetSamplers(1, 1, m_spBRDF_Sampler.GetAddressOf());

	m_FbxSelectObj.m_CameraInfo.m_vCamera = *((TBasisFBX::TVector3*)&m_pMainCamera->m_vCameraPos);
	m_FbxSelectObj.m_CameraInfo.m_vLook = *((TBasisFBX::TVector3*)&m_pMainCamera->m_vLookVector);

	m_FbxSelectObj.SetMatrix(nullptr, matView, matProj);
	//m_FbxSelectObj.PreRender();	
	//m_FbxSelectObj.Draw();
	for (int iObj = 0; iObj < m_FbxSelectObj.m_pMeshImp->m_DrawList.size(); iObj++)
	{
		TBasisFBX::TFbxModel* pFbxObj = m_FbxSelectObj.m_pMeshImp->m_DrawList[iObj];
		pFbxObj->m_pColorTex = m_albedoTexture;
	}
	//m_pImmediateContext->PSSetShaderResources(0, 1, m_albedoTexture->m_pSRV.GetAddressOf());
	m_FbxSelectObj.Render();
}
void Sample::MultiThreadRender()
{
	// Frame
	for (int iThread = 0; iThread < MAX_THREAD; iThread++)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		g_Handler.SetEvent(iThread);
	}
	//g_pSample->m_FbxSelectObj.Frame(g_fSecPerFrame, g_fDurationTime);
	//g_pSample->DrawFbxObject();

	// Render
	int iEventArray[] = { MAX_THREAD + 0, };
	int iReturn = g_Handler.WaitForMultipleObject(1, iEventArray, true);
	iReturn -= MAX_THREAD;
	
	for (int iThread = 0; iThread < MAX_THREAD; iThread++)
	{
		// commendlist execute->Render
		m_pImmediateContext->ExecuteCommandList(m_pd3dPerSceneCommandList[iThread], true);

		m_pd3dPerSceneCommandList[iThread]->Release();
		m_pd3dPerSceneCommandList[iThread] = nullptr;

		std::cout << "Render! ID=" << iThread << std::endl;
		g_Handler.ResetEvent(iEventArray[iThread]);
	}
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateResource()
{
	HRESULT hr = S_OK;
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::DeleteResource()
{
	HRESULT hr= S_OK;	
	return hr;
}
Sample::Sample(void)
{
	g_pSample = this;
}
Sample::~Sample(void)
{
}

TBASIS_RUN(L"TBasisSample DrawFBX");
