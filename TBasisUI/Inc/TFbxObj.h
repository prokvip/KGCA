#pragma once
#include "TObjectMgr.h"
namespace TBasisFBX
{
	struct TCameraInfo
	{
		TBasisFBX::TVector3 m_vLook;
		TBasisFBX::TVector3 m_vCamera;
	};

	class TBASISFBX_API TFbx :public TObject3D
	{
	public:
		TCameraInfo m_CameraInfo;
		UINT     m_iShadowID = 0;
		TVector4 m_vShadowColor;
	public:
		TFbxImporter* m_pMeshImp;
		TFbxImporter* m_pAnimImporter;
		float m_fDir = 1.0f;
		float m_fTime = 0.0f;
		float m_fSpeed = 1.0f;
		TBoneWorld			m_matBoneArray;
		std::vector<TFbx>	m_DrawList;
	public:
		virtual bool	Init()override;
		virtual bool	Frame(float fSecperFrame, float fGameTimer)override;
		virtual bool	Render()override;
		virtual bool    Release() override;
		void			GenAABB()override;
	public:
		bool	RenderShadow(TShader* pShader);
		TBasisFBX::TMatrix Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fFrame);
	};
};