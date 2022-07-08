#include "Graphics.hpp"
#include "Window.hpp"

#include "WICTextureLoader.hpp"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


#include <iostream>


Graphics::Graphics(HWND hwnd, Scene& scene)
	: m_hwnd(hwnd), m_scene(scene)
{
	init();
}

void Graphics::init()
{
	std::cout << "Graphics::init\n";

	UINT flags = 0u;
#ifdef DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HANDLE_IF_FAILED(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		0u,
		D3D11_SDK_VERSION,
		(ID3D11Device**)m_device_com.GetAddressOf(),
		NULL,
		&m_context_com
	));

	HANDLE_IF_FAILED(CreateDXGIFactory2(0u, __uuidof(IDXGIFactory2), &m_factory_com));

	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width = static_cast<UINT>(m_scene.camera.size.x * m_scene.camera.pixel_size);
	sd.Height = static_cast<UINT>(m_scene.camera.size.y * m_scene.camera.pixel_size);
	sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Stereo = FALSE;
	sd.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Scaling = DXGI_SCALING_STRETCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	sd.Flags = 0u;

	HANDLE_IF_FAILED(m_factory_com->CreateSwapChainForHwnd(
		(IUnknown*)m_device_com.Get(),
		m_hwnd,
		&sd,
		NULL, NULL,
		(IDXGISwapChain1**)m_swap_com.GetAddressOf()));

	//m_swap_com->SetSourceSize(m_scene.camera.size.x, m_scene.camera.size.y);
	//m_swap_com->ResizeBuffers(2, m_scene.camera.size.x, m_scene.camera.size.y, sd.Format, sd.Flags);

	HANDLE_IF_FAILED(m_swap_com->GetBuffer(0, __uuidof(ID3D11Resource), &m_bbuffer_com));

	HANDLE_IF_FAILED(m_device_com->CreateRenderTargetView(m_bbuffer_com.Get(), nullptr, &m_target_com));

	create_resources();

	textures.insert(std::pair<std::wstring, Texture>(
		m_scene.tilemap.sprite.path,
		Texture::from_file(this, m_scene.tilemap.sprite.path)));
}

void Graphics::create_resources()
{
	std::cout << "Graphics::create_resources\n";

	for (std::shared_ptr<Entity> e : *m_scene.m_entity_collection_ptr)
	{
		if (e->sprite.path.size() == 0)
			continue;

		textures.insert(std::pair<std::wstring, Texture>(
			e->sprite.path,
			Texture::from_file(this, e->sprite.path)
		));
	}

	// viewport

	D3D11_VIEWPORT vp{};
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = m_scene.camera.size.x * m_scene.camera.pixel_size;
	vp.Height = m_scene.camera.size.y * m_scene.camera.pixel_size;
	m_context_com->RSSetViewports(1u, &vp);


	// vertex buffer

	make_vertex_list(verticies);
	D3D11_BUFFER_DESC bd{};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = stride;
	bd.ByteWidth = verticies.size() * stride;
	D3D11_SUBRESOURCE_DATA suda{};
	suda.pSysMem = verticies.data();
	suda.SysMemPitch = 0u;
	suda.SysMemSlicePitch = 0u;
	HANDLE_IF_FAILED(m_device_com->CreateBuffer(&bd, &suda, vbuffer_com.GetAddressOf()));


	// pixel shadedr

	HANDLE_IF_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &blob_com));
	HANDLE_IF_FAILED(m_device_com->CreatePixelShader(
		blob_com->GetBufferPointer(),
		blob_com->GetBufferSize(),
		nullptr,
		&pshader_com
	));
	m_context_com->PSSetShader(pshader_com.Get(), nullptr, 0u);


	// vertex shader

	HANDLE_IF_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &blob_com));
	HANDLE_IF_FAILED(m_device_com->CreateVertexShader(
		blob_com->GetBufferPointer(),
		blob_com->GetBufferSize(),
		nullptr,
		&vshader_com
	));
	m_context_com->VSSetShader(vshader_com.Get(), nullptr, 0u);


	// const buffer

	D3D11_BUFFER_DESC cbd{};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;
	const_buffer =
	{
		{ m_scene.camera.size.x, m_scene.camera.size.y },
		{ m_scene.camera.position.x, m_scene.camera.position.y }
	};
	cbd.ByteWidth = sizeof(const_buffer);
	D3D11_SUBRESOURCE_DATA csd{};
	csd.pSysMem = &const_buffer;
	HANDLE_IF_FAILED(m_device_com->CreateBuffer(&cbd, &csd, &cbuffer_com));
	m_context_com->VSSetConstantBuffers(0u, 1u, cbuffer_com.GetAddressOf());


	// input layout

	const D3D11_INPUT_ELEMENT_DESC ied[]
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,							D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HANDLE_IF_FAILED(m_device_com->CreateInputLayout(
		ied,
		std::size(ied),
		blob_com->GetBufferPointer(),
		blob_com->GetBufferSize(),
		&input_layout_com
	));
	m_context_com->IASetInputLayout(input_layout_com.Get());

	//m_context_com->OMSetRenderTargets(1u, m_target_com.GetAddressOf(), nullptr);
	m_context_com->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void Graphics::update_resources()
{
	std::cout << "Graphics::update_resources\n";

	D3D11_MAPPED_SUBRESOURCE resource{};

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* blendState;
	HANDLE_IF_FAILED(m_device_com->CreateBlendState(&blendDesc, &blendState))
	m_context_com->OMSetBlendState(blendState, NULL, 0xffffffff);

	// update constant buffer

	const_buffer.position.x = m_scene.camera.position.x;
	const_buffer.position.y = m_scene.camera.position.y;
	resource = {};
	m_context_com->Map(cbuffer_com.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &const_buffer, sizeof(const_buffer));
	m_context_com->Unmap(cbuffer_com.Get(), 0);
	m_context_com->VSSetConstantBuffers(0u, 1u, cbuffer_com.GetAddressOf());
}

