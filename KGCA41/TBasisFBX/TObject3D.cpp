#include "pch.h"
#include "TObject3D.h"
namespace TBasisFBX
{
	void        TObject3D::GenAABB()
	{
		// aabb 
		m_BoxCollision.vMin = TBasisFBX::TVector3(100000, 100000, 100000);
		m_BoxCollision.vMax = TBasisFBX::TVector3(-100000, -100000, -100000);
		for (int i = 0; i < m_VertexList.size(); i++)
		{
			if (m_BoxCollision.vMin.x > m_VertexList[i].p.x)
			{
				m_BoxCollision.vMin.x = m_VertexList[i].p.x;
			}
			if (m_BoxCollision.vMin.y > m_VertexList[i].p.y)
			{
				m_BoxCollision.vMin.y = m_VertexList[i].p.y;
			}
			if (m_BoxCollision.vMin.z > m_VertexList[i].p.z)
			{
				m_BoxCollision.vMin.z = m_VertexList[i].p.z;
			}

			if (m_BoxCollision.vMax.x < m_VertexList[i].p.x)
			{
				m_BoxCollision.vMax.x = m_VertexList[i].p.x;
			}
			if (m_BoxCollision.vMax.y < m_VertexList[i].p.y)
			{
				m_BoxCollision.vMax.y = m_VertexList[i].p.y;
			}
			if (m_BoxCollision.vMax.z < m_VertexList[i].p.z)
			{
				m_BoxCollision.vMax.z = m_VertexList[i].p.z;
			}
		}

		// 4      5
		// 6      7

		// 0     1
		// 2     3
		m_BoxCollision.vList[0] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMin.z);
		m_BoxCollision.vList[1] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMin.z);
		m_BoxCollision.vList[2] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMin.z);
		m_BoxCollision.vList[3] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMin.z);

		m_BoxCollision.vList[4] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMax.z);
		m_BoxCollision.vList[5] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMax.y,
			m_BoxCollision.vMax.z);
		m_BoxCollision.vList[6] = TBasisFBX::TVector3(m_BoxCollision.vMin.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMax.z);
		m_BoxCollision.vList[7] = TBasisFBX::TVector3(m_BoxCollision.vMax.x,
			m_BoxCollision.vMin.y,
			m_BoxCollision.vMax.z);
	}
	void		TObject3D::SetMatrix(TBasisFBX::TMatrix* matWorld,
		TBasisFBX::TMatrix* matView, TBasisFBX::TMatrix* matProj)
	{

		m_ConstantList.matWorld = m_matWorld.Transpose();
		if (matWorld != nullptr)
		{
			m_matWorld = *matWorld;
			m_ConstantList.matWorld = matWorld->Transpose();
		}
		if (matView != nullptr)
		{
			m_matView = *matView;
			m_ConstantList.matView = matView->Transpose();
		}
		if (matProj != nullptr)
		{
			m_matProj = *matProj;
			m_ConstantList.matProj = matProj->Transpose();
		}

		TBasisFBX::D3DXMatrixInverse(&m_ConstantList.matNormal, NULL, &m_matWorld);
		TBasisFBX::D3DXMatrixTranspose(&m_ConstantList.matNormal, &m_ConstantList.matNormal);
		TBasisFBX::D3DXMatrixTranspose(&m_ConstantList.matNormal, &m_ConstantList.matNormal);
		UpdateData();
		UpdateCollision();
	}
	void		TObject3D::UpdateData()
	{
		m_vLight.x = m_matWorld._11;
		m_vLight.y = m_matWorld._12;
		m_vLight.z = m_matWorld._13;
		m_vUp.x = m_matWorld._21;
		m_vUp.y = m_matWorld._22;
		m_vUp.z = m_matWorld._23;
		m_vLook.x = m_matWorld._31;
		m_vLook.y = m_matWorld._32;
		m_vLook.z = m_matWorld._33;

		TBasisFBX::D3DXVec3Normalize(&m_vLight, &m_vLight);
		TBasisFBX::D3DXVec3Normalize(&m_vUp, &m_vUp);
		TBasisFBX::D3DXVec3Normalize(&m_vLook, &m_vLook);
	}
	void		TObject3D::UpdateCollision()
	{
		m_BoxCollision.vAxis[0] = m_vLight;
		m_BoxCollision.vAxis[1] = m_vUp;
		m_BoxCollision.vAxis[2] = m_vLook;


		m_BoxCollision.vMin = TBasisFBX::TVector3(100000, 100000, 100000);
		m_BoxCollision.vMax = TBasisFBX::TVector3(-100000, -100000, -100000);
		for (int iV = 0; iV < 8; iV++)
		{
			TBasisFBX::TVector3 pos;
			TBasisFBX::D3DXVec3TransformCoord(&pos, &m_BoxCollision.vList[iV], &m_matWorld);
			if (m_BoxCollision.vMin.x > pos.x)
			{
				m_BoxCollision.vMin.x = pos.x;
			}
			if (m_BoxCollision.vMin.y > pos.y)
			{
				m_BoxCollision.vMin.y = pos.y;
			}
			if (m_BoxCollision.vMin.z > pos.z)
			{
				m_BoxCollision.vMin.z = pos.z;
			}

			if (m_BoxCollision.vMax.x < pos.x)
			{
				m_BoxCollision.vMax.x = pos.x;
			}
			if (m_BoxCollision.vMax.y < pos.y)
			{
				m_BoxCollision.vMax.y = pos.y;
			}
			if (m_BoxCollision.vMax.z < pos.z)
			{
				m_BoxCollision.vMax.z = pos.z;
			}
		}

	TVector3 vHalf = m_BoxCollision.vMax - m_BoxCollision.vCenter;
		m_BoxCollision.size.x = fabs(TBasisFBX::D3DXVec3Dot(&m_BoxCollision.vAxis[0], &vHalf));
		m_BoxCollision.size.y = fabs(TBasisFBX::D3DXVec3Dot(&m_BoxCollision.vAxis[1], &vHalf));
		m_BoxCollision.size.z = fabs(TBasisFBX::D3DXVec3Dot(&m_BoxCollision.vAxis[2], &vHalf));
		m_BoxCollision.vCenter = (m_BoxCollision.vMin + m_BoxCollision.vMax);
		m_BoxCollision.vCenter /= 2.0f;
	}
	void		TObject3D::AddPosition(TBasisFBX::TVector3 vPos)
	{
		// ������ġ
		m_vPos += vPos;
		//  10,10f    ~     w(50, 10)
		//
		//   h
		SetVertexData();
		SetIndexData();
		if (m_pContext != nullptr)
		{
			m_pContext->UpdateSubresource(
				m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
		}
	}
	/// <summary>
	/// m_rtDraw, m_rtCollision ���ŵȴ�.
	/// </summary>
	/// <param name="vPos"></param>
	void		TObject3D::SetPosition(TBasisFBX::TVector3 vPos)
	{
		m_vPos = vPos;
		m_matWorld._41 = m_vPos.x;
		m_matWorld._42 = m_vPos.y;
		m_matWorld._43 = m_vPos.z;
	}
	bool    TObject3D::SetVertexData()
	{
		return true;
	}
	bool    TObject3D::SetIndexData()
	{
		m_IndexList.clear();
		m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
		m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
		return true;
	}
	void    TObject3D::FadeIn()
	{
		m_fAlpha += m_fSecPerFrame * 0.5f;
		m_fAlpha = min(m_fAlpha, 1.0f);
		if (m_fAlpha >= 1.0f)
		{
			m_bFadeIn = false;
		}
	}
	void    TObject3D::FadeOut()
	{
		m_fAlpha = m_fAlpha - m_fSecPerFrame * 0.5f;
		m_fAlpha = max(m_fAlpha, 0.0f);
		if (m_fAlpha <= 0.0f)
		{
			m_bFadeOut = false;
		}
	}
	bool	TObject3D::Frame(float fSecperFrame, float fGameTimer)
	{
		m_fSecPerFrame = fSecperFrame;
		m_fGameTimer = fGameTimer;

		if (m_bFadeIn)	FadeIn();
		if (m_bFadeOut)	FadeOut();

		/*m_LightConstantList.vLightDir.x = 0.0f;
		m_LightConstantList.vLightDir.y = 1.0f;
		m_LightConstantList.vLightDir.z = 0.0f;
		m_LightConstantList.vLightDir.w = 1.0f;*/

		m_ConstantList.Color = m_vColor;
		m_ConstantList.Timer = TBasisFBX::TVector4(
			fGameTimer,
			0,
			0,
			1.0f);
		return true;
	}
	TObject3D::TObject3D()
	{
		m_fAlpha = 1.0f;
		m_vColor = TBasisFBX::TVector4(1, 1, 1, 1);
		m_vLight.x = 1;
		m_vLight.y = 0;
		m_vLight.z = 0;
		m_vUp.x = 0;
		m_vUp.y = 1;
		m_vUp.z = 0;
		m_vLook.x = 0;
		m_vLook.y = 0;
		m_vLook.z = 1;

		m_BoxCollision.vAxis[0] = TBasisFBX::TVector3(1, 0, 0);
		m_BoxCollision.vAxis[1] = TBasisFBX::TVector3(0, 1, 0);
		m_BoxCollision.vAxis[2] = TBasisFBX::TVector3(0, 0, 1);
		m_BoxCollision.size.x = 1.0f;
		m_BoxCollision.size.y = 1.0f;
		m_BoxCollision.size.z = 1.0f;
		m_BoxCollision.vMin = TBasisFBX::TVector3(-1.0f, -1.0f, -1.0f);
		m_BoxCollision.vMax = TBasisFBX::TVector3(1.0f, 1.0f, 1.0f);
	}
	TObject3D::~TObject3D()
	{

	}
};