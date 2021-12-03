#include "Entity.h"

void Entity::move_to(vec2 p)
{
	transform.position = p;
}

void Entity::move_by(vec2 vec)
{
	transform.position += vec;
}

void Entity::apply_force(vec2 vec)
{
	transform.velocity += vec / rigidbody.mass;
}

float Entity::left() { return transform.position.x - collider.width / 2; } // left extends in world transform.position
float Entity::right() { return transform.position.x + collider.width / 2; } // right extends in world transform.position
float Entity::top() { return transform.position.y + collider.height / 2; } // top extends in world transform.position
float Entity::bottom() { return transform.position.y - collider.height / 2; } // bottom extends in world transform.position