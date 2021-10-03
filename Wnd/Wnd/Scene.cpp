#include "Scene.h"

Scene::Scene()
	: m_entity_collection_ptr{ new EntityCollection() },
	m_frame_ptr{ new Frame( 0 ) }
{ }

Scene::~Scene()
{
	delete m_entity_collection_ptr;
	delete m_frame_ptr;
}

void Scene::update(float delta_time)
{
	m_frame_ptr->delta_time = delta_time;

	for (Entity* entity : *(m_entity_collection_ptr))
	{
		entity->on_update();
		Physics::update_entity(*entity);
	}

	Physics::check_collisions(*m_entity_collection_ptr);
}

bool Scene::try_find_entity(entity_id_t id, Entity& out_entity)
{
	for (Entity* entity : *m_entity_collection_ptr)
	{
		if (entity->Id == id)
		{
			out_entity = *entity;
			return true;
		}
	}

	return false;
}

void Scene::init()
{
	for (Entity* entity : *m_entity_collection_ptr)
	{
		entity_id_t curr_id = math::hash_string(entity->texture.path);
		for (entity_id_t id : m_resource_ids)
		{
			if (curr_id != id)
			{
				m_resource_ids.push_back(curr_id);
			}
		}
	}
}