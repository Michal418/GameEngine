#include <chrono>
#include <d2d1.h>
#include <d3d9.h>

#include "../Graphics/Renderer.h"

Renderer::Renderer(std::shared_ptr<Scene> scene_ptr)
	: m_hwnd{}, msg{},
	m_direct2d_factory_ptr(NULL), m_render_target_ptr(NULL),
	m_win_height{}, m_win_width{},
	m_scene_ptr(scene_ptr)
{ }

Renderer::~Renderer()
{
	SafeRelease(&m_background_brush_ptr);
	SafeRelease(&m_foreground_brush_ptr);
}

HRESULT Renderer::init(uint16_t width, uint16_t height, LPCWSTR name)
{
	HRESULT hr = CoInitialize(NULL);

	if (FAILED(hr))
		return hr;

	m_win_width = width;
	m_win_height = height;

	hr = create_device_independent_resources();

	if (FAILED(hr))
		return hr;

	// window class
	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = name;

	RegisterClassEx(&wcex);

	m_hwnd = CreateWindow(
		name,
		name,
		WS_OVERLAPPED | WS_CAPTION			// Window style
		| WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, //static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
		height, //static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	hr = m_hwnd ? S_OK : E_FAIL;
	if (FAILED(hr))
		return hr;

	hr = create_device_resources();
	if (FAILED(hr)) return hr;

	for (auto& entity : *m_scene_ptr->m_entity_collection_ptr)
	{
		if (m_texture_keys[entity->Id])
			continue;

		Sprite sprite{};
		sprite.size = entity->sprite.size;
		if (SUCCEEDED(load_image(entity->sprite.path, sprite.bitmap)))
		{
			entity_id_t id = ++last_texture_id;
			m_textures[id] = sprite;
			m_texture_keys[entity->Id] = id;
		}
	}

	SetPriorityClass(m_hwnd, REALTIME_PRIORITY_CLASS);

	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);

	return hr;
}

HRESULT Renderer::create_device_independent_resources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_direct2d_factory_ptr);
	return hr;
}

HRESULT Renderer::create_device_resources()
{
	HRESULT hr = S_OK;

	if (!m_render_target_ptr)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_direct2d_factory_ptr->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_render_target_ptr
		);

		if (FAILED(hr)) return hr;

		// Create a gray brush.
		hr = m_render_target_ptr->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::LightSlateGray),
			&m_background_brush_ptr
		);

		if (FAILED(hr)) return hr;

		// Create a blue brush.
		hr = m_render_target_ptr->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&m_foreground_brush_ptr
		);

		hr = m_render_target_ptr->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Cyan),
			&m_gizmos_brush_ptr
		);
	}

	return hr;
}

Sprite Renderer::get_texture(entity_id_t id)
{
	return m_textures[m_texture_keys[id]];
}

HRESULT Renderer::draw_entity(Entity& entity)
{
	HRESULT hr = S_OK;

	/*
	if (!m_debug_bitmap)
	{
		hr = load_image(L"debug_image.png", m_debug_bitmap);
		if (FAILED(hr)) return hr;
	}
	*/

	Sprite sprite = get_texture(entity.Id);
	if (!sprite.bitmap)
		return hr;

	D2D1_SIZE_U bitmap_size = sprite.bitmap->GetPixelSize();
	Camera cam = m_scene_ptr->camera;

	D2D1_RECT_F r = D2D1::RectF(
		entity.transform.position.x - bitmap_size.width * sprite.size - cam.left(),
		m_win_height - entity.transform.position.y - bitmap_size.height * sprite.size + cam.bottom(),
		entity.transform.position.x + bitmap_size.width * sprite.size - cam.left(),
		m_win_height - entity.transform.position.y + bitmap_size.height * sprite.size + cam.bottom()
	);

	m_render_target_ptr->DrawBitmap(sprite.bitmap, r);

	return hr;
}

HRESULT Renderer::draw_entity_gizmos(Entity& entity)
{
	auto brush = m_scene_ptr->m_colliding ? m_foreground_brush_ptr : m_background_brush_ptr;
	Camera cam = m_scene_ptr->camera;

	// vykreslí okraje collideru entity
	m_render_target_ptr->DrawRectangle(D2D1::RectF(
		entity.left() - cam.left(),
		m_win_height - entity.top() + cam.bottom(),
		entity.right() - cam.left(),
		m_win_height - entity.bottom() + cam.bottom()), brush);

	// vykreslí rychlostní vektor entity
	m_render_target_ptr->DrawLine(
		{
			entity.transform.position.x,
			entity.transform.position.y
		},
		{
			entity.transform.position.x + entity.transform.velocity.x,
			entity.transform.position.y + entity.transform.position.y
		},
		m_gizmos_brush_ptr, 2.5f);

	return S_OK;
}

