#include "RandomDungeonGenerator.h"
#include "App.h"
#include "Map.h"
#include "Log.h"
#include"Scene.h"


#include <random>
#include <vector>
#include <iostream>




namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
		return dist(mt);
	}

	int randomInt(int min, int max) // inclusive min/max
	{
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	bool randomBool(double probability = 0.5)
	{
		std::bernoulli_distribution dist(probability);
		return dist(mt);
	}
}




Dungeon::~Dungeon()
{
}

bool Dungeon::Awake()
{

	LOG("Loading Procedural Map Data");
	bool ret = true;



	return ret;
}



void Dungeon :: generate(int maxFeatures)
{
		// place the first room in the center
		if (!makeRoom(_width / 2, _height / 2, static_cast<Direction>(randomInt(4), true)))
		{
			std::cout << "Unable to place the first room.\n";
			return;
		}

		// we already placed 1 feature (the first room)
		for (int i = 1; i < maxFeatures; ++i)
		{
			if (!createFeature())
			{
				std::cout << "Unable to place more features (placed " << i << ").\n";
				break;
			}
		}

	


	}

	void Dungeon:: render()
	{

		//Todo 3
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x) {
				iPoint pos = app->map->MapToWorld(x, y);

				switch (getTile(x, y))
				{
				case Tile::Unused:
					app->render->DrawTexture(app->scene->fondoTex, pos.x, pos.y, NULL);
					break;
				case Tile::Wall:
					app->render->DrawTexture(app->scene->wallTex, pos.x, pos.y, NULL);
					break;
				case Tile::Floor:
					app->render->DrawTexture(app->scene->floorTex, pos.x, pos.y, NULL);
					break;
				case Tile::ClosedDoor:
					app->render->DrawTexture(app->scene->doorTex, pos.x, pos.y, NULL);
					break;
				
				};
			}
			
		}
	}


	char Dungeon:: getTile(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= _width || y >= _height)
			return Unused;

		return _tiles[x + y * _width];
	}

	void Dungeon::setTile(int x, int y, char tile)
	{
		_tiles[x + y * _width] = tile;
	}

	bool Dungeon:: createFeature()
	{
		for (int i = 0; i < 1000; ++i)
		{
			if (_exits.empty())
				break;

			// choose a random side of a random room 
			int r = randomInt(_exits.size());
			int x = randomInt(_exits[r].x, _exits[r].x + _exits[r].width - 1);
			int y = randomInt(_exits[r].y, _exits[r].y + _exits[r].height - 1);

			// north, south, west, east
			for (int j = 0; j < DirectionCount; ++j)
			{
				if (createFeature(x, y, static_cast<Direction>(j)))
				{
					_exits.erase(_exits.begin() + r);
					return true;
				}
			}
		}

		return false;
	}

	bool Dungeon:: createFeature(int x, int y, Direction dir)
	{
		static const int roomChance = 50; 

		int dx = 0;
		int dy = 0;

		if (dir == North)
			dy = 1;
		else if (dir == South)
			dy = -1;
		else if (dir == West)
			dx = 1;
		else if (dir == East)
			dx = -1;



		if (randomInt(100) < roomChance)
		{
			if (makeRoom(x, y, dir))
			{
				setTile(x, y, ClosedDoor);

				return true;
			}
		}

	

		return false;
	}

	bool Dungeon:: makeRoom(int x, int y, Direction dir, bool firstRoom)
	{
		//Todo 2
		static const int minRoomSize = 3;
		static const int maxRoomSize = 10;

		Rect room;
		room.width = randomInt(minRoomSize, maxRoomSize);
		room.height = randomInt(minRoomSize, maxRoomSize);

		if (dir == North)
		{
			room.x = x - room.width / 2;
			room.y = y - room.height;
		}

		else if (dir == South)
		{
			room.x = x - room.width / 2;
			room.y = y + 1;
		}

		else if (dir == West)
		{
			room.x = x - room.width;
			room.y = y - room.height / 2;
		}

		else if (dir == East)
		{
			room.x = x + 1;
			room.y = y - room.height / 2;
		}

		if (placeRect(room, Floor))
		{
			_rooms.emplace_back(room);

			if (dir != South || firstRoom) // north side
				_exits.emplace_back(Rect{ room.x, room.y - 1, room.width, 1 });
			if (dir != North || firstRoom) // south side
				_exits.emplace_back(Rect{ room.x, room.y + room.height, room.width, 1 });
			if (dir != East || firstRoom) // west side
				_exits.emplace_back(Rect{ room.x - 1, room.y, 1, room.height });
			if (dir != West || firstRoom) // east side
				_exits.emplace_back(Rect{ room.x + room.width, room.y, 1, room.height });

			return true;
		}

		return false;
	}



	bool Dungeon:: placeRect(const Rect& rect, char tile)
	{
		if (rect.x < 1 || rect.y < 1 || rect.x + rect.width > _width - 1 || rect.y + rect.height > _height - 1)
			return false;

		for (int y = rect.y; y < rect.y + rect.height; ++y)
			for (int x = rect.x; x < rect.x + rect.width; ++x)
			{
				if (getTile(x, y) != Unused)
					return false; // the area already used
			}

		for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
			for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x)
			{
				if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width || y == rect.y + rect.height)
					setTile(x, y, Wall);
				else
					setTile(x, y, tile);
			}

		return true;
	}

	bool Dungeon:: placeObject(char tile)
	{
		if (_rooms.empty())
			return false;

		int r = randomInt(_rooms.size()); // choose a random room
		int x = randomInt(_rooms[r].x + 1, _rooms[r].x + _rooms[r].width - 2);
		int y = randomInt(_rooms[r].y + 1, _rooms[r].y + _rooms[r].height - 2);

		if (getTile(x, y) == Floor)
		{
			setTile(x, y, tile);

			// place one object in one room (optional)
			_rooms.erase(_rooms.begin() + r);

			return true;
		}

		return false;
	}




