#include "Sample.h"
// 교점 계산
bool Sample::GetIntersection(
				T::TVector3 vStart, T::TVector3 vEnd, 
				T::TVector3 v0, T::TVector3 v1, T::TVector3 v2,
				T::TVector3 vNormal)
{
	T::TVector3 vDirection = vEnd - vStart;
	T::TVector3 v0toStart = v0 - vStart;
	float A = T::D3DXVec3Dot(&vNormal, &vDirection);
	float a = T::D3DXVec3Dot(&vNormal, &v0toStart);
	float t = a / A;
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}
	m_vIntersection = vStart + vDirection * t;
	return true;
}
// 영역에 점 포함 여부 테스트 
bool    Sample::PointInPolygon(T::TVector3 vert, T::TVector3 faceNormal,
	T::TVector3 v0, T::TVector3 v1, T::TVector3 v2)
{
	T::TVector3 e0, e1, e2, iInter, vNormal;
	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	iInter = vert - v0;
	T::D3DXVec3Cross(&vNormal, &e0, &iInter);
	T::D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	iInter = vert - v1;
	T::D3DXVec3Cross(&vNormal, &e1, &iInter);
	T::D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = T::D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	iInter = vert - v2;
	T::D3DXVec3Cross(&vNormal, &e2, &iInter);
	T::D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = T::D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;
	return true;
};
bool Sample::IntersectTriangle(
	const TVector3& orig, const TVector3& dir,
	TVector3& v0, TVector3& v1, TVector3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	TVector3 edge1 = v1 - v0;
	TVector3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	TVector3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	TVector3 tvec; // 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)
		return false;

	// Prepare to test V parameter
	TVector3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	//TVector3 e3 = v2 - v1;
	//TVector3 vi = orig + (*t) * dir;
	//TVector3 i0 = v0 + edge1 * (*u) + edge2 * (*v);
	//
	//this->vPickRayOrig = orig;
	//this->vPickRayDir = dir;
	return true;
}
void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void    Sample::CreateMapObject()
{
	srand(time(NULL));
	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PS");

	m_pBoxObj = new TBoxObj;
	m_pBoxObj->m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");
	m_pBoxObj->m_pVShader = pVShader;
	m_pBoxObj->m_pPShader = pPShader;
	if (!m_pBoxObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return ;
	}

	T::TMatrix matScale;
	T::TMatrix matRotateObj;
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		TMapObject* pObj = new TMapObject;
		for (int iv = 0; iv < 8; iv++)
		{
			pObj->box.vList[iv] = m_pBoxObj->m_BoxCollision.vList[iv];
		}
		pObj->vPos = T::TVector3(
			randstep(m_MapObj.m_BoxCollision.vMin.x, m_MapObj.m_BoxCollision.vMax.x),
			0.0f,
			randstep(m_MapObj.m_BoxCollision.vMin.z, m_MapObj.m_BoxCollision.vMax.z));
		
		T::D3DXMatrixScaling(&matScale, randstep(10.0f, 100.0f),
			randstep(10.0f, 100.0f),
			randstep(10.0f, 100.0f));
		T::D3DXMatrixRotationYawPitchRoll(&matRotateObj,
			cosf(randstep(0.0f, 360.0f)) * XM_PI,
			sinf(randstep(0.0f, 360.0f)) * XM_PI,
			1.0f);
		pObj->matWorld = matScale * matRotateObj;
		pObj->vPos.y = m_MapObj.GetHeight(pObj->vPos.x, pObj->vPos.z);
		pObj->matWorld._41 = pObj->vPos.x;
		pObj->matWorld._42 = pObj->vPos.y;
		pObj->matWorld._43 = pObj->vPos.z;
		pObj->UpdateData();
		pObj->UpdateCollision();
		pObj->pObject = m_pBoxObj;
		m_pObjList.push_back(pObj);
	}
}
bool	Sample::Init()
{	
	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PS");

	m_CameraTopView.CreateViewMatrix(T::TVector3(0, 3000.0f, -1),
									 T::TVector3(0, 0.0f, 0));
	m_CameraTopView.CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 10000.0f);

	m_Camera.Init();
	m_Camera.CreateViewMatrix(  T::TVector3(0, 500.0f, -100.0f),
								T::TVector3(0, 0.0f, 0));
	m_Camera.CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 5000.0f);
	m_Camera.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_Camera.m_pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VSColor");;
	m_Camera.m_pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PSColor");;
	m_Camera.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_Camera.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return false;
	}
	m_PlayerObj.Init();
	m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;
	m_PlayerObj.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_PlayerObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return false;
	}

	m_SkyObj.Init();
	m_SkyObj.SetPosition(T::TVector3(0.0f, 0.0f, 0.0f));
	if (!m_SkyObj.Create(m_pd3dDevice.Get(),
		m_pImmediateContext.Get(),
		L"sky.hlsl",
		L"../../data/sky/LobbyCube.dds"))
	{
		return false;
	}


	m_MapObj.Init();
	m_MapObj.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_MapObj.CreateHeightMap(L"../../data/map/129.jpg");
	TTexture* pTexMap = I_Texture.Load(L"../../data/map/020.bmp");
	m_MapObj.m_pColorTex = pTexMap;	
	m_MapObj.m_pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"map.hlsl", "VS");;
	m_MapObj.m_pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"map.hlsl", "PS");;
	// 정점개수 ( 2n승+1)
	m_MapObj.CreateMap(m_MapObj.m_iNumCols, m_MapObj.m_iNumRows, 20.0f);
	if (!m_MapObj.Create(m_pd3dDevice.Get(),m_pImmediateContext.Get()))
	{
		return false;
	}
	m_Quadtree.DrawDebugInit(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_Quadtree.m_pCamera = &m_Camera;
	m_Quadtree.Build(&m_MapObj, 2);
	CreateMapObject();
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		m_Quadtree.AddObject(m_pObjList[iObj]);
	}	
	return true;
}
bool	Sample::Frame()
{	
	T::TVector2 dir = TInput::Get().GetDelta();
	/*if (TInput::Get().GetKey('A') || TInput::Get().GetKey(VK_LEFT))
	{
		m_PlayerObj.m_vPos -= m_PlayerObj.m_vRight *  g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('D') || TInput::Get().GetKey(VK_RIGHT))
	{
		m_PlayerObj.m_vPos += m_PlayerObj.m_vRight * g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('W') || TInput::Get().GetKey(VK_UP))
	{
		m_PlayerObj.m_vPos += m_PlayerObj.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('S') || TInput::Get().GetKey(VK_DOWN))
	{
		m_PlayerObj.m_vPos -= m_PlayerObj.m_vLook * g_fSecPerFrame * 10.0f;
	}*/
	T::TMatrix matRotate;
	T::TMatrix matScale;

	T::D3DXMatrixRotationY(&matRotate, -dir.y);
	T::D3DXMatrixScaling(&matScale, 50, 50, 50);
	m_PlayerObj.m_matWorld = matScale*matRotate;
	m_PlayerObj.m_vPos.y= m_MapObj.GetHeight(m_PlayerObj.m_vPos.x, m_PlayerObj.m_vPos.z)+50;
	m_PlayerObj.SetPosition(m_PlayerObj.m_vPos);
	m_Camera.m_vTarget = m_PlayerObj.m_vPos;
	float y = m_MapObj.GetHeight(m_Camera.m_vCamera.x, m_Camera.m_vCamera.z);
	/*m_Camera.m_vCamera = m_PlayerObj.m_vPos +
						m_PlayerObj.m_vLook * -1.0f *5.0f +
						m_PlayerObj.m_vUp * 5.0f;*/

	if (TInput::Get().GetKey('A') || TInput::Get().GetKey(VK_LEFT))
	{
		m_Camera.MoveSide(-g_fSecPerFrame * 100.0f);
	}
	if (TInput::Get().GetKey('D') || TInput::Get().GetKey(VK_RIGHT))
	{
		m_Camera.MoveSide(g_fSecPerFrame * 100.0f);
	}
	//m_Camera.MoveLook(10.0f);
	if (TInput::Get().GetKey('W') )
	{
		m_Camera.MoveLook(g_fSecPerFrame * 100.0f);
	}
	if (TInput::Get().GetKey('S') || TInput::Get().GetKey(VK_DOWN))
	{
		m_Camera.MoveLook(-g_fSecPerFrame * 100.0f);
	}
	
	m_Camera.Update(T::TVector4(-dir.x, -dir.y,0,0));
	m_MapObj.Frame();
	m_Quadtree.Update(&m_Camera);
	m_PlayerObj.Frame();

	// 마우스피킹
	if (TInput::Get().m_dwMouseState[1] == KEY_HOLD)
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);
		//((2S_x - 2X)/ Width) - 1
		T::TVector3 vView, vProj; // view
		vProj.x = (((2.0f * ptCursor.x - 2.0f* m_ViewPort.TopLeftX) / m_ViewPort.Width) - 1 ) ;
		vProj.y = -(((2.0f * ptCursor.y - 2.0f * m_ViewPort.TopLeftY) / m_ViewPort.Height) - 1);
		vProj.z = 1.0f;
		vView.x = vProj.x / m_Camera.m_matProj._11;
		vView.y = vProj.y / m_Camera.m_matProj._22;
		vView.z = vProj.z;
		T::TMatrix m;
		T::D3DXMatrixInverse(&m, nullptr, &m_Camera.m_matView);

		// world
		T::TRay ray;		
		ray.direction.x = vView.x*m._11 + vView.y*m._21 + vView.z*m._31;
		ray.direction.y = vView.x*m._12 + vView.y*m._22 + vView.z*m._32;
		ray.direction.z = vView.x*m._13 + vView.y*m._23 + vView.z*m._33;
		ray.position.x = m._41;
		ray.position.y = m._42;
		ray.position.z = m._43;
		T::D3DXVec3Normalize(&ray.direction, &ray.direction);
		T::TVector3 vStart = ray.position; // 교점
		T::TVector3 vEnd = ray.position + ray.direction * m_Camera.m_fFarDistance; // 교점
		/*for (TFace& tFace : m_MapObj.m_FaceList)
		{
			T::TVector3 v0, v1, v2;
			DWORD i0 = tFace.v0;
			DWORD i1 = tFace.v1;
			DWORD i2 = tFace.v2;
			v0 = m_MapObj.m_VertexList[tFace.v0].p;
			v1 = m_MapObj.m_VertexList[tFace.v1].p;
			v2 = m_MapObj.m_VertexList[tFace.v2].p;*/
		for (int iNode = 0; iNode < m_Quadtree.g_pDrawLeafNodes.size(); iNode++)
		{
			TNode* pNode = m_Quadtree.g_pDrawLeafNodes[iNode];

			/*if (TCollision::CheckBoxToRay(pNode->m_Box, ray) == false)
			{
				continue;
			}*/

			for (int i = 0; i < pNode->m_IndexList[0].size(); i += 3)
			{
				T::TVector3 v0, v1, v2;
				DWORD i0 = pNode->m_IndexList[0][i + 0];
				DWORD i1 = pNode->m_IndexList[0][i + 1];
				DWORD i2 = pNode->m_IndexList[0][i + 2];
				v0 = m_MapObj.m_VertexList[i0].p;
				v1 = m_MapObj.m_VertexList[i1].p;
				v2 = m_MapObj.m_VertexList[i2].p;
				// 1)교점 계산
				//if (GetIntersection(vStart, vEnd,v0,v1,v2, tFace.vNomal)==true)
				//{
				//	// 교점이 해당 페이스 안에 있는지 여부 판단
				//	// 점 포함 테스트
				//	if (PointInPolygon(m_vIntersection, tFace.vNomal,
				//										v0, v1, v2)== true)
				//	{			
				//		m_MapObj.m_VertexList[i0].c = T::TVector4(1,0,0,1);
				//		m_MapObj.m_VertexList[i1].c = T::TVector4(1, 0, 0, 1);
				//		m_MapObj.m_VertexList[i2].c = T::TVector4(1, 0, 0, 1);
				//		m_MapObj.m_pContext->UpdateSubresource(
				//			m_MapObj.m_pVertexBuffer, 0, NULL, 
				//			&m_MapObj.m_VertexList.at(0), 0, 0);

				//		DisplayText("\n%10.4f, %10.4f, %10.4f ",
				//			m_vIntersection.x,
				//			m_vIntersection.y,
				//			m_vIntersection.z);
				//		m_vIntersectionList.push_back(m_vIntersection);
				//		//break;
				//	}
				//}

				//2번 교점
				float t, u, v;
				if (IntersectTriangle(ray.position, ray.direction,
					v0, v1, v2, &t, &u, &v))
				{
					m_vIntersection = ray.position + ray.direction * t;
					//m_MapObj.m_VertexList[tFace.v0].c = T::TVector4(1, 0, 0, 1);
					//m_MapObj.m_VertexList[tFace.v1].c = T::TVector4(1, 0, 0, 1);
					//m_MapObj.m_VertexList[tFace.v2].c = T::TVector4(1, 0, 0, 1);
					m_MapObj.m_VertexList[i0].c = T::TVector4(1, 0, 0, 1);
					m_MapObj.m_VertexList[i1].c = T::TVector4(1, 0, 0, 1);
					m_MapObj.m_VertexList[i2].c = T::TVector4(1, 0, 0, 1);
					m_MapObj.m_pContext->UpdateSubresource(
						m_MapObj.m_pVertexBuffer, 0, NULL,
						&m_MapObj.m_VertexList.at(0), 0, 0);

					DisplayText("\n%10.4f, %10.4f, %10.4f ",
						m_vIntersection.x,
						m_vIntersection.y,
						m_vIntersection.z);
					m_vIntersectionList.push_back(m_vIntersection);
				}
			}
		}
	}
	return true;
}
bool	Sample::Render()
{		
	m_SkyObj.SetMatrix(NULL, &m_Camera.m_matView, &m_Camera.m_matProj);	
	m_SkyObj.Render();

	if (m_bWireFrame)
		m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullWireFrame);
	else
		m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);

	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	m_MapObj.SetMatrix(nullptr, &m_Camera.m_matView,&m_Camera.m_matProj);
	//m_MapObj.Render();	
	m_Quadtree.Render();	

	m_PlayerObj.SetMatrix(nullptr, &m_Camera.m_matView,	&m_Camera.m_matProj);
	m_PlayerObj.Render();		
	
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));


	//MiniMapRender();
	return true;
}

