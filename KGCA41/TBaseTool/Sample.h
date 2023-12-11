#include "TStd.h"
#include "TCore.h"
#include "TDebugCamera.h"


class Sample : public TCore
{
	std::shared_ptr<TDebugCamera>  m_pDebugCamera;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool  DeleteDxResource() override;
	bool  CreateDxResource() override;
};

