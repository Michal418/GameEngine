#include "Physics.h"

#include "../Scene/Scene.h"

#define min(a, b) a > b ? b : a
#define max(a, b) a > b ? a : b

void Physics::resolve_collision(Entity& a, Entity& b)
{
	Collision abcollision = get_collision(a, b);
	Collision bacollision = get_collision(b, a);

	if (a.collider.type == Collider::Type::static_body
		&& b.collider.type == Collider::Type::static_body)
	{
		a.on_collision(b);
		b.on_collision(a);
	}
	else if (a.collider.type == Collider::Type::static_body
		&& b.collider.type == Collider::Type::dynamic_body)
	{
		Physics::resolve_sollision_static(bacollision);
		a.on_collision(b);
		b.on_collision(a);
	}
	else if (a.collider.type == Collider::Type::dynamic_body
		&& b.collider.type == Collider::Type::static_body)
	{
		Physics::resolve_sollision_static(abcollision);
		a.on_collision(b);
		b.on_collision(a);
	}
	else if (a.collider.type == Collider::Type::dynamic_body
		&& b.collider.type == Collider::Type::dynamic_body)
	{
		Physics::resolve_sollision_dynamic(abcollision);
		a.on_collision(b);
		b.on_collision(a);
	}
}

void Physics::resolve_tile_collision(Entity& a, vec2 distance, vec2 point)
{
	float e = a.rigidbody.restitution;

	// collision resolution
	a.transform.position += distance;
	a.transform.velocity *= distance.x == 0 ?
		vec2{ 1, -e } :
		vec2{ -e, 1 };

	// friction
	vec2 v_relative = a.transform.velocity;
	vec2 n = distance.get_normalized();

	vec2 t = (v_relative - vec2::dot_product(v_relative, n) * n).get_normalized();

	float j = -(1 + e) * vec2::dot_product(-a.transform.velocity, t)
		/ (1 / a.rigidbody.mass);

	float jt = -vec2::dot_product(v_relative, t);
	jt /= (1 / a.rigidbody.mass);

	float static_friction = sqrtf(powf(a.rigidbody.friction, 2) + 1);

	vec2 friction_impulse = abs(jt) < j * static_friction ?
		jt * t :
		-j * t * static_friction / 3;

	a.transform.velocity += .01f * friction_impulse;
}

// collision.a musí být dynamické a collision.b musí být statické
void Physics::resolve_sollision_static(Collision c)
{
	Entity& a = c.a;
	Entity& b = c.b;

	vec2& distance = c.distance;
	float e = fmaxf(a.rigidbody.restitution - b.rigidbody.restitution, 0);

	// collision resolution
	{
		a.transform.position += distance;
		a.transform.velocity *= distance.x == 0 ?
			vec2{ 1, -e } :
			vec2{ -e, 1 };
	}

	// friction
	{
		vec2 v_relative = a.transform.velocity;
		vec2 n = distance.get_normalized();

		vec2 t = (v_relative - vec2::dot_product(v_relative, n) * n).get_normalized();

		float j = -(1 + e) * vec2::dot_product(-a.transform.velocity, t)
			/ (1 / c.a.rigidbody.mass);

		float jt = -vec2::dot_product(v_relative, t);
		jt /= (1 / c.a.rigidbody.mass);

		float static_friction = sqrtf(powf(c.a.rigidbody.friction, 2) + powf(c.b.rigidbody.friction, 2));

		vec2 friction_impulse = abs(jt) < j * static_friction ?
			jt * t :
			-j * t * static_friction / 3;

		c.a.transform.velocity += .01f * friction_impulse;
	}
}

void Physics::resolve_sollision_dynamic(Collision c)
{
	Entity& a = c.a;
	Entity& b = c.b;

	float mass_sum = a.rigidbody.mass + b.rigidbody.mass;
	float apart = (b.rigidbody.mass / mass_sum);
	float bpart = (a.rigidbody.mass / mass_sum);

	vec2 v_relative = a.transform.velocity - b.transform.velocity;
	float e = fmaxf(a.rigidbody.restitution - b.rigidbody.restitution, 0);
	vec2 n = c.distance.get_normalized();

	// move objects outside eachother
	{
		a.transform.position += c.distance * apart;
		b.transform.position -= c.distance * bpart;
	}
	// penalty method
	{
		constexpr float k = 33;
		vec2 f = c.distance * k;
		a.transform.velocity += f * apart;
		b.transform.velocity -= f * bpart;
	}
	// impulse rosolution
	{
		constexpr float ia = 2000;
		constexpr float ib = 2000;

		float numerator = -(1 + e) * vec2::dot_product(v_relative, n);
		float denominator =
			(1 / a.rigidbody.mass)
			+ (1 / b.rigidbody.mass)
			+ powf((a.transform.position - c.point).dot_product(n), 2) / ia
			+ powf((b.transform.position - c.point).dot_product(n), 2) / ib;

		float j = numerator / denominator;

		a.transform.velocity += j * n / a.rigidbody.mass;
		b.transform.velocity -= j * n / b.rigidbody.mass;
	}
	// friction
	{
		vec2 t = (v_relative - vec2::dot_product(v_relative, n) * n).get_normalized();

		float j = -(1 + e) * vec2::dot_product(c.b.transform.velocity - c.a.transform.velocity, t)
			/ ((1 / c.a.rigidbody.mass) + (1 / c.b.rigidbody.mass));

		float jt = -vec2::dot_product(v_relative, t);
		jt /= (1 / c.a.rigidbody.mass + 1 / c.b.rigidbody.mass);

		float static_friction = sqrtf(powf(c.a.rigidbody.friction, 2) + powf(c.b.rigidbody.friction, 2));

		vec2 friction_impulse = abs(jt) < j * static_friction ?
			jt * t :
			-j * t * static_friction / 3;

		c.b.transform.velocity -= (1 / c.a.rigidbody.mass) * friction_impulse;
		c.a.transform.velocity += (1 / c.b.rigidbody.mass) * friction_impulse;
	}
}

