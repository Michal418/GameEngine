#include "Input.h"
#include <string>
#include <Windows.h>

#define KEY_PRESSED 0x8000

template<class T>
void swap(T*& a, T*& b)
{
	T* temp = a;
	a = b;
	b = temp;
}

Input::Input()
	: m_key_held_times{ new float[highest_key_plus] }
{
	for (int i = 0; i < highest_key_plus; i++)
	{
		m_key_held_times[i] = 0;
	}
};

Input::~Input()
{
	if (m_key_held_times)
		delete[] m_key_held_times;
}

void Input::init()
{
	if (Input::s_instance)
		delete Input::s_instance;

	Input::s_instance = new Input();
}

void Input::deinit()
{
	if (m_key_held_times)
		delete[] m_key_held_times;
	m_key_held_times = nullptr;
}

void Input::update(float delta_time)
{
	this->delta_time = delta_time;

	for (int key = 0; key < highest_key_plus; key = key++)
	{
		if (GetKeyState(key) & KEY_PRESSED)
		{
			m_key_held_times[key] += delta_time;
		}
		else
		{
			m_key_held_times[key] = 0;
		}
	}
}

bool Input::key_down(int key)
{
	return m_key_held_times[key] > 0 && m_key_held_times[key] - delta_time == 0;
}

bool Input::key_down(Key key)
{
	return key_down((int)key);
}

bool Input::key_up(int key)
{
	return false;
}

bool Input::key_up(Key key)
{
	return key_up((int)key);
}

bool Input::key_pressed(int key)
{
	return m_key_held_times[key] > 0;
}

bool Input::key_pressed(Key key)
{
	return key_pressed((int)key);
}

bool Input::key_held_for_seconds(int key, float seconds)
{
	return m_key_held_times[key] >= seconds;
}

bool Input::key_held_for_seconds(Key key, float seconds)
{
	return key_held_for_seconds((int)key, seconds);
}

float Input::get_key_held_time(int key)
{
	return m_key_held_times[key];
}

float Input::get_key_held_time(Key key)
{
	return get_key_held_time((int)key);
}

Input* Input::s_instance = nullptr;

// free functions

bool key_down(int key)
{
	return Input::s_instance->m_key_held_times[key] > 0
		&& Input::s_instance->m_key_held_times[key] - Input::s_instance->delta_time == 0;
}

bool key_down(Key key)
{
	return key_down((int)key);
}

bool key_up(int key)
{
	return false;
}

bool key_up(Key key)
{
	return key_up((int)key);
}

bool key_pressed(int key)
{
	return Input::s_instance->m_key_held_times[key] > 0;
}

bool key_pressed(Key key)
{
	return key_pressed((int)key);
}

bool key_held_for_seconds(int key, float seconds)
{
	return Input::s_instance->m_key_held_times[key] >= seconds;
}

bool key_held_for_seconds(Key key, float seconds)
{
	return Input::s_instance->key_held_for_seconds((int)key, seconds);
}
