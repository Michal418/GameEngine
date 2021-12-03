#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi.h>
#include <D3DCompiler.h>
#include <wincodec.h>
#include <map>


#include "Vertex.h"
#include "../Logger.h"
#include "../Scene/Scene.h"
#include "../Graphics/Texture.h"
#include "../Tilemap.h"

#define ZeroDesc(d) ZeroMemory(&d, sizeof(d))

class VertexBuffer;

namespace wrl = Microsoft::WRL;

struct ConstantBuffer
{
	struct
	{
		float x, y;
	} size;
	struct
	{
		float x, y;
	} position;
};

class Graphics
{
public:
	Graphics(HWND, Scene&);

private:
	void init();
	void create_resources();
	void update_resources();
	void render();
	void draw_entity(std::shared_ptr<Entity>);
	void clear_buffer();

	void draw_tilemap(Vertex* verts, int size);

	void make_quad(Entity&, std::vector<Vertex>&);
	void make_vertex_list(std::vector<Vertex>&);
	void make_triangle_list(std::vector<Vertex>&);

public:
	void update();

private:
	HWND m_hwnd;
	Scene& m_scene;

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	std::map<std::wstring, Texture> textures;
	std::vector<Vertex> quad = std::vector<Vertex>(6u);
	std::vector<Vertex> verticies;
	ConstantBuffer const_buffer;

	wrl::ComPtr<ID3DBlob> blob_com = nullptr;
	wrl::ComPtr<ID3D11PixelShader> pshader_com = nullptr;
	wrl::ComPtr<ID3D11VertexShader> vshader_com = nullptr;
	wrl::ComPtr<ID3D11Buffer> cbuffer_com = nullptr;
	wrl::ComPtr<ID3D11Buffer> vbuffer_com = nullptr;
	wrl::ComPtr<ID3D11InputLayout> input_layout_com = nullptr;
	wrl::ComPtr<ID3D11ShaderResourceView> null_texture_view_com = nullptr;

	wrl::ComPtr<IDXGIFactory2> m_factory_com = nullptr;
	wrl::ComPtr<ID3D11Device1> m_device_com = nullptr;
	wrl::ComPtr<IDXGISwapChain2> m_swap_com = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> m_context_com = nullptr;
	wrl::ComPtr<ID3D11Resource> m_bbuffer_com = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> m_target_com = nullptr;

	friend class Texture;
};