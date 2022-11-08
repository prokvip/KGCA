#include "TFbxFile.h"
HRESULT	TFbxFile::CreateConstantBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	for (int iBone = 0; iBone < 255; iBone++)
	{
		D3DXMatrixIdentity(&m_cbDataBone.matBone[iBone]);
	}

	D3D11_BUFFER_DESC       bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CONSTANT_BONE_BUFFER) * 1; // ����Ʈ �뷮
	// GPU �޸𸮿� �Ҵ�
	bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA  sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_cbDataBone;
	hr = pDevice->CreateBuffer(
		&bd, // ���� �Ҵ�
		&sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
		&m_pConstantBufferBone);
	return hr;
}
bool TFbxFile::UpdateFrame(ID3D11DeviceContext* pContext)
{
	m_fAnimFrame = m_fAnimFrame +
		g_fSecondPerFrame * m_fAnimSpeed *	m_AnimScene.fFrameSpeed * m_fAnimInverse;
	if (m_fAnimFrame > m_AnimScene.iEndFrame ||
		m_fAnimFrame < m_AnimScene.iStartFrame)
	{
		m_fAnimFrame = min(m_fAnimFrame, m_AnimScene.iEndFrame);
		m_fAnimFrame = max(m_fAnimFrame, m_AnimScene.iStartFrame);
		m_fAnimInverse *= -1.0f;
	}

	for (int iBone = 0; iBone < m_pObjectList.size(); iBone++)
	{
		TMatrix matAnim2 = m_pObjectList[iBone]->Interplate(m_fAnimFrame, m_AnimScene);
		if (m_pObjectList[iBone]->m_dxMatrixBindPseMap.size())
		{
			auto iter = m_pObjectList[iBone]->m_dxMatrixBindPseMap.find(iBone);
			if (iter != m_pObjectList[iBone]->m_dxMatrixBindPseMap.end())
			{
				TMatrix matBind = iter->second;
				TMatrix matAnim = matBind *matAnim2;
				D3DXMatrixTranspose(&m_cbDataBone.matBone[iBone], &matAnim);
			}
		}
	}
	pContext->UpdateSubresource(m_pConstantBufferBone, 0, nullptr,&m_cbDataBone, 0, 0);
	return true;
}
bool TFbxFile::Render()
{
	for (auto obj : m_pDrawObjList)
	{
		obj->Render();
	}
	return true;
}
