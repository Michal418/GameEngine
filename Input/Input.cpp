#include "Input.h"
#include <string>
#include <windowsx.h>
#include <Windows.h>

#define KEY_PRESSED 0x8000

template<class T>
void swap(T*& a, T*& b)
{
	T* temp = a;
	a = b;
	b = temp;
}

Input::Input(Camera& camera)
	: m_camera(camera)
{
};

Input::~Input()
{
	deinit();
}

void Input::init(Camera& camera)
{
	if (Input::s_instance)
		delete Input::s_instance;

	Input::s_instance = new Input(camera);
}

void Input::deinit()
{
}

// events

void Input::on_key_down(uint64_t key_code, int64_t param)
{
	m_keys[key_code] = true;
	m_keys_down[key_code] = true;
}

void Input::on_key_up(uint64_t key_code, int64_t param)
{
	m_keys[key_code] = false;
	m_keys_up[key_code] = true;
}

void Input::on_char(char ch)
{
	if (ch == '\r')
	{
		m_string = "\r\n";
		return;
	}

	m_string += ch;
}

void Input::update(float delta_time)
{
	m_keys_up = m_keys_down = std::vector<bool>(s_highest_key_plus, false);
}

void Input::on_focus_lost()
{
	m_keys = std::vector<bool>(s_highest_key_plus, false);
}

void Input::on_mouse_move(uint64_t winde_param, int64_t long_param)
{
	m_mouse = vec2
	{
		(float)GET_X_LPARAM(long_param),
		(float)GET_Y_LPARAM(long_param)
	};
}

void Input::on_mouse_down(Key key)
{
	m_keys[(int)key] = true;
	m_keys_down[(int)key] = true;
}

void Input::on_mouse_up(Key key)
{
	m_keys[(int)key] = false;
	m_keys_up[(int)key] = true;
}

// getters

std::string Input::get_string()
{
	std::string val = m_string;
	m_string.clear();
	return val;
}

vec2 Input::get_mouse_screen()
{
	return m_mouse;
}

vec2 Input::get_mouse()
{
	return vec2{ (m_mouse.x / m_camera.pixel_size) + m_camera.left(), -(m_mouse.y / m_camera.pixel_size) + m_camera.top() };
}

bool Input::get_key_down(uint64_t key)
{
	return m_keys_down[key];
}

bool Input::get_key_up(uint64_t key)
{
	return m_keys_up[key];
}

bool Input::get_key(uint64_t key)
{
	return m_keys[key];
}

bool Input::get_key_down(Key key)
{
	return get_key_down((uint64_t)key);
}

bool Input::get_key_up(Key key)
{
	return get_key_up((uint64_t)key);
}

bool Input::get_key(Key key)
{
	return get_key((uint64_t)key);
}

Input* Input::s_instance = nullptr;