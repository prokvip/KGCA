#pragma once
#include "TStd.h"
class TShader
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;

	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*  m_pPS;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_pHS;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>	m_pDS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGS;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_pCS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;
public:
	bool	Init();
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext);
	bool	Release();

	HRESULT Load(
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pImmediateContext,
		std::wstring name);
};

