#include "Entity.h"

void Entity::move_to(vec2 p)
{
	vec2 vec = position - p;
	move_by(vec);
}

void Entity::move_by(vec2 vec)
{
	position += vec;
}

void Entity::apply_force(vec2 vec)
{
	velocity += vec / mass;
}

float Entity::left() { return position.x - width; } // left extends in world position
float Entity::right() { return position.x + width; } // right extends in world position
float Entity::top() { return position.y + height; } // top extends in world position
float Entity::bottom() { return position.y - height; } // bottom extends in world position