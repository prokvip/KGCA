#pragma once
#include "TStd.h"
#include "TCollision.h"
#include "TVector.h"
struct SimpleVertex
{
	TVector		p; // 위치
	TVector4D	c;
};
class TBaseObject
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext);
public:
	std::vector< SimpleVertex>  m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;
public:
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateShader();
	HRESULT     CreateVertexLayout();
public:
	virtual bool		Create(); // 초기화
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Frame(float fDeltaTime, float fGameTime) { return true; };
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
public:
	std::string  m_csName;
	float       m_fFriction;
	float       m_fMass;
	float       m_fSpeed;
public:
	TVector    m_vDirection;
	TBox       m_Box;
	TSphere    m_Sphere;
	TVector    m_vAcceleration;
	TVector    m_vVelocity;
	TVector    m_vForces;
public:
	TVector2D   m_vDirection2D;
	TRect       m_rt;
	TCircle     m_Circle;
	TVector2D   m_vAcceleration2D;
	TVector2D   m_vVelocity2D;
	TVector2D   m_vForces2D;
public:
	TBaseObject();
	virtual ~TBaseObject();
};

