#include "TWriter.h"
// 종속적 객체 생성
HRESULT TWriter::CreateDXResource()
{
	HRESULT hr = m_d2dRT->CreateSolidColorBrush({ 0,0,0,1 }, 
				&m_pTextColor);
	return true;
}
// 종속적 객체 소멸
HRESULT TWriter::DeleteDXResource()
{
	if (m_pTextColor) m_pTextColor->Release();
	if (m_d2dRT) m_d2dRT->Release();
	return true;
}
bool		TWriter::Set(IDXGISurface1* dxgiSurface)
{
	// 중요 : 디바이스 생성 플래그 지정해야 한다. 
	// UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	// 3d -> 2D 연동
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));
	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	props.dpiX = 96;
	props.dpiY = 96;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	HRESULT hr = m_d2dFactory->CreateDxgiSurfaceRenderTarget(
		dxgiSurface,
		&props,
		&m_d2dRT);

	CreateDXResource();
	return true;
}
bool		TWriter::Init()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&m_pDWriteFactory);
	hr = m_pDWriteFactory->CreateTextFormat(
		L"고딕",//L"Gabriola", //L"고딕",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30,
		L"en-us",//, // L"ko-kr",//
		&m_pTextFormat);

	m_szDefaultText = L"KGCA Game Academy";
	return true;
}
bool		TWriter::Frame() {
	return true;
}
bool		TWriter::Render()
{
	Draw(0, 0, m_szDefaultText, { 1,0,0,1 });
	return true;
}
bool		TWriter::Draw(float x, float y, std::wstring text, D2D1_COLOR_F color)
{
	m_d2dRT->BeginDraw();

	D2D1_RECT_F rt = { x, y,g_rtClient.right,g_rtClient.bottom };
	m_pTextColor->SetColor(color);
	m_pTextColor->SetOpacity(1.0f);
	//m_pTextColor->SetTransform()
	m_d2dRT->DrawText(text.c_str(), text.size(),
		m_pTextFormat, rt, m_pTextColor);

	m_d2dRT->EndDraw();
	return true;
}
bool		TWriter::Release()
{
	if (m_pTextColor) m_pTextColor->Release();	
	if (m_d2dRT) m_d2dRT->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_d2dFactory) m_d2dFactory->Release();
	return true;
}