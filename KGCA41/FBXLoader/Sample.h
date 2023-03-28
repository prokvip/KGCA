#include "TBasisLib.h"
#include "TCamera.h"
#include "map/TMap.h"
#include "TFbxObj.h"

class Sample : public TBasisLib
{
public:
	TMap		m_CustomMap;
	TCamera*	m_pMainCamera;
	vector<std::shared_ptr<TFbxObj>>		m_ObjList;

	TFbxObj		m_Obj;
	std::shared_ptr<TFbxObj>	m_BindPoseObj1;
	std::shared_ptr<TFbxObj>	m_BindPoseObjIdle1;

	std::shared_ptr<TFbxObj>	m_BindPoseObj2;
	std::shared_ptr<TFbxObj>	m_BindPoseObjIdle2;
	std::shared_ptr<TFbxObj>	m_BindPoseObjWalk2;
	std::shared_ptr<TFbxObj>	m_BindPoseObjRun2;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		LoadBindPoseObject();
	bool		LoadUE4PoseObject();
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	void	DisplayErrorBox(const WCHAR* lpszFunction);
	DWORD	LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list);
public:
	Sample(void);
	virtual ~Sample(void);
};