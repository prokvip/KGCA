#pragma once
#include "TObject3D.h"
struct TPaticle
{
	bool	 bEnable;
	TVector3 vPos;
	TVector3 vVel;	
	float    fLifeTime;
	float    fTimer;
	TVector4 vColor;
	TPaticle()
	{
		fTimer = 0.0f;
		bEnable = true;
		vPos = TVector3(0, 0, 0);
		vVel = TVector3(randstep(-1.0f, 1.0f),
						randstep(1.0f, 3.0f),
						randstep(-1.0f, 1.0f));
		fLifeTime = randstep(3.0f, 10.0f);
		vColor = TVector4(randstep(0.0f,1.0f),
			randstep(0.0f, 1.0f),
			randstep(0.0f, 1.0f),1);
	}
};
class TParticleObj : public TObject3D
{
public:
	ID3D11GeometryShader* m_pGS;
	UINT			m_iParticleCounter = 1;
	std::vector<TPaticle>  m_Paticles;
public:
	bool  Init() override;
	bool  Frame() override;
	bool  Render() override;
	bool  Release() override;
	virtual void		SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
public:
	virtual void		CreateVertexData() override;
	virtual void		CreateIndexData() override;
	virtual HRESULT		CreateVertexLayout()override;
	virtual bool		CreateShader(std::wstring filename) override;
};

