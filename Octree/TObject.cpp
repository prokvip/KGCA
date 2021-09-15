#include "TObject.h"
TObject::TObject()
{
	m_iNodeIndex = -1;
	m_fMass = 1.0f;
	m_vPos = { 20.0f,20,-20 };
	m_vVelocity = { 0.0f,0.0f,0.0f };
	m_vAcceleration = { 0.0f,0.0f,0.0f };
	m_vForces = { 0.0f,0.0f,0.0f };

	m_tRect.vSize = { 4.0f, 4.0f, 4.0f };
	TVector3 vHalf = m_tRect.vSize / 2.0f;
	m_tRect.vPivot = { m_vPos.x - vHalf.x, m_vPos.y - vHalf.y, m_vPos.z + vHalf.z };
	m_tRect.vMin = { m_vPos.x - vHalf.x, m_vPos.y - vHalf.y, m_vPos.z - vHalf.z };
	m_tRect.vMax = { m_vPos.x + vHalf.x, m_vPos.y + vHalf.y, m_vPos.z + vHalf.z };	
}

TObject::TObject(TVector3 vPos, TVector3 vSize)
{
	m_iNodeIndex = -1;
	m_fMass = 1.0f;
	m_vPos = vPos;
	m_vVelocity = { 0.0f,0.0f,0.0f };
	m_vAcceleration = { 0.0f,0.0f,0.0f };
	m_vForces = { 0.0f,0.0f,0.0f };
	
	TVector3 vHalf = vSize / 2.0f;
	m_tRect.vPivot = { m_vPos.x - vHalf.x, m_vPos.y - vHalf.y, m_vPos.z + vHalf.z };
	m_tRect.vMin = { m_vPos.x - vHalf.x, m_vPos.y - vHalf.y, m_vPos.z - vHalf.z };
	m_tRect.vMax = { m_vPos.x + vHalf.x, m_vPos.y + vHalf.y, m_vPos.z + vHalf.z };
	m_tRect.vSize = vSize;
}