#include <Windows.h>
#include "Game.h"

class Player1 : public Entity
{
	float speed = 1;

public:
	virtual void on_create() override
	{
		texture = { L"", 3.2f };
		mass = 20;
		restitution = 1;
		position = { 100, 300 };
	};

	virtual void on_update() override
	{
		if (Input::s_instance->key_pressed(Key::shift))
			speed = 2;
		else
			speed = 1;

		if (key_pressed(Key::right_arrow))
			apply_force({ 100000 * speed * frame_ptr->delta_time, 0 });
		if (key_pressed(Key::left_arrow))
			apply_force({ -100000 * speed * frame_ptr->delta_time, 0 });
		if (key_pressed(Key::up_arrow))
			apply_force({ 0, 100000 * speed * frame_ptr->delta_time });
		if (key_pressed(Key::down_arrow))
			apply_force({ 0, -100000 * speed * frame_ptr->delta_time });

		if (key_down(Key::z))
		{
			apply_force({ 0, 300000 * frame_ptr->delta_time });
		}

		if (key_held_for_seconds(Key::ctrl, .25f))
		{
			position = { 200, 300 };
			velocity = { 0, 0 };
		}
	};
};

class Player2 : public Entity
{
public:

	virtual void on_create() override
	{
		texture = { L"", .33f };
		mass = 20;
		restitution = .2f;
		position = { 500, 600 };
	}

	virtual void on_update() override
	{
		if (key_pressed(Key::ctrl))
		{
			position = { 500, 600 };
			velocity = { 0, 0 };
		}
	}
};



class GameTest : public Game {
public:
	virtual void on_start() override
	{
		m_scene_ptr->add_entity<Player1>();
		m_scene_ptr->add_entity<Player2>();

		m_scene_ptr->add_entity<Entity>([](Entity& e)
			{
				e.mass = 1e+10F;
				e.position = { 640, 50 };
				e.width = 1800;
				e.restitution = .25f;
				e.friction = .85f;
				e.height = 20;
				e.collider_type = Entity::ColliderType::static_body;
			});

		m_scene_ptr->add_entity<Entity>([](Entity& e)
			{
				e.position = { 1200, 350 };
				e.width = 20;
				e.height = 600;
				e.collider_type = Entity::ColliderType::static_body;
				e.restitution = .6f;
			});

		m_scene_ptr->add_entity<Entity>([](Entity& e)
			{
				e.position = { -550, 350 };
				e.width = 20;
				e.height = 600;
				e.collider_type = Entity::ColliderType::static_body;
				e.restitution = .7f;
			});

		m_scene_ptr->add_entity<Entity>([](Entity& e)
			{
				e.position = { 1200, 250 };
				e.width = 150;
				e.height = 15;
				e.collider_type = Entity::ColliderType::static_body;
				e.restitution = .5f;
			});
	}

	virtual void on_update(float delta_time) override
	{
		m_scene_ptr->camera.position = (*this->m_scene_ptr->m_entity_collection_ptr)[0]->position;

		if (key_held_for_seconds(Key::escape, .5f))
		{
			close();
		}
	}
};


/*
* - tilemapy
*	- parsování tilemapy ze souboru
*	- uložení tilemapy v pamìti
*	- renderování tilemapy
*	- fyzika tilemapy
* - DirectInput pro lepší input
* - Direct3D a swap-chain
* - serializace sacény a entit
*/


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	GameTest game = GameTest();
	game.run();

	return 0;
}
