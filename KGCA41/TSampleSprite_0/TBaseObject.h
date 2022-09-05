#pragma once
#include "TTexture.h"
#include "TShader.h"
#include "TCollision.h"
// p(��ġ)n(�븻)c(�÷�)t(�ؽ�ó��ǥ)
struct SimpleVertex
{
	TVector		p; // ��ġ
	TVector4D	c;
	TVector2D   t;
};
class TBaseObject
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext);
public:
	TTexture* m_pTexture;
	std::vector< SimpleVertex>  m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	TShader* m_pShader;
public:
	HRESULT		CreateVertexBuffer();
	bool		CreateShader(std::wstring filename);
	HRESULT     CreateVertexLayout();
	bool		LoadTexture(std::wstring filename);
public:
	virtual bool		Create(
		ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext* pContext,
		const wchar_t* texName = 0,
		const wchar_t* shaderName=0);
public:
	virtual bool		Init(); // �ʱ�ȭ
	virtual bool		Frame();// �ǽð� ���
	virtual bool		Render();// �ǽð� ������
	virtual bool		Release();// �Ҹ� �� ����
};

