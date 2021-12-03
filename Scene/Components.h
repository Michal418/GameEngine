struct vec2;
typedef entity_id_t;

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