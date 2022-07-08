#pragma once

class Entity;

struct Collision
{
	Entity& a;
	Entity& b;
	vec2 distance;
	vec2 point;

	Collision(Entity& a, Entity& b) : a(a), b(b), distance(0, 0), point(0, 0) { }
	Collision(Entity& a, Entity& b, vec2 d, vec2 p) : a(a), b(b), distance(d), point(p) { }
};