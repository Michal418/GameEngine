#pragma once
#include <vector>
#include <assert.h>
#include  "Graphics/Vertex.hpp"
#include "../src/Scene/Entity.hpp"

struct Rigidbody;

struct Tile
{
	bool solid;
	int16_t id;

	Tile(bool solid, int16_t id);
	Tile();
};

struct Object
{
	std::string name;
	vec2 position;
};

class Tilemap
{
public:
	Tilemap() = default;
	Tilemap(const Tilemap& t);
	Tilemap(int height, int width, int size);
	~Tilemap();
	Tilemap& operator=(const Tilemap& t);

public:
	void set_tile(int x, int y, Tile t);
	const Tile& get_tile(int x, int y) const;
	int get_width() const;
	int get_height() const;
	int tile_diameter() const;

	static Tilemap from_file(std::string fname);

	void update_verticies(const Camera& cam);

	struct TileCollumn
	{
		Tilemap* parent;
		int coll;

		TileCollumn(Tilemap* parent, int collumn);
		const Tile& operator[](int row) const;
	};

	const TileCollumn operator[](int y);

private:
	void cleanup();

public:
	Rigidbody rigidbody;
	std::vector<Vertex> verticies;
	std::vector<Object> objects;
	Sprite sprite;
	struct {
		unsigned int x, y;
	} image_size;
	struct {
		unsigned int x, y;
	} tile_size;

private:
	Tile** map;

	int height, width;
	int size;

	friend struct TileCollumn;
};

