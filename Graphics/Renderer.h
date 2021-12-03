 #pragma once
// Windows Header Files:
#include <windows.h>

#pragma comment(lib, "d2d1.lib")

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <map>

#include "../Scene/Scene.h"

template<class Interface>
inline void SafeRelease(
    Interface** ppInterfaceToRelease
)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

enum class GameResult { ok = 1, failed = -1, quit = -2, distrust_timer = 2 };

struct Sprite
{
	ID2D1Bitmap* bitmap{};
	float size = 1;
};

class Renderer
{
public:
	Renderer(std::shared_ptr<Scene>);
	~Renderer();

public:
	void invalidate_surface() const;
	bool process_messages();
	HRESULT init(uint16_t width, uint16_t height, LPCWSTR name);
	void show_message(LPCWSTR text);

private:
	static LRESULT CALLBACK win_proc(HWND, UINT, WPARAM, LPARAM);
	static Renderer* get_this_ptr(HWND, UINT, LPARAM);

private:
	HRESULT create_device_independent_resources();
	HRESULT create_device_resources();
	void discard_device_resources();
	HRESULT on_render();
	HRESULT load_image(const LPCWSTR file_name, ID2D1Bitmap*& out_bitmap_ptr) const;
	HRESULT load_image(const char* file_name, ID2D1Bitmap*& out_bitmap_ptr) const;
	HRESULT draw_entity(Entity&);
	HRESULT draw_entity_gizmos(Entity&);
	Sprite get_texture(entity_id_t);

public:
	HRESULT m_hresult;

private:
	GameResult result = GameResult::ok;
	MSG msg;
	HWND m_hwnd;
	uint32_t m_win_height, m_win_width;
	ID2D1SolidColorBrush*	m_background_brush_ptr; // vlastník je Renderer
	ID2D1SolidColorBrush*	m_foreground_brush_ptr; // vlastník je Renderer
	ID2D1SolidColorBrush* m_gizmos_brush_ptr; // vlastník Renderer
	ID2D1Factory*			m_direct2d_factory_ptr; // vlastník je Renderer
	ID2D1HwndRenderTarget*	m_render_target_ptr; // vlastník je Renderer
	ID2D1Bitmap* m_debug_bitmap{}; // vlastník je Renderer
	std::shared_ptr<Scene> m_scene_ptr; // vlastník je Game
	std::map<entity_id_t, Sprite> m_textures;
	std::map<entity_id_t, entity_id_t> m_texture_keys;
	uint64_t last_texture_id = 1;
};