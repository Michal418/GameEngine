#pragma once
#include "Vector2.h"

class ICollider
{
public:
	virtual inline float get_top() = 0;
	virtual inline float get_bottom() = 0;
	virtual inline float get_left() = 0;
	virtual inline float get_right() = 0;
	virtual inline float get_x() = 0;
	virtual inline float get_y() = 0;
	virtual inline vec2 get_center() = 0;

	float __declspec(property(get = get_top)) Top;
	float __declspec(property(get = get_bottom)) Bottom;
	float __declspec(property(get = get_left)) Left;
	float __declspec(property(get = get_right)) Right;
	float __declspec(property(get = get_x)) X;
	float __declspec(property(get = get_y)) Y;
	vec2 __declspec(property(get = get_center)) Center;
};

class BoxCollider : public ICollider
{
public:
	BoxCollider(float radius, vec2* center_ptr)
		: m_radius{ radius }, m_center_ptr{ center_ptr }
	{
	}

public:
	inline float get_top() override { return m_center_ptr->y + m_radius; }
	inline float get_bottom() override { return m_center_ptr->y - m_radius; }
	inline float get_left() override { return m_center_ptr->x - m_radius; }
	inline float get_right() override { return m_center_ptr->x + m_radius; }
	inline float get_x() override { return m_center_ptr->x; }
	inline float get_y() override { return m_center_ptr->y; }
	inline vec2 get_center() override { return *m_center_ptr; }

public:
	const float m_radius;
	const vec2* m_center_ptr;
};

class RectCollider : public ICollider
{
public:
	RectCollider(float width, float height, vec2* center_ptr)
		: m_width{ width }, m_height{ height }, m_center_ptr{ center_ptr }
	{ }

public:
	inline float get_top() override { return m_center_ptr->y + m_height; }
	inline float get_bottom() override { return m_center_ptr->y - m_height; }
	inline float get_left() override { return m_center_ptr->x - m_width; }
	inline float get_right() override { return m_center_ptr->x + m_width; }
	inline vec2 get_center() override { return *m_center_ptr; }

public:
	const vec2* m_center_ptr;
	const float m_width;
	const float m_height;
};