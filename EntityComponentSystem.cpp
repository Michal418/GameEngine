#include "EntityComponentSystem.h"

void EntityComponentSystem::push_entity(Entity& e)
{
	if (e.collider.type == Collider::Type::dynamic_body) {
		dynamic_colliders.push_back(e.collider);
	}
	else {
		static_colliders.push_back(e.collider);
	}

	ids.push_back(e.Id);
	rigidbodys.push_back(e.rigidbody);
	sprites.push_back(e.sprite);
	transforms.push_back(e.transform);
}

template<>
Rigidbody* EntityComponentSystem::get_component<Rigidbody>(entity_id_t id)
{
	for (Rigidbody& r : rigidbodys) {
		if (r.id == id) {
			return &r;
		}
	}

	return nullptr;
}

template<>
Transform* EntityComponentSystem::get_component<Transform>(entity_id_t id)
{
	for (Transform& c : transforms) {
		if (c.id == id) {
			return &c;
		}
	}

	return nullptr;
}

template<>
Sprite* EntityComponentSystem::get_component<Sprite>(entity_id_t id)
{
	for (Sprite& c : sprites) {
		if (c.id == id) {
			return &c;
		}
	}

	return nullptr;
}

template<>
Collider* EntityComponentSystem::get_component<Collider>(entity_id_t id)
{
	for (Collider& c : dynamic_colliders) {
		if (c.id == id) {
			return &c;
		}
	}

	for (Collider& c : static_colliders) {
		if (c.width == id) {
			return &c;
		}
	}

	return nullptr;
}

