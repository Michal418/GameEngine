#include "Physics.h"

void Physics::resolve_collision(Entity& a, Entity& b)
{
	Collision collision = Physics::get_collision(a, b);
	Collision bcollision = Collision{ collision.b, collision.a, collision.distance, collision.point };

	if (a.collider_type == Entity::ColliderType::static_body)
	{
		if (b.collider_type == Entity::ColliderType::static_body)
		{
			a.on_collision(collision);
			b.on_collision(bcollision);
			return;
		}

		Physics::resolve_sollision_static(bcollision);
		a.on_collision(collision);
		b.on_collision(bcollision);
		return;
	}

	if (b.collider_type == Entity::ColliderType::static_body)
	{
		Physics::resolve_sollision_static(collision);
		a.on_collision(collision);
		b.on_collision(bcollision);
		return;
	}

	Physics::resolve_sollision_dynamic(collision);

	a.on_collision(collision);
	b.on_collision(bcollision);
}

// collision.a musí být dynamické a collision.b musí být statické
void Physics::resolve_sollision_static(Collision c)
{
	Entity& a = c.a;
	Entity& b = c.b;

	vec2 distance = c.distance;
	float e = fminf(c.a.restitution, c.b.restitution);

	// sollision resolution
	{
		a.position += distance;
		a.velocity *= distance.x == 0 ?
			vec2{ 1, -e } :
			vec2{ -e, 1 };
	}

	// friction
	{
		vec2 v_relative = a.velocity;
		vec2 n = distance.get_normalized();

		vec2 t = (v_relative - vec2::dot_product(v_relative, n) * n).get_normalized();

		float j = -(1 + e) * vec2::dot_product(-a.velocity, t)
			/ ((1 / c.a.mass));

		float jt = -vec2::dot_product(v_relative, t);
		jt /= (1 / c.a.mass);

		float static_friction = sqrtf(powf(c.a.friction, 2) + powf(c.b.friction, 2));

		vec2 friction_impulse = abs(jt) < j * static_friction ?
			jt * t :
			-j * t * static_friction / 3;

		c.a.velocity += .01f * friction_impulse;
	}
}

void Physics::resolve_sollision_dynamic(Collision c)
{
	Entity& a = c.a;
	Entity& b = c.b;

	float mass_sum = a.mass + b.mass;
	float apart = (b.mass / mass_sum);
	float bpart = (a.mass / mass_sum);

	vec2 v_relative = a.velocity - b.velocity;
	float e = fminf(a.restitution, b.restitution);
	vec2 n = c.distance.get_normalized();

	// move objects outside eachother
	{
		a.position += c.distance * apart;
		b.position -= c.distance * bpart;
	}
	// penalty method
	{
		constexpr float k = 33;
		vec2 f = c.distance * k;
		a.velocity += f * apart;
		b.velocity -= f * bpart;
	}
	// impulse rosolution
	{
		constexpr float ia = 2000;
		constexpr float ib = 2000;

		float numerator = -(1 + e) * vec2::dot_product(v_relative, n);
		float denominator =
			(1 / a.mass)
			+ (1 / b.mass)
			+ powf((a.position - c.point).dot_product(n), 2) / ia
			+ powf((b.position - c.point).dot_product(n), 2) / ib;

		float j = numerator / denominator;

		a.velocity += j * n / a.mass;
		b.velocity -= j * n / b.mass;
	}
	// friction
	{
		vec2 t = (v_relative - vec2::dot_product(v_relative, n) * n).get_normalized();

		float j = -(1 + e) * vec2::dot_product(c.b.velocity - c.a.velocity, t)
			/ ((1 / c.a.mass) + (1 / c.b.mass));

		float jt = -vec2::dot_product(v_relative, t);
		jt /= (1 / c.a.mass + 1 / c.b.mass);

		float static_friction = sqrtf(powf(c.a.friction, 2) + powf(c.b.friction, 2));

		vec2 friction_impulse = abs(jt) < j * static_friction ?
			jt * t :
			-j * t * static_friction / 3;

		c.b.velocity -= (1 / c.a.mass) * friction_impulse;
		c.a.velocity += (1 / c.b.mass) * friction_impulse;
	}
}

Collision Physics::get_collision(Entity& a, Entity& b)
{
	Collision collision(a, b);

	float left = a.right() - b.left();
	float right = b.right() - a.left();
	float top = b.top() - a.bottom();
	float bottom = a.top() - b.bottom();

	auto horizontal_point = [&a, &b](float x)
	{
		float _top = a.top() < b.top() ? a.top() : b.top();
		float _bottom = a.bottom() > b.bottom() ? a.bottom() : b.bottom();
		return vec2{ x, (_top - _bottom) / 2 };
	};

	auto vertical_point = [&a, &b](float y)
	{
		float _left = a.left() > b.left() ? a.left() : b.left();
		float _right = a.right() < b.right() ? a.right() : b.right();
		return vec2{ _right - _left, y };
	};

	if (left < right && left < top && left < bottom)
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
	if (e.collider_type == Entity::ColliderType::static_body)
		return;

	if (e.use_gravity)
		e.velocity.y -= gravitational_constant * e.frame_ptr->delta_time;

	e.position += e.velocity * e.frame_ptr->delta_time;
}

void Physics::check_collisions(EntityCollection& entities)
{
	const size_t size = entities.count();
	for (uint32_t curri = 0; curri < size - 1; curri++)
	{
		for (uint32_t nexti = curri + 1; nexti < size; nexti++)
		{
			Entity* curr_entity = entities[curri];
			Entity* next_entity = entities[nexti];

			if (curr_entity->left() < next_entity->right()
				&& curr_entity->right() > next_entity->left()
				&& curr_entity->bottom() < next_entity->top()
				&& curr_entity->top() > next_entity->bottom())
			{
				Physics::resolve_collision(*curr_entity, *next_entity);
			}

		}
	}
}