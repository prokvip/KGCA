#pragma once
#include "TStd.h"
#include "TPlaneObj.h"

class TScene
{
public:
	virtual bool   Init()
	{
		return true;
	}
	virtual bool   Frame()
	{
		return true;
	}
	virtual bool   Render()
	{
		return true;
	}
	virtual bool   Release()
	{
		return true;
	}
	virtual void  SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
	{

	}

public:
	virtual ~TScene() {}
};

class TSceneTitle : public TScene
{	
	using U_TObject = std::unique_ptr<TObject>;
	U_TObject m_pMapObj = nullptr;
public:
	virtual bool Init() override;
	virtual bool Frame()override;
	virtual bool Render()override;
	virtual bool Release()override;
public:
	virtual ~TSceneTitle() {}
};
class TSceneLobby : public TScene
{
public:
	virtual ~TSceneLobby() {}
};
class TSceneInGame : public TScene
{
public:
	virtual ~TSceneInGame() {}
};
class TSceneResult : public TScene
{
public:
	virtual ~TSceneResult() {}
};
