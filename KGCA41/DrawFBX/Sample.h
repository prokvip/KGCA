#pragma once
#include "TBasisLib.h"
#include "TFbxObj.h"
#include "TModelViewCamera.h"
#include "TBasisUI.h"
#include "TEditBox.h"
#include "utils.h"

struct TWidgetCmp
{
	bool operator()(TUI::TWidget* a, TUI::TWidget* b) const
	{
		return a->GetPriority() > b->GetPriority();
	}
};

struct ComputeProgram
{
	ComPtr<ID3D11ComputeShader> computeShader;
};

struct Texture
{
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11UnorderedAccessView> uav;
	UINT width, height;
	UINT levels;
};

struct TScriptSubData
{
	std::wstring title;
	std::wstring file;
	float value;
};
struct TModelScript
{
	std::vector<TScriptSubData>  dataList;
};

class Sample : public TBasisLib
{
	TBasisFBX::TTexture* m_pDefaultWhiteTexture = nullptr;
	TBasisFBX::TTexture* m_pDefaultBlackTexture = nullptr;

	TBasisFBX::TTexture* m_albedoTexture=nullptr;
	TBasisFBX::TTexture* m_normalTexture = nullptr;
	TBasisFBX::TTexture* m_metalnessTexture = nullptr;
	TBasisFBX::TTexture* m_roughnessTexture = nullptr;
	TBasisFBX::TTexture* m_emissiveTexture = nullptr;
	TBasisFBX::TTexture* m_envTexture = nullptr;
	//TBasisFBX::TTexture* m_irmapTexture = nullptr;
	//TBasisFBX::TTexture* m_spBRDF_LUT = nullptr;
	TBasisFBX::TTexture* m_envTextureSrc = nullptr;
	Texture m_irmapTexture;
	Texture m_spBRDF_LUT;
	//Texture m_envTexture;
	ComPtr<ID3D11SamplerState> m_defaultSampler;
	ComPtr<ID3D11SamplerState> m_computeSampler;
	ComPtr<ID3D11SamplerState> m_spBRDF_Sampler;
public:	
	TBasisFBX::TFbx						m_FbxSelectObj;
	TVector3 m_vLightDir;
	std::shared_ptr<TCamera>			m_pMainCamera=nullptr;
	std::vector<TModelScript>			m_fbxList;
	std::map<std::wstring, TModelScript> m_fbxFileList;
	void			SelectFbxObject(TModelScript& model);
public:
public:
	std::shared_ptr<TUI::TEditBox>		m_pEditBox;
	std::shared_ptr<TUI::TComboBox>		m_pComboBox;
	std::shared_ptr<TUI::TWidget>		m_pListBox;
	std::shared_ptr<TUI::TText>			m_pText;
	std::shared_ptr<TUI::TWidget>		m_Dlg;
	std::vector<TUI::TWidget*>			m_pWidgets;
	std::priority_queue<TUI::TWidget*, std::vector<TUI::TWidget*>, TWidgetCmp>   m_DrawWidgets;
	void CreateWidget();
public:
	ComputeProgram		createComputeProgram(const ComPtr<ID3DBlob>& csBytecode) const;
	ComPtr<ID3DBlob>	compileShader(const std::wstring& filename, const std::string& entryPoint, const std::string& profile);
	Texture				createTextureCube(UINT width, UINT height, DXGI_FORMAT format, UINT levels=1) const;
	void				createTextureUAV(Texture& texture, UINT mipSlice) const;
	Texture				createTexture(UINT width, UINT height, DXGI_FORMAT format, UINT levels) const;
	ComPtr<ID3D11SamplerState> createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode) const;
	ComPtr<ID3D11Buffer> createConstantBuffer(const void* data, UINT size) const;

public:	
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();	
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample(void);
	virtual ~Sample(void);
};