Collision Physics::get_collision(Entity& a, Entity& b)
{
	Collision collision(a, b);

	float left = a.right() - b.left();
	float right = b.right() - a.left();
	float top = b.top() - a.bottom();
	float bottom = a.top() - b.bottom();

	auto horizontal_point = [&a, &b](float x) {
		return vec2{ x, (min(a.top(), b.top()) - max(a.bottom(), b.bottom())) / 2 };
	};

	auto vertical_point = [&a, &b](float y) {
		return vec2{ min(a.right(), b.right()) - max(a.left(), b.left()), y };
	};


	if (left < right and left < top and left < bottom)
	{
		collision.distance = vec2{ -left, 0 };
		collision.point = horizontal_point(b.left());
	}
	else if (right < left && right < top && right < bottom)
	{
		collision.distance = vec2{ right, 0 };
		collision.point = horizontal_point(a.left());
	}
	else if (top < left && top < right && top < bottom)
	{
		collision.distance = vec2{ 0, top };
		collision.point = vertical_point(a.bottom());
	}
	else if (bottom < left && bottom < right && bottom < top)
	{
		collision.distance = vec2{ 0, -bottom };
		collision.point = vertical_point(b.bottom());
	}

	return collision;
}

void Physics::update_entity(Entity& e)
{
	if (e.collider.type == Collider::Type::static_body)
		return;

	if (e.rigidbody.use_gravity)
		e.transform.velocity.y -= gravitational_constant * e.rigidbody.gravity_scale * e.frame_ptr->delta_time;

	e.transform.position += e.transform.velocity * e.frame_ptr->delta_time;
}

void Physics::check_tile_collision(Entity& a, Tilemap& tilemap)
{
	int d = tilemap.tile_diameter();

	auto f = [&a, d, &tilemap](int x, int y) {
		if (x < 0 || x >= tilemap.get_width() || y < 0 || y >= tilemap.get_height()) {
			return;
		}

		assert(y < tilemap.get_height());

		const Tile& t = tilemap[x][y];

		if (t.solid) {
			float b_left = x * d;
			float b_right = b_left + d;
			float b_bottom = y * d;
			float b_top = b_bottom + d;

			float left = a.right() - b_left;
			float right = b_right - a.left();
			float top = b_top - a.bottom();
			float bottom = a.top() - b_bottom;

			auto horizontal_point = [&a, b_top, b_bottom](float x) {
				return vec2{ x, (min(a.top(), b_top)) - max(a.bottom(), b_bottom) / 2 };
			};

			auto vertical_point = [&a, b_right, b_left](float y) {
				return vec2{ min(a.right(), b_right) - max(a.left(), b_left), y };
			};

			vec2 distance{};
			vec2 point{};

			if (left < right and left < top and left < bottom)
			{
				distance = vec2{ -left, 0 };
				point = horizontal_point(b_left);
			}
			else if (right < left && right < top && right < bottom)
			{
				distance = vec2{ right, 0 };
				point = horizontal_point(a.left());
			}
			else if (top < left && top < right && top < bottom)
			{
				distance = vec2{ 0, top };
				point = vertical_point(a.bottom());
			}
			else if (bottom < left && bottom < right && bottom < top)
			{
				distance = vec2{ 0, -bottom };
				point = vertical_point(b_bottom);
			}

			//resolve_tile_collision(a, distance, point);

			Entity e;
			e.rigidbody = tilemap.rigidbody;
			e.collider.width = e.collider.height = d;
			e.collider.type = Collider::Type::static_body;
			e.transform.velocity = { 0, 0 };
			e.transform.position = { b_left + d / 2, b_bottom + d / 2 };
			resolve_sollision_static({ a, e, distance, point });
			a.on_collision(e);
		}
	};

	for (int x = a.left(); x <= a.right(); x += a.right() - a.left()) {
		for (int y = a.bottom(); y <= a.top(); y += d) {
			f(x / d, y / d);
		}
	}
	for (int x = a.left() + d; x <= a.right() - d; x += d) {
		for (int y = a.bottom(); y <= a.top(); y += a.top() - a.bottom()) {
			f(x / d, y / d);
		}
	}
}

void Physics::check_collisions(Scene& scene)
{
	const size_t size = scene.m_entity_collection_ptr->count();
	for (uint32_t curri = 0; curri < size - 1; curri++)
	{
		std::shared_ptr<Entity> curr_entity = (*scene.m_entity_collection_ptr)[curri];

		for (uint32_t nexti = curri + 1; nexti < size; nexti++)
		{
			std::shared_ptr<Entity> next_entity = (*scene.m_entity_collection_ptr)[nexti];

			if (curr_entity->left() < next_entity->right()
				&& curr_entity->right() > next_entity->left()
				&& curr_entity->bottom() < next_entity->top()
				&& curr_entity->top() > next_entity->bottom())
			{
				Physics::resolve_collision(*curr_entity, *next_entity);
			}

		}
	}

	for (std::shared_ptr<Entity> e : *scene.m_entity_collection_ptr) {
		check_tile_collision(*e, scene.tilemap);
	}
}