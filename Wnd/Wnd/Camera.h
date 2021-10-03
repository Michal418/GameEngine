#pragma once
#include "Vector2.h"

struct Camera
{
	vec2 position{ 0, 0 };
	vec2 size{ 1920, 1080 };

	inline float left() { return position.x - size.x / 2; }
	inline float right() { return position.x + size.x / 2; }
	inline float top() { return position.y + size.y / 2; }
	inline float bottom() { return position.y - size.y / 2; }

	Camera() { };
};