bool    Sample::MiniMapRender()
{
	/*for (int iNode = 0; iNode < m_Quadtree.g_pDrawLeafNodes.size(); iNode++)
	{
			m_MapObj.SetMatrix(nullptr, &m_CameraTopView.m_matView,
										&m_CameraTopView.m_matProj);
			m_MapObj.m_ConstantList.Color = T::TVector4(0, 1, 0, 1);
			m_MapObj.m_pContext->UpdateSubresource(
				m_MapObj.m_pConstantBuffer, 0, NULL, &m_MapObj.m_ConstantList, 0, 0);

			m_MapObj.m_pContext->IASetIndexBuffer(
				m_Quadtree.g_pDrawLeafNodes[iNode]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			m_MapObj.m_pContext->DrawIndexed(m_Quadtree.g_pDrawLeafNodes[iNode]->m_IndexList.size(), 0, 0);
			
	}*/
	for (auto obj : m_pObjList)
	{
		obj->pObject->SetMatrix(&obj->matWorld, &m_CameraTopView.m_matView,
				&m_CameraTopView.m_matProj);
		obj->pObject->m_ConstantList.Color = T::TVector4(0, 0, 0, 1);
		obj->pObject->Render();
	}
	/*for (auto pObj : m_pObjList)
	{
		pObj->pObject->SetMatrix(&pObj->matWorld,  &m_Camera.m_matView,
								&m_Camera.m_matProj);
		if (m_Camera.ClassifyOBB(&pObj->box) == TRUE)
		{
			pObj->pObject->Render();
		}
	}*/
	m_Camera.SetMatrix(nullptr, &m_CameraTopView.m_matView,	&m_CameraTopView.m_matProj);
	m_Camera.Render();
	return true;
}
bool	Sample::Release()
{
	m_SkyObj.Release();
	m_MapObj.Release();
	m_PlayerObj.Release();
	m_Camera.Release();

	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		if (m_pObjList[iObj] != nullptr)
		{
			delete m_pObjList[iObj];
			m_pObjList[iObj] = nullptr;
		}
	}
	if (m_pBoxObj != nullptr)
	{
		m_pBoxObj->Release();
		delete m_pBoxObj;
		m_pBoxObj = nullptr;
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();