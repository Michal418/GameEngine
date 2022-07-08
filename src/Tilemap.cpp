#include "Tilemap.hpp"
#include "Graphics/Texture.hpp"
#include "Logger.hpp"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Tileset.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>


#include <iostream>


Tilemap::Tilemap(const Tilemap & t)
	: width(t.width), height(t.height), size(t.size), map(new Tile* [t.height]), sprite(t.sprite),
	image_size(t.image_size), tile_size(t.tile_size), objects(t.objects)
{
	std::cout << "Tilemap::Tilemap\n";

	for (int y = 0; y < height; y++) {
		map[y] = new Tile[width];
		for (int x = 0; x < width; x++) {
			map[y][x] = t.map[y][x];
		}
	}
}

Tilemap::Tilemap(int height, int width, int size)
	: height(height), width(width), size(size), map(new Tile* [height]),
	image_size(), tile_size()
{
	std::cout << "Tilemap::Tilemap\n";

	//map = std::vector<std::vector<Tile>>(height, std::vector<Tile>(width, { false, L"" }));
	for (int i = 0; i < height; i++) {
		map[i] = new Tile[width];
	}
}

Tilemap::~Tilemap()
{
	std::cout << "Tilemap::~Tilemap\n";

	cleanup();
}

Tilemap& Tilemap::operator=(const Tilemap& t)
{
	std::cout << "Tilemap::operator=\n";


	cleanup();
	objects = t.objects;
	sprite = t.sprite;
	width = t.width;
	height = t.height;
	size = t.size;
	image_size = t.image_size;
	tile_size = t.tile_size;
	map = new Tile * [height];
	for (int y = 0; y < height; y++) {
		map[y] = new Tile[width];
		for (int x = 0; x < width; x++) {
			map[y][x] = t.map[y][x];
		}
	}
	return *this;
}

void Tilemap::set_tile(int x, int y, Tile t)
{
	std::cout << "Tilemap::set_tile\n";

	assert(x < width && y < height);
	map[y][x] = t;
}

void Tilemap::update_verticies(const Camera& camera)
{
	std::cout << "Tilemap::update_verticies\n";

	verticies.clear();
	for (int y = max(static_cast<int>(camera.position.y - camera.size.y) / size, 0);
		y < min((camera.position.y + camera.size.y) / size + size, height);
		y++)
	{
		for (int x = max(static_cast<int>(camera.position.x - camera.size.x) / size, 0);
			x < min((camera.position.x + camera.size.x) / size + size, width);
			x++)
		{
			const Tile& t = get_tile(x, y);
			if (t.id == 0) {
				continue;
			}

			int tilesPerRow = image_size.x / tile_size.x;
			float tilex = (t.id - 1) % tilesPerRow * (float)tile_size.x;
			tilex /= image_size.x;

			float tiley = static_cast<float>(t.id - 1) / tilesPerRow * tile_size.y;
			tiley /= image_size.y;

			float hs = (float)tile_size.x / image_size.x;
			float vs = (float)tile_size.y / image_size.y;

			verticies.push_back({ (float)x * size,			(float)y * size + size, (float)tilex,			(float)tiley });
			verticies.push_back({ (float)x * size + size,	(float)y * size + size, (float)tilex + hs,		(float)tiley });
			verticies.push_back({ (float)x * size + size,	(float)y * size,		(float)tilex + hs,		(float)tiley + vs });

			verticies.push_back({ (float)x * size,			(float)y * size + size, (float)tilex,			(float)tiley });
			verticies.push_back({ (float)x * size + size,	(float)y * size,		(float)tilex + hs,		(float)tiley + vs });
			verticies.push_back({ (float)x * size,			(float)y * size,		(float)tilex,			(float)tiley + vs });
		}
	}
}

const Tilemap::TileCollumn Tilemap::operator[](int x)
{
	std::cout << "Tilemap::operator[]\n";

	assert(x >= 0 && x < width);
	return TileCollumn(this, x);
}

void Tilemap::cleanup()
{
	std::cout << "Tilemap::cleanup\n";

	for (int i = 0; i < height; i++) {
		if (map[i]) {
			delete[] map[i];
		}
	}
	if (map) {
		delete[] map;
	}
}

const Tile& Tilemap::get_tile(int x, int y) const
{
	std::cout << "Tilemap::get_tile\n";

	assert(x < width && y < height);
	return map[y][x];
}

int Tilemap::get_width() const
{
	std::cout << "Tilemap::get_width\n";

	return width;
}

int Tilemap::get_height() const
{
	std::cout << "Tilemap::get_height\n";

	return height;
}

int Tilemap::tile_diameter() const
{
	std::cout << "Tilemap::tile_diameter\n";

	return size;
}

Tilemap Tilemap::from_file(std::string fname)
{
	std::cout << "Tilemap::from_file\n";

	tmx::Map map;
	if (!map.load(fname)) {
		ALERT_TEXT("tileset neexistuje");
		throw "tileset neexistuje";
	}

	tmx::TileLayer& layer = map.getLayers()[0].get()->getLayerAs<tmx::TileLayer>();
	tmx::Tileset tileset = map.getTilesets()[0];

	const std::string& img = tileset.getImagePath();

	const tmx::Vector2u& size = layer.getSize();

	Tilemap tilemap = Tilemap((int)size.y, (int)size.x, 32);

	for (auto& object : map.getLayers()[1].get()->getLayerAs<tmx::ObjectGroup>().getObjects()) {
		Object o;
		o.name = object.getName();
		o.position = vec2{ object.getPosition().x, size.y * map.getTileSize().y - object.getPosition().y };
		tilemap.objects.push_back(o);
	}

	tilemap.image_size.x = tileset.getImageSize().x;
	tilemap.image_size.y = tileset.getImageSize().y;
	tilemap.tile_size.x = map.getTileSize().x;
	tilemap.tile_size.y = map.getTileSize().y;

	wchar_t buff[50];
	DWORD len = 50;
	GetUserNameW(buff, &len);
	std::wstring name = std::wstring(buff);

	tilemap.sprite.path =std::wstring(img.begin(), img.end());
	tilemap.sprite.size = 1;

	auto& tiles = layer.getTiles();
	for (unsigned int y = 0; y < size.y; y++) {
		for (unsigned int x = 0; x < size.x; x++) {
			auto& tile = tiles[(size.y - 1 - y) * size.x + x];
			tilemap.set_tile(x, y, Tile{ tile.ID != 0, (int16_t)tile.ID });
		}
	}

	return tilemap;
}

Tile::Tile(bool solid, int16_t id)
	: solid(solid), id(id)
{
	std::cout << "Tile::Tile\n";
}

Tile::Tile()
	: solid(false), id(-1)
{
	std::cout << "Tile::Tile\n";
}

Tilemap::TileCollumn::TileCollumn(Tilemap* parent, int collumn)
	: parent(parent), coll(collumn) {
	std::cout << "Tilemap::TileCollumn::TileCollumn\n";
}

const Tile& Tilemap::TileCollumn::operator[](int row) const {
	std::cout << "Tilemap::TileCollumn::operator[]\n";

	assert(row >= 0 && row < parent->height);
	return parent->map[row][coll];
}