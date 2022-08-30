#pragma once
#include "TObject.h"
template<class T>
class TPlayer
{
protected:
	T* m_pImp;
public:	
	T* GetImp() { return m_pImp; }
	virtual void   SetPosition(TVector p, TVector s) {};
	virtual void   SetPosition(TVector2D p, TVector2D s) {};
	virtual bool	Frame(float fDeltaTime, float fGameTime) { return true; };
	virtual bool    Render() { return true; };
	virtual ~TPlayer() {}
};
class TGamePlayer : public TPlayer<TObject>
{
public:
	virtual void   SetPosition(TVector p, TVector s) {
		m_pImp->SetPosition(p, s);
	};
	bool	Frame(float fDeltaTime, float fGameTime) override;
	bool    Render() override;
	TGamePlayer()
	{
		m_pImp = new TObject;
	}
	~TGamePlayer()
	{
		delete m_pImp;
	}
};
class TGamePlayer2D : public TPlayer<TObject2D>
{
public:
	virtual void   SetPosition(TVector2D p, TVector2D s) {
		m_pImp->SetPosition(p, s);
	};
	bool	Frame(float fDeltaTime, float fGameTime) override;
	bool    Render() override;
	TGamePlayer2D()
	{
		m_pImp = new TObject2D;
	}
	~TGamePlayer2D()
	{
		delete m_pImp;
	}
};
