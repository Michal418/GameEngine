#pragma once
#include <wrl.h>
#include <string>

namespace wrl = Microsoft::WRL;

class Graphics;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class Texture
{
public:
	Texture(Graphics* graphics_ptr);
	Texture();

public:
	static Texture from_file(Graphics* graphics_ptr, std::wstring file);
	void bind() const;

private:
	const Graphics* graphics_ptr;
	wrl::ComPtr<ID3D11Resource> texture_com;
	wrl::ComPtr<ID3D11ShaderResourceView> texture_view_com ;
	wrl::ComPtr<ID3D11SamplerState> sampler_com;
};