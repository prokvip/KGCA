#pragma once
#include "TBasisStd.h"

struct TLightInfo
{
	TVector3				m_vPosition;	// ����Ʈ�� ��ġ
	TVector3				m_vDirection;	// ����Ʈ�� ����
	float					m_fRange;		// ������ ��ȿ �Ÿ�
	float					m_fFalloff;		// ���� �ܰ� �ܺ� ���� �ֵ�( Luminance )
	float					m_fTheta;		// ���� ���� ���� ����		
	float					m_fPhi;			// �ܺ� ���� ���� ����
	TVector4				m_DiffuseColor;
};

class TLightObj : public TLightInfo
{
public:
	TMatrix				m_matInitWorld;
	TMatrix				m_matWorld;
	TVector3				m_vInitPosition;// �ʱ� ����Ʈ�� ��ġ��
	TVector3				m_vScale;		// ����Ʈ ������Ʈ�� ũ�⺯ȯ ��
	TVector3				m_vRotation;    // ����Ʈ ������Ʈ�� ȸ��(xyz)
	TVector3				m_vOffset;		// ���� ���̿��� �߰��� ��ġ(x,y,z)
public:
	T_STR		m_szName;
	T_STR		m_szPath;
public:
	HRESULT				Load(ID3D11Device* pDevice, const TCHAR* strFilePath);
	void				SetPath(TCHAR* pPath);
	void					SetValue(TVector3* pvPos = NULL,
		TVector3* pvDir = NULL,
		float* pfRange = NULL,
		TVector4* pvColor = NULL,
		TVector3* pvScale = NULL,
		TVector3* pvRotation = NULL,
		float fTheta = XM_PI / 4.0f,
		float fPhi = 5.0f);
public:
	bool					Init();
	bool					Frame(float fElapaseTime = 0.0f, float fSecondPerFrame = 0.0f);
	bool					Update(TMatrix* pmatLightWorld);
	bool					Render();
	bool					Release();
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	TLightObj(void);
	~TLightObj(void);
};
class TLightMgr : public TSingleton < TLightMgr >
{
private:
	friend class TSingleton<TLightMgr>;
	ID3D11Device*			m_pd3dDevice;
public:
	typedef map <INT, TLightObj*>					TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;


public:
	void			SetDevice(ID3D11Device*	 pDevice);

	//TVector3 vPosition( 0.0f, 10.0f, 0.0f );
	//TVector3 vDir( 0.0f, -1.0f, 0.0f );
	INT				Add(TVector3,
		TVector3,
		const TCHAR *pFileName = _T("NULL"));

	TLightObj* const		GetPtr(INT iIndex);
	TLightObj* const		GetPtr(T_STR strFindName);
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
	bool			Update(TMatrix* pmatLightWorld);
public:
	TLightMgr();
	~TLightMgr();
};
#define I_Light	 TLightMgr::GetInstance()