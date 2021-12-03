#include "../Scene/Scene.h"
#include "../Math.h"



Scene::Scene(uint16_t width, uint16_t height, uint16_t pixel_size)
	: m_entity_collection_ptr{ new EntityCollection() },
	m_frame_ptr{ new Frame( 0 ) },
	camera(width, height, pixel_size)
{
	init();
}

Scene::Scene(vec2 world_size, vec2 screen_size)
	: m_entity_collection_ptr{ new EntityCollection() },
	m_frame_ptr{ new Frame(0) },
	camera(world_size, screen_size)
{
	init();
}

Scene::~Scene()
{
	delete m_entity_collection_ptr;
	delete m_frame_ptr;
}

void Scene::update(float delta_time)
{
	m_frame_ptr->delta_time = delta_time;

	for (std::shared_ptr<Entity> entity : *(m_entity_collection_ptr))
	{
		entity->on_update();
		Physics::update_entity(*entity);
	}

	Physics::check_collisions(*this);
}

bool Scene::try_find_entity(entity_id_t id, Entity& out_entity)
{
	for (std::shared_ptr<Entity> entity : *m_entity_collection_ptr)
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
	tilemap = Tilemap::from_file("tilemap.tmx");
}