void Graphics::render()
{
	std::cout << "Graphics::render\n";

	m_context_com->Draw(verticies.size(), 0u);
}

void Graphics::clear_buffer()
{
	std::cout << "Graphics::clear_buffer\n";

	const float color[] = { 0.19f, 0.16f, 0.25f, 1.0f };
	m_context_com->ClearRenderTargetView(m_target_com.Get(), color);
}

void Graphics::draw_tilemap(Vertex* verts, int size)
{
	std::cout << "Graphics::draw_tilemap\n";

	textures[m_scene.tilemap.sprite.path].bind();

	m_context_com->OMSetRenderTargets(1u, m_target_com.GetAddressOf(), nullptr);


	D3D11_MAPPED_SUBRESOURCE resource{};
	m_context_com->Map(vbuffer_com.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, verts, size * stride);
	m_context_com->Unmap(vbuffer_com.Get(), 0);
	m_context_com->IASetVertexBuffers(0u, 1u, vbuffer_com.GetAddressOf(), &stride, &offset);

	m_context_com->Draw(size, 0u);
}

void Graphics::draw_entity(std::shared_ptr<Entity> entity)
{
	std::cout << "Graphics::draw_entity\n";

	textures[entity->sprite.path].bind();

	m_context_com->OMSetRenderTargets(1u, m_target_com.GetAddressOf(), nullptr);

	make_quad(*entity, quad);

	D3D11_MAPPED_SUBRESOURCE resource{};
	m_context_com->Map(vbuffer_com.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, quad.data(), quad.size() * stride);
	m_context_com->Unmap(vbuffer_com.Get(), 0);
	m_context_com->IASetVertexBuffers(0u, 1u, vbuffer_com.GetAddressOf(), &stride, &offset);

	m_context_com->Draw(quad.size(), 0u);
}

void Graphics::make_triangle_list(std::vector<Vertex>& list)
{
	std::cout << "Graphics::make_triangle_list\n";

	UINT size = m_scene.m_entity_collection_ptr->count() * 6u;
	if (list.size() != size)
		list.resize(size);

	int i = 0;
	for (auto& e : *m_scene.m_entity_collection_ptr)
	{
		list[i++] = { e->left(), e->top(), 1, 0 };
		list[i++] = { e->right(), e->top(), 0, 0 };
		list[i++] = { e->right(), e->bottom(), 0, 1};

		list[i++] = { e->left(), e->top(), 1, 0 };
		list[i++] = { e->right(), e->bottom(), 0, 1 };
		list[i++] = { e->left(), e->bottom(), 1, 1 };
	}
}

void Graphics::make_quad(Entity& entity, std::vector<Vertex>& quad)
{
	std::cout << "Graphics::make_quad\n";

	if (quad.size() != 6u)
		quad.resize(6u);

	quad[0] = { entity.left(), entity.top(), 1, 0 };
	quad[1] = { entity.right(), entity.top(), 0, 0 };
	quad[2] = { entity.right(), entity.bottom(), 0, 1 };

	quad[3] = { entity.left(), entity.top(), 1, 0 };
	quad[4] = { entity.right(), entity.bottom(), 0, 1 };
	quad[5] = { entity.left(), entity.bottom(), 1, 1 };

	/*for (auto& v : quad)
	{
		v.x -= fmodf(v.x, 0.5f) - 0.5f;
		v.y -= fmodf(v.y, 0.5f) - 0.5f;
	}*/
}

void Graphics::make_vertex_list(std::vector<Vertex>& list)
{
	std::cout << "Graphics::make_vertex_list\n";

	UINT size = m_scene.m_entity_collection_ptr->count() * 8;
	if (list.size() != size)
		list.resize(size);

	int i = 0;
	for (auto& e : *m_scene.m_entity_collection_ptr)
	{
		list[i++] = { e->left(), e->top() };
		list[i++] = { e->right(), e->top() };

		list[i++] = { e->right(), e->top() };
		list[i++] = { e->right(), e->bottom() };

		list[i++] = { e->right(), e->bottom() };
		list[i++] = { e->left(), e->bottom() };

		list[i++] = { e->left(), e->bottom() };
		list[i++] = { e->left(), e->top() };
	}
}

void Graphics::update()
{
	std::cout << "Graphics::update\n";

	update_resources();

	m_scene.tilemap.update_verticies(m_scene.camera);

	int step = 6;
	for (int i = 0; i < (int)m_scene.tilemap.verticies.size() - step; i += step) {
		draw_tilemap(m_scene.tilemap.verticies.data() + i, step);
	}

	for (auto& e : *m_scene.m_entity_collection_ptr) {
		draw_entity(e);
	}

	DXGI_PRESENT_PARAMETERS pp{};
	pp.DirtyRectsCount = 0;
	pp.pDirtyRects = nullptr;
	pp.pScrollOffset = 0;
	pp.pScrollRect = 0;

	if (FAILED(m_swap_com->Present1(1u, 0u, &pp))) {
		HANDLE_IF_FAILED(m_device_com->GetDeviceRemovedReason());
	}
	clear_buffer();
}