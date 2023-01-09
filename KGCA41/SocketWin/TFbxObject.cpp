#include "TFbxObject.h"
void	TFbxObject::CreateVertexData()
{
}
HRESULT	TFbxObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	if (vbDataList.size() > 0)
	{
		m_pSubVB.resize(vbDataList.size());
		for (int ivb = 0; ivb < vbDataList.size(); ivb++)
		{
			if (vbDataList[ivb].size() > 0)
			{
				m_pSubVB[ivb] =
					TDX::CreateVertexBuffer(m_pd3dDevice,
						&vbDataList[ivb].at(0),
						vbDataList[ivb].size(),
						sizeof(PNCT_VERTEX));
			}
		}
	}
	else
	{
		hr = TObject3D::CreateVertexBuffer();
	}
	return hr;
}
HRESULT TFbxObject::CreateIndexBuffer()
{
	return S_OK;
}
bool    TFbxObject::LoadTexture(W_STR texturename)
{
	if (vbTexList.size() > 0)
	{
		W_STR szDefaultDir = L"../../data/fbx/";
		m_pSubTexture.resize(vbTexList.size());
		for (int ivb = 0; ivb < vbTexList.size(); ivb++)
		{
			W_STR szLoadFile = szDefaultDir + vbTexList[ivb];
			m_pSubTexture[ivb] = I_Tex.Load(szLoadFile);
		}
	}
	else
	{
		m_pTexture = I_Tex.Load(texturename);
		if (m_pTexture != nullptr)
		{
			m_pTextureSRV = m_pTexture->m_pTextureSRV;
			return true;
		}
	}
	return false;
}
bool	TFbxObject::PostRender()
{
	if (m_pIndexBuffer == nullptr)
	{
		if (vbDataList.size() > 0)
		{
			for (int iSubObj = 0; iSubObj < m_pSubVB.size(); iSubObj++)
			{
				if (vbDataList[iSubObj].size() <= 0) continue;
				UINT stride = sizeof(PNCT_VERTEX); // 정점1개의 바이트용량
				UINT offset = 0; // 정점버퍼에서 출발지점(바이트)
				//SLOT(레지스터리)
				m_pImmediateContext->IASetVertexBuffers(0, 1,
					&m_pSubVB[iSubObj], &stride, &offset);
				if (m_pSubTexture[iSubObj] != nullptr)
				{
					m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSubTexture[iSubObj]->m_pTextureSRV);
				}
				m_pImmediateContext->Draw(vbDataList[iSubObj].size(), 0);
			}
		}
		else
		{
			m_pImmediateContext->Draw(m_VertexList.size(), 0);
		}
	}
	else
	{
		m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0);
	}
	return true;
}
bool	TFbxObject::Release()
{
	TObject3D::Release();

	for (int iSubObj = 0; iSubObj < m_pSubVB.size(); iSubObj++)
	{
		if (m_pSubVB[iSubObj])
		{
			m_pSubVB[iSubObj]->Release();
		}
	}
	return true;
}
TBASIS_EX::TMatrix TFbxObject::Interplate(float fFrame)
{
	//   10                20
	//   A=0 --------------- B=20
	//   t=0 ~ t=0.5f     t=1
	TAnimTrack A, B;
	A = m_AnimTracks[max(m_AnimScene.iStartFrame, fFrame+0)];
	B = m_AnimTracks[min(m_AnimScene.iEndFrame, fFrame+1)];
	if (A.iFrame == B.iFrame)
	{
		return m_AnimTracks[fFrame].matAnim;
	}
	float t = (fFrame - A.iFrame)*(B.iFrame-A.iFrame);
	TBASIS_EX::TVector3 pos;
	TBASIS_EX::D3DXVec3Lerp(&pos, &A.t, &B.t, t);
	TBASIS_EX::TVector3 scale;
	TBASIS_EX::D3DXVec3Lerp(&scale, &A.s, &B.s, t);
	TBASIS_EX::TQuaternion qRotation;
	TBASIS_EX::D3DXQuaternionSlerp(&qRotation, &A.r, &B.r, t);

	TBASIS_EX::TMatrix matScale;
	//TBASIS_EX::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::D3DXMatrixRotationQuaternion(&matRotation, &qRotation);

	TBASIS_EX::TMatrix  matCurrent = /*matScale **/ matRotation;
	matCurrent._41 = pos.x;
	matCurrent._42 = pos.y;
	matCurrent._43 = pos.z;
	return matCurrent;
	//return m_AnimTracks[fFrame].matAnim;
}