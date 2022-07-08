#include "Window.hpp"
#include "../Game.hpp"

Window::Window(Game& game, Scene& scene)
	: m_scene(scene), m_parent(game),
	m_win_width(m_scene.camera.size.x * m_scene.camera.pixel_size),
	m_win_height(m_scene.camera.size.y * m_scene.camera.pixel_size)
{}


HRESULT Window::init(LPCWSTR name)
{
	// window class
	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = L"Menu name";
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = name;

	RegisterClassEx(&wcex);

	m_hwnd = CreateWindow(
		name,
		name,
		WS_OVERLAPPED | WS_BORDER | WS_SYSMENU | WS_MINIMIZE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_win_width, //static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
		m_win_height, //static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	if (m_hwnd == 0)
	{
		SHOW_INFO(E_FAIL);
		return E_FAIL;
	}

	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	//UpdateWindow(m_hwnd);

	m_graphics_ptr = std::make_unique<Graphics>(m_hwnd, m_scene);

	return S_OK;
}

bool Window::process_messages()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
		return false;

	return m_parent.running;
}

void Window::show_message(LPCWSTR text)
{
	MessageBox(m_hwnd, text, L"Chyba", MB_OK);
}

void Window::invalidate_surface()
{
	m_graphics_ptr->update();
}

LRESULT Window::win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	Window* this_ptr = get_this_ptr(hwnd, uMsg, lParam);
	if (this_ptr == nullptr)
		return 1;

	switch (uMsg)
	{
	case WM_EXITSIZEMOVE:
		this_ptr->m_parent.reset_timer();
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window* Window::get_this_ptr(HWND hwnd, UINT uMsg, LPARAM lParam)
{
	Window* this_ptr{};

	if (uMsg == WM_NCCREATE)
	{
		this_ptr = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this_ptr)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		this_ptr = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
	}

	return this_ptr;
}

HRESULT Window::on_render()
{
	return E_NOTIMPL;
}
