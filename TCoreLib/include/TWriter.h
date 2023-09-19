#pragma once
#include <wrl.h> 
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <vector>
#include <string>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
using namespace Microsoft::WRL;

struct TTextData
{
	std::wstring text;
	D2D1_RECT_F  layout;
	D2D1::ColorF color = { 0.0f, 0.0f, 0.0f, 1.0f };
};
class TWriter
{
	std::vector< TTextData>   m_TextList;
public:
	void		AddText(std::wstring text,
		float x=0, float y=0,
		D2D1::ColorF color = {0.0f, 0.0f, 0.0f,1.0f});
public:
	ComPtr<ID2D1Factory> m_pD2DFactory = nullptr;	
	ComPtr<IDWriteFactory> m_pDWriteFactory = nullptr;
	ComPtr<IDWriteTextFormat> m_pDefaultTextFormat = nullptr;
	ID2D1RenderTarget* m_pRT = nullptr;
	ID2D1SolidColorBrush* m_pDefaultBrush = nullptr;
public:
	bool Create(IDXGISurface1* pBackBuffer);
	bool Init();
	bool Frame();
	bool PreRender();
	bool Render();
	bool PostRender();
	bool Release();
	bool CrateDXWriteRT(IDXGISurface1* pSurface);
public:
	// resize
	virtual bool  DeleteDxResource();
	virtual bool  CreateDxResource(IDXGISurface1* pBackBuffer);
	static TWriter& GetInstance()
	{
		static TWriter input;
		return input;
	}
private:
	TWriter() {}
};

#define I_Writer TWriter::GetInstance()