HRESULT Renderer::on_render()
{
	HRESULT hr = create_device_resources();

	if (FAILED(hr)) return hr;

	m_render_target_ptr->BeginDraw();
	m_render_target_ptr->SetTransform(D2D1::Matrix3x2F::Identity());

	// vyèistí obrazovku
	m_render_target_ptr->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));

	// vykreslí každou entitu
	for (Entity* e : *(m_scene_ptr->m_entity_collection_ptr))
	{
		hr = draw_entity(*e);
		if (FAILED(hr)) return hr;
	}

	// vykreslí gizmos každé enity
	for (Entity* e : *(m_scene_ptr->m_entity_collection_ptr))
	{
		hr = draw_entity_gizmos(*e);
		if (FAILED(hr)) return hr;
	}

	hr = m_render_target_ptr->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		discard_device_resources();
	}

	return S_OK;
}

void Renderer::discard_device_resources()
{
	SafeRelease(&m_render_target_ptr);
	SafeRelease(&m_foreground_brush_ptr);
	SafeRelease(&m_background_brush_ptr);
}

Renderer* Renderer::get_this_ptr(HWND hwnd, UINT uMsg, LPARAM lParam)
{
	Renderer* pThis{};

	if (uMsg == WM_NCCREATE)
	{
		pThis = static_cast<Renderer*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<Renderer*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
	}

	return pThis;
}

void Renderer::invalidate_surface() const
{
	InvalidateRect(m_hwnd, NULL, FALSE);
}

HRESULT Renderer::load_image(const char* file_name, ID2D1Bitmap*& out_bitmap_ptr) const
{
	size_t size = strlen(file_name);
	wchar_t* texture_path = new wchar_t[size + 1];
	mbstowcs_s(nullptr, texture_path, size + 1, file_name, size);
	HRESULT hr = load_image(texture_path, out_bitmap_ptr);
	delete[] texture_path;
	return hr;
}

HRESULT Renderer::load_image(const LPCWSTR file_name, ID2D1Bitmap*& out_bitmap_ptr) const
{
	/*
	* Nevím jak a proè to funguje
	*/

	HRESULT hr = S_OK;

	static IWICImagingFactory* iwic_factory_ptr = NULL;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&iwic_factory_ptr));

	if (FAILED(hr)) return hr;

	IWICBitmapDecoder* decoder_ptr = NULL;
	hr = iwic_factory_ptr->CreateDecoderFromFilename(
		file_name,                      // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&decoder_ptr                        // Pointer to the decoder
	);

	if (FAILED(hr)) return hr;

	IWICBitmapFrameDecode* frame_ptr = NULL;
	hr = decoder_ptr->GetFrame(0, &frame_ptr);

	if (FAILED(hr)) return hr;

	IWICFormatConverter* format_converter_ptr{};
	hr = iwic_factory_ptr->CreateFormatConverter(&format_converter_ptr);

	if (FAILED(hr)) return hr;

	hr = format_converter_ptr->Initialize(
		frame_ptr,                          // Input bitmap to convert
		GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
		WICBitmapDitherTypeNone,         // Specified dither pattern
		NULL,                            // Specify a particular palette
		0.f,                             // Alpha threshold
		WICBitmapPaletteTypeCustom       // Palette translation type
	);

	hr = m_render_target_ptr->CreateBitmapFromWicBitmap(format_converter_ptr, NULL, &out_bitmap_ptr);

	return hr;
}

LRESULT CALLBACK Renderer::win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(wParam);
		return 0;

		// input

	case WM_CHAR:
		Input::s_instance->on_char((char)wParam);
		break;
	case WM_KEYDOWN:
		Input::s_instance->on_key_down((uint64_t)wParam, (int64_t)lParam);
		break;
	case WM_KEYUP:
		Input::s_instance->on_key_up((uint64_t)wParam, (int64_t)lParam);
		break;
	case WM_KILLFOCUS:
		Input::s_instance->on_focus_lost();
		break;
	case WM_MOUSEMOVE:
		Input::s_instance->on_mouse_move((uint64_t)wParam, (int64_t)lParam);
		break;
	case WM_LBUTTONDOWN:
		Input::s_instance->on_mouse_down(Key::left_mouse);
		break;
	case WM_LBUTTONUP:
		Input::s_instance->on_mouse_up(Key::left_mouse);
		break;
	case WM_RBUTTONDOWN:
		Input::s_instance->on_mouse_down(Key::right_mouse);
		break;
	case WM_RBUTTONUP:
		Input::s_instance->on_mouse_up(Key::right_mouse);
		break;
	case WM_MBUTTONDOWN:
		Input::s_instance->on_mouse_down(Key::middle_mouse);
		break;
	case WM_MBUTTONUP:
		Input::s_instance->on_mouse_up(Key::middle_mouse);
		break;

	}

	Renderer* this_ptr = Renderer::get_this_ptr(hwnd, uMsg, lParam);
	if (this_ptr == nullptr)
		return 1;

	switch (uMsg)
	{
	case WM_PAINT:
		this_ptr->m_hresult = this_ptr->on_render();
		if (FAILED(this_ptr->m_hresult))
			PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Renderer::show_message(LPCWSTR text)
{
	MessageBox(m_hwnd, text, L"Chyba", MB_OK);
}

bool Renderer::process_messages()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	return true;
}