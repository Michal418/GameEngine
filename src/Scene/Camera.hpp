#pragma once
#include "../Vector2.hpp"

struct Camera
{
	Camera(vec2 world_size, vec2 screen_size)
		: size(world_size), pixel_size(screen_size.x / world_size.x)
	{
	}

	Camera(float width, float height, float pixel_size)
		: size(width, height), pixel_size(pixel_size)
	{
	}

	vec2 position{ 0, 0 };
	vec2 size;
	float pixel_size;

	inline float left() const { return position.x - size.x / 2; }
	inline float right() const { return position.x + size.x / 2; }
	inline float top() const { return position.y + size.y / 2; }
	inline float bottom() const { return position.y - size.y / 2; }
};

