#include "Texture.hpp"

#include "../Graphics/Graphics.hpp"
#include "../Logger.hpp"

#include "../Graphics/WICTextureLoader.hpp"

Texture::Texture(Graphics* graphics_ptr)
	: graphics_ptr(graphics_ptr)
{}

Texture::Texture()
	: graphics_ptr(nullptr)
{}

Texture Texture::from_file(Graphics* graphics_ptr, std::wstring file)
{
	Texture tex = Texture(graphics_ptr);
	tex.graphics_ptr = graphics_ptr;

	HANDLE_IF_FAILED(CreateWICTextureFromFile(
		graphics_ptr->m_device_com.Get(),
		graphics_ptr->m_context_com.Get(),
		file.c_str(),
		tex.texture_com.GetAddressOf(),
		tex.texture_view_com.GetAddressOf(),
		1024u));

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroDesc(SRVDesc);
	tex.texture_view_com->GetDesc(&SRVDesc);
	memset(&SRVDesc, 0, sizeof(SRVDesc));
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	ALERT_IF_FAILED(graphics_ptr->m_device_com->CreateShaderResourceView(
		tex.texture_com.Get(), &SRVDesc, tex.texture_view_com.GetAddressOf()));

	// sampler
	D3D11_SAMPLER_DESC samplerds{};
	ZeroDesc(samplerds);
	samplerds.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerds.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerds.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerds.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerds.MipLODBias = 0.0f;
	samplerds.MaxAnisotropy = 0u;
	samplerds.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerds.BorderColor[0] = samplerds.BorderColor[1] = samplerds.BorderColor[2] = samplerds.BorderColor[3] = 0.0f;
	samplerds.MinLOD = 0.0f;
	samplerds.MaxLOD = 0.0f;

	ALERT_IF_FAILED(graphics_ptr->m_device_com->CreateSamplerState(&samplerds, tex.sampler_com.GetAddressOf()));

	return tex;
}

void Texture::bind() const
{
	if (! graphics_ptr) {
		return;
	}
	if (! texture_view_com) {
		graphics_ptr->m_context_com->PSSetShaderResources(0u, 1u, graphics_ptr->null_texture_view_com.GetAddressOf());
		return;
	}

	graphics_ptr->m_context_com->PSSetSamplers(0u, 1u, sampler_com.GetAddressOf());
	graphics_ptr->m_context_com->PSSetShaderResources(0u, 1u, texture_view_com.GetAddressOf());
}