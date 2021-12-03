#pragma once
#include <windows.h>
#include <memory>

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Input/KeyCode.h"
#include "../Logger.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class Game;

class Window
{
public:
	Window(Game& game, Scene& scene);

public:
	HRESULT init(LPCWSTR name);
	bool process_messages();
	void show_message(LPCWSTR text);
	void invalidate_surface();

private:
	static LRESULT CALLBACK win_proc(HWND, UINT, WPARAM, LPARAM);
	static Window* get_this_ptr(HWND, UINT, LPARAM);

private:
	HRESULT on_render();

public:
	HRESULT m_hresult{};

private:
	Game& m_parent;
	Scene& m_scene;
	std::unique_ptr<Graphics> m_graphics_ptr = nullptr;
	MSG msg{};
	HWND m_hwnd{};
	uint32_t m_win_height = 0, m_win_width = 0;
	uint64_t last_texture_id = 1;
};