#pragma once
#include "Entity.h"
#include "EntityCollection.h"
#include "Collision.h"

class Physics
{
public:
	static void update_entity(Entity&);
	static void check_collisions(EntityCollection&);

private:
	static void resolve_collision(Entity& a, Entity& b);
	static void resolve_sollision_dynamic(Collision);
	static void resolve_sollision_static(Collision);
	static Collision get_collision(Entity& a, Entity& b);

public:
	static constexpr float gravitational_constant = 2000;
};