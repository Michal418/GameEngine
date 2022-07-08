#pragma once
#include <functional>

#include "../Graphics/Sprite.hpp"
#include "../Vector2.hpp"
#include "../Input/Input.hpp"
#include "../Scene/Collision.hpp"

struct Frame
{
	float delta_time;

	Frame(float delta_time)
		: delta_time{ delta_time }
	{ }
};

typedef uint64_t entity_id_t;

struct Component
{
	entity_id_t id = 0;
};

struct Rigidbody : public Component
{
	float mass = 90;
	float restitution = .40f;
	float friction = .35f;
	float gravity_scale = 1.0f;
	bool use_gravity = true;
};

struct Transform : public Component
{
	vec2 position{ 0, 0 };
	vec2 velocity{ 0, 0 };
};

struct Collider : public Component
{
	enum class Type { dynamic_body, static_body };
	Type type = Type::dynamic_body;
	float width = 100;
	float height = 100;
};

class Entity
{
public:
	void move_to(vec2 p);
	void move_by(vec2 vec);
	void apply_force(vec2);

	virtual void on_update() { }
	virtual void on_create() { }
	virtual void on_collision(Entity) { }

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
	Collider collider{};
	Rigidbody rigidbody{};
	Transform transform{};

	Sprite sprite{};

	Frame* frame_ptr;

	entity_id_t __declspec(property(get = get_id)) Id;

private:
	entity_id_t id = 0;

	friend class EntityCollection;
};