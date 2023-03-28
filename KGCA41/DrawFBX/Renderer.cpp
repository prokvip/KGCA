#include "Sample.h"
ComPtr<ID3D11Buffer> Sample::createConstantBuffer(const void* data, UINT size) const
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = static_cast<UINT>(size);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = data;

	ComPtr<ID3D11Buffer> buffer;
	const D3D11_SUBRESOURCE_DATA* bufferDataPtr = data ? &bufferData : nullptr;
	if (FAILED(m_pd3dDevice->CreateBuffer(&desc, bufferDataPtr, &buffer))) {
		throw std::runtime_error("Failed to create constant buffer");
	}
	return buffer;
}
ComputeProgram Sample::createComputeProgram(const ComPtr<ID3DBlob>& csBytecode) const
{
	ComputeProgram program;
	if (FAILED(m_pd3dDevice->CreateComputeShader(csBytecode->GetBufferPointer(), csBytecode->GetBufferSize(), nullptr, &program.computeShader))) {
		throw std::runtime_error("Failed to create compute shader from compiled bytecode");
	}
	return program;
}
ComPtr<ID3DBlob> Sample::compileShader(const std::wstring& filename, const std::string& entryPoint, const std::string& profile)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> shader;
	ComPtr<ID3DBlob> errorBlob;

	std::printf("Compiling HLSL shader: %s [%s]\n", filename.c_str(), entryPoint.c_str());

	if (FAILED(D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), profile.c_str(), flags, 0, &shader, &errorBlob)))
	{
		std::string errorMsg = "Shader compilation failed: " + wtm(filename);
		if (errorBlob) {
			errorMsg += std::string("\n") + static_cast<const char*>(errorBlob->GetBufferPointer());
		}
		throw std::runtime_error(errorMsg);
	}
	return shader;
}


Texture Sample::createTextureCube(UINT width, UINT height, DXGI_FORMAT format, UINT levels) const
{
	Texture texture;
	texture.width = width;
	texture.height = height;
	texture.levels = (levels > 0) ? levels : Utility::numMipmapLevels(width, height);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = levels;
	desc.ArraySize = 6;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	if (levels == 0) {
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (FAILED(m_pd3dDevice->CreateTexture2D(&desc, nullptr, &texture.texture))) {
		throw std::runtime_error("Failed to create cubemap texture");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;
	if (FAILED(m_pd3dDevice->CreateShaderResourceView(texture.texture.Get(), &srvDesc, &texture.srv))) {
		throw std::runtime_error("Failed to create cubemap texture SRV");
	}
	return texture;
}


void Sample::createTextureUAV(Texture& texture, UINT mipSlice) const
{
	assert(texture.texture);

	D3D11_TEXTURE2D_DESC desc;
	texture.texture->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = desc.Format;
	if (desc.ArraySize == 1) {
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = mipSlice;
	}
	else {
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		uavDesc.Texture2DArray.MipSlice = mipSlice;
		uavDesc.Texture2DArray.FirstArraySlice = 0;
		uavDesc.Texture2DArray.ArraySize = desc.ArraySize;
	}

	if (FAILED(m_pd3dDevice->CreateUnorderedAccessView(texture.texture.Get(), &uavDesc, &texture.uav))) {
		throw std::runtime_error("Failed to create texture UAV");
	}
}


Texture Sample::createTexture(UINT width, UINT height, DXGI_FORMAT format, UINT levels) const
{
	Texture texture;
	texture.width = width;
	texture.height = height;
	texture.levels = (levels > 0) ? levels : Utility::numMipmapLevels(width, height);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = levels;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	if (levels == 0) {
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (FAILED(m_pd3dDevice->CreateTexture2D(&desc, nullptr, &texture.texture))) {
		throw std::runtime_error("Failed to create 2D texture");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	if (FAILED(m_pd3dDevice->CreateShaderResourceView(texture.texture.Get(), &srvDesc, &texture.srv))) {
		throw std::runtime_error("Failed to create 2D texture SRV");
	}
	return texture;
}


ComPtr<ID3D11SamplerState> Sample::createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode) const
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = filter;
	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;
	desc.MaxAnisotropy = (filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY : 1;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> samplerState;
	if (FAILED(m_pd3dDevice->CreateSamplerState(&desc, &samplerState))) {
		throw std::runtime_error("Failed to create sampler state");
	}
	return samplerState;
}