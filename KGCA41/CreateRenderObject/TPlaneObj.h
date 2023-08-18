#pragma once
#include "TObject.h"
class TPlaneObj : public TObject
{
public:
	virtual bool CreateVertexBuffer() override;
	virtual bool    Frame();
};
class TNpc : public TPlaneObj
{
	TVector3 m_vDirection = { 1,1,0 };
public:
	void			Move(float fSecond)
	{
		TVector3 vVelocity = m_vDirection * 500.0f * fSecond;
		m_vPos = m_vPos + vVelocity;

		if (m_vPos.x < -1000.0f)
		{
			m_vDirection.x *= -1.0f;
			m_vPos.x = -1000.0f;
		}
		if (m_vPos.y < -1000.0f)
		{
			m_vDirection.y *= -1.0f;
			m_vPos.y = -1000.0f;
		}
		if (m_vPos.x > 1000.0f)
		{
			m_vDirection.x *= -1.0f;
			m_vPos.x = 1000.0f;
		}
		if (m_vPos.y > 1000.0f)
		{
			m_vDirection.y *= -1.0f;
			m_vPos.y = 1000.0f;
		}

	};
	virtual bool    Frame()
	{
		TMatrix matScale, matRotation, matTranslate;
		matScale.Sclae(m_vScale);
		matRotation.ZRotate(m_vRotation.z);
		matTranslate.Translation(m_vPos);
		m_matWorld = matScale * matRotation * matTranslate;
		return true;
	}
	TNpc()
	{
		m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
	}
};
