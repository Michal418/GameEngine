#pragma once
#include <vector>
#include <string>

#include "../Input/KeyCode.h"
#include "../Vector2.h"
#include "../Scene/Camera.h"
#include <queue>

class Input
{
private:
	Input(Camera&);
	~Input();

public:
	bool get_key(uint64_t);
	bool get_key(Key);
	bool get_key_down(uint64_t);
	bool get_key_down(Key);
	bool get_key_up(uint64_t);
	bool get_key_up(Key);
	vec2 get_mouse();
	vec2 get_mouse_screen();
	std::string get_string();

	static void init(Camera&);
	void deinit();

	void update(float delta_time);
public:
	static Input* s_instance;

	friend class Renderer;
	friend class Window;

private:
	void on_key_down(uint64_t key_code, int64_t param);
	void on_key_up(uint64_t key_code, int64_t param);
	void on_mouse_move(uint64_t wide_param, int64_t long_param);
	void on_mouse_down(Key);
	void on_mouse_up(Key);
	void on_char(char);
	void on_focus_lost();

private:
	static constexpr int s_highest_key_plus = 227;

	Camera& m_camera;

	vec2 m_mouse = { 0, 0 };
	std::vector<bool> m_keys = std::vector<bool>(s_highest_key_plus, false);
	std::vector<bool> m_keys_down = std::vector<bool>(s_highest_key_plus, false);
	std::vector<bool> m_keys_up = std::vector<bool>(s_highest_key_plus, false);
	std::string m_string{};
};