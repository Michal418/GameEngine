#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include <comdef.h>
#include <chrono>
#include <string>

#include "../Wnd/Scene/Scene.h"
#include "../Wnd/Input/Input.h"

#define WINDOW

class Window;

class Game
{
public:
	Game() = delete;
	Game(LPCWSTR name);
	~Game();

public:
	void run();

protected:
	virtual void on_create() { };
	virtual void on_start() { };
	virtual void on_update(float delta_time) { };

	void close();

private:
	void create();
	void start();
	void update(float delta_time);

	void reset_timer();
	int64_t get_milliseconds_since_epoch() const;

protected:
	Scene m_scene;

private:
	std::shared_ptr<Window> m_window_ptr;
	const wchar_t* name;
	bool running = true;
	int64_t last_update_time;
	vec2 size = { 1080, 1920 };

	friend class Window;
};
