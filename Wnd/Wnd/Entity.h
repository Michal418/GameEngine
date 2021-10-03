#pragma once
#include <functional>

#include "Vector2.h"
#include "Texture.h"
#include "Input.h"
#include "Collision.h"

struct Frame
{
	float delta_time;

	Frame(float delta_time)
		: delta_time{ delta_time }
	{ }
};

typedef uint64_t entity_id_t;

class Entity
{
public:
	void move_to(vec2 p);
	void move_by(vec2 vec);
	void apply_force(vec2);

	virtual void on_update() { }
	virtual void on_create() { }
	virtual void on_collision(Collision) { }

	float left();
	float right();
	float top();
	float bottom();

	inline entity_id_t get_id() { return id; }

	bool operator!=(const Entity& other)
	{
		return this->id != other.id;
	}

	bool operator==(const Entity& other)
	{
		return this->id == other.id;
	}

public:
	enum class ColliderType { dynamic_body, static_body };
	ColliderType collider_type = ColliderType::dynamic_body;
	bool use_gravity = true;
	float mass = 0;
	float restitution = .85f;
	Texture texture{};
	vec2 position{ 0, 0 };
	vec2 velocity{ 0, 0 };
	float width = 100;
	float height = 100;
	float friction = .86f;

	Frame* frame_ptr;

	entity_id_t __declspec(property(get = get_id)) Id;

private:
	entity_id_t id = 0;

	friend class EntityCollection;
};