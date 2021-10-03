#pragma once
#include <vector>
#include <string>

#include "KeyCode.h"

class Input
{
private:
	Input();
	~Input();

public:
	void update(float delta_time);

	bool key_pressed(int);
	bool key_pressed(Key);
	bool key_held_for_seconds(int, float);
	bool key_held_for_seconds(Key, float);
	bool key_down(int);
	bool key_down(Key);
	bool key_up(int);
	bool key_up(Key);

	float get_key_held_time(int key);
	float get_key_held_time(Key key);

	static void init();
	void deinit();

public:
	static Input* s_instance;

	// owned pointers

	float* m_key_held_times;

	// o 1 víc než je nejvyšší èíslo v Key enum
	static constexpr int highest_key_plus = 224;

	float delta_time = 0;
};

bool key_pressed(int);
bool key_pressed(Key);
bool key_held_for_seconds(int, float);
bool key_held_for_seconds(Key, float);
bool key_down(int);
bool key_down(Key);
bool key_up(int);
bool key_up(Key);