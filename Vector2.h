#pragma once

#include <math.h>

class vec2
{
public:

	float x, y;

	vec2() : x(0), y(0) { }
	vec2(float x, float y) : x(x), y(y) { };
	vec2& operator=(const vec2& v) { x = v.x; y = v.y; return *this; }

	inline bool operator==(const vec2& other) const
	{
		return x == other.x && y == other.y;
	}

	inline bool operator!=(const vec2& other) const
	{
		return x != other.x || y != other.y;
	}

	inline bool operator<(const vec2& other) const
	{
		return this->Size < other.Size;
	}

	inline bool operator>(const vec2& other) const
	{
		return this->Size > other.Size;
	}

	inline vec2& operator+=(vec2 other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	inline vec2 operator+(vec2 other) const
	{
		return vec2{ this->x + other.x, this->y + other.y };
	}
	inline vec2& operator-=(vec2 other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	inline vec2 operator-(vec2 other) const
	{
		return vec2{ this->x - other.x, this->y - other.y };
	}
	inline vec2& operator*=(float i)
	{
		this->x *= i;
		this->y *= i;
		return *this;
	}
	inline vec2 operator*(vec2 other) const
	{
		return vec2{ x * other.x, y * other.y };
	}
	inline vec2 operator*=(vec2 other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}
	inline vec2 operator*(float i) const
	{
		return vec2{ this->x * i, this->y * i };
	}
	inline vec2 operator/(float i) const
	{
		return vec2{ this->x / i, this->y / i };
	}
	inline vec2 operator-(float i) const
	{
		return vec2(x - i, y - i);
	}
	inline vec2 operator-() const
	{
		return vec2{ -x, -y };
	}

	static inline float dot_product(vec2 a, vec2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float get_scalar_square() const
	{
		return x * x + y * y;
	}

	inline float dot_product(vec2 other) const
	{
		return vec2::dot_product(*this, other);
	}

	inline vec2 get_normalized() const
	{
		if (x == 0)
			return { 0, 1 };
		if (y == 0)
			return { 1, 0 };

		float n = 1.0f / fminf(x, y);
		return { x * n, y * n };
	}

	inline vec2 get_squared() const
	{
		return { x * x, y * y };
	}

	inline float get_size() const
	{
		return static_cast<float>(sqrt(static_cast<double>(x) * static_cast<double>(x)))
			+ static_cast<float>(sqrt(static_cast<double>(y) * static_cast<double>(y)));
	}

	float __declspec(property(get = get_size)) Size;
	vec2 __declspec(property(get = get_normalized)) Normalized;
};

inline vec2 operator/(float k, const vec2& vec)
{
	return { vec.x / k, vec.y / k };
}

inline vec2 operator*(float k, const vec2& vec)
{
	return { vec.x * k, vec.y * k };
}