#pragma once
#include "../Scene/Entity.hpp"
#include "../Scene/Collision.hpp"

class Scene;
class Tilemap;

class Physics
{
public:
	static void update_entity(Entity&);
	static void check_collisions(Scene&);

private:
	static void resolve_collision(Entity& a, Entity& b);
	static void check_tile_collision(Entity&, Tilemap&);
	static void resolve_tile_collision(Entity&, vec2 distance, vec2 point);
	static void resolve_sollision_dynamic(Collision);
	static void resolve_sollision_static(Collision);
	static Collision get_collision(Entity& a, Entity& b);

public:
	static constexpr float gravitational_constant = 2000;
};