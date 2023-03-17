#ifndef __RandomDungeonGenerator__
#define __RandomDungeonGenerator__

#include "Module.h"

#include <random>
#include <vector>
#include <iostream>



struct Rect
{
	int x, y;
	int width, height;
};

class Dungeon : public Module
{
public:
	enum Tile
	{
		Unused,
		Floor, 
		Wall, 
		ClosedDoor, 
		 
	
	};

	enum Direction
	{
		North,
		South,
		West,
		East,
		DirectionCount
	};

public:

	

	Dungeon(int width=80, int height=45)
		: _width(width)
		, _height(height)
		, _tiles(width* height, Unused)
		, _rooms()
		, _exits()
	{
	}



	~Dungeon();

	bool Awake();

	void generate(int maxFeatures);
	

	void render();
		


	char getTile(int x, int y) const;


	void setTile(int x, int y, char tile);


	bool createFeature();


	bool createFeature(int x, int y, Direction dir);


	bool makeRoom(int x, int y, Direction dir, bool firstRoom = false);


	/*bool makeCorridor(int x, int y, Direction dir);*/
	

	bool placeRect(const Rect& rect, char tile);


	bool placeObject(char tile);

	
	

private:
	int _width, _height;
	std::vector<char> _tiles;
	std::vector<Rect> _rooms; // rooms for place stairs or monsters
	std::vector<Rect> _exits; // 4 sides of rooms or corridors
};



#endif