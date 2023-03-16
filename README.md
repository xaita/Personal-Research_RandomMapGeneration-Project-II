## Random Map Generation

My name is Xiao Shan Costajussa, student of  at UPC CITM. 

This content is generated for the second year's subject Project II

### About the research: What are Random Generated Maps?

The procedural generation is the process to generate content algorithmically rather than manual. 

In other words, random generated maps are created by computers instead of persons.

|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/random_map_1.gif" alt="Height Formula" width="600" height="300">|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/random_map_2.gif" alt="Height Formula" width="600" height="300">|
|:---:|:---:|
|Random generated terrain from Seeds of Residence (2018)|Random generated dungeon from a videgame made in Phaser 3 Engine|

### Examples in video game:


|_ScreenShot_ | _Game_| _Info_ |
| --------- | ------- | ---------- |
|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/Terraria.jpg" alt="Height Formula" width="400" height="300">| Terraria (2011) | Terraria is a very complete game in the sense of random world generation, since it has random terrain generation following a pattern of biomes, generation of caves with different heights, and a generation of dungeons.   |
|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/Issac.jpg" alt="Height Formula" width="400" height="300">  | The Binding of Isaac Repentance (2021)  | Isaac on the other hand only has a random generation of dungeons, which behaves a little differently than usual, since it does not generate the rooms randomly, but it already has them predefined and what changes is the combination between them. |
|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/no_mans_sky.PNG" alt="Height Formula" width="400" height="300"> | No Man's Sky (2016) | No Man's Sky is a special case, since it is not only dedicated to procedurally create terrains or biomes, but it generates planets and entire systems and in exorbitant quantities with millions and millions of planets and stars, where the generation of these planets depends on the distance they have from the sun and the size of the planet, to know what resource will be on that planet and thus conclude its viability to produce life and what kind of life. | 
***

### Why do we want random generated maps?
Nowadays, a video game demands several materials to compose complex and large scene, carrying high costs in their content development. Developers use these algorithms to
reduce the amount of time taken to create a game or certain features of it.

Also, from the player perspective, it is more enjoyable to play a diferent map every time. So replayability is also a very important factor.


### Characteristics of a generated random map
#### Why is it good?

- It saves development time and money
- It can save on memory usage
- It increases replayability
- It creates dynamic content
- It creates a large variety of options

#### Why is it bad?
- You may generate an unplayable world
- Worlds can feel repetitive


### When should we use a random generated map?
- Open world or survival games
- Exploration games.
- If your main mechanic is the repetition like in a roge-like/lite game.

|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/minecraft_cover.png" alt="Height Formula" width="300" height="500">|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/Spore.jpg" alt="Height Formula" width="300" height="500">|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/Rogue Legacy 2.jpg" alt="Height Formula" width="300" height="500">|
|:---:|:---:|:---:|
|Minecraft (2009) |Spore (2008)|Rogue Legacy 2 (2020)|


### How can we create this algorithm?


The tricky part in procedural generation is not to make things random, but to make them in a consistent way despite it’s randomness. 
There are two types of maps:

|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/outdoor_map.gif" alt="Height Formula" width="600" height="340">|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/indoor_map.gif" alt="Height Formula" width="600" height="340">|
|:---:|:---:|
|Outdoor map |Indoor map|

We will use different procedures to achieve each of them.

#### Indoor map-Dungeon Generation 

There are many ways to generate algorithms to generate dungeons, and this is just one of many, but it has served as a basis for me to generate my dunegon.

If you want to see exactly my implementation you can find it in my [Github repository](https://github.com/xaita/RandomMapGeneration-Project-II).

|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/Ejemplo dungeon.png" alt="Height Formula" width="801" height="403">|
|:---:|
|Result|


#### What does this algorithm ?

	  1.Fill the whole map with solid earth

	  2.Dig out a single room in the centre of the map

	  3.Pick a wall of any room

	  4.Decide upon a new feature to build

	  5.See if there is room to add the new feature through the chosen wall

	  6.If yes, continue. If no, go back to step 3

	  7.Add the feature through the chosen wall

	  8.Go back to step 3, until the dungeon is complete

Clik [here](http://www.roguebasin.com/index.php/Dungeon-Building_Algorithm) if you want more information.

```ruby
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

struct Rect
{
	int x, y;
	int width, height;
};

class Dungeon
{
public:
	enum Tile
	{
		Unused		= ' ',
		Floor		= '.',
		Corridor	= ',',
		Wall		= '#',
		ClosedDoor	= '+',
		OpenDoor	= '-',
		UpStairs	= '<',
		DownStairs	= '>'
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
	Dungeon(int width, int height)
		: _width(width)
		, _height(height)
		, _tiles(width * height, Unused)
		, _rooms()
		, _exits()
	{
	}

	void generate(int maxFeatures)
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

		if (!placeObject(UpStairs))
		{
			std::cout << "Unable to place up stairs.\n";
			return;
		}

		if (!placeObject(DownStairs))
		{
			std::cout << "Unable to place down stairs.\n";
			return;
		}

		for (char& tile : _tiles)
		{
			if (tile == Unused)
				tile = '.';
			else if (tile == Floor || tile == Corridor)
				tile = ' ';
		}
	}

	void print()
	{
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
				std::cout << getTile(x, y);

			std::cout << std::endl;
		}
	}

private:
	char getTile(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= _width || y >= _height)
			return Unused;

		return _tiles[x + y * _width];
	}

	void setTile(int x, int y, char tile)
	{
		_tiles[x + y * _width] = tile;
	}

	bool createFeature()
	{
		for (int i = 0; i < 1000; ++i)
		{
			if (_exits.empty())
				break;

			// choose a random side of a random room or corridor
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

	bool createFeature(int x, int y, Direction dir)
	{
		static const int roomChance = 50; // corridorChance = 100 - roomChance

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

		if (getTile(x + dx, y + dy) != Floor && getTile(x + dx, y + dy) != Corridor)
			return false;

		if (randomInt(100) < roomChance)
		{
			if (makeRoom(x, y, dir))
			{
				setTile(x, y, ClosedDoor);

				return true;
			}
		}

		else
		{
			if (makeCorridor(x, y, dir))
			{
				if (getTile(x + dx, y + dy) == Floor)
					setTile(x, y, ClosedDoor);
				else // don't place a door between corridors
					setTile(x, y, Corridor);

				return true;
			}
		}

		return false;
	}

	bool makeRoom(int x, int y, Direction dir, bool firstRoom = false)
	{
		static const int minRoomSize = 3;
		static const int maxRoomSize = 6;

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
		

	bool placeRect(const Rect& rect, char tile)
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

	bool placeObject(char tile)
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

private:
	int _width, _height;
	std::vector<char> _tiles;
	std::vector<Rect> _rooms; // rooms for place stairs or monsters
	std::vector<Rect> _exits; // 4 sides of rooms or corridors
};

int main()
{
	Dungeon d(79, 24);
	d.generate(50);
	d.print();

	std::cout << "Press Enter to quit... ";
	std::cin.get();

	return 0;
}

```

#### Outdoor map-Perlin Noise 

A common way to generate 2D maps is to use a noise function, such as perlin noise. This is what the noise function looks like:

|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/noise.png" alt="Height Formula" width="801" height="403">|
|:---:|
|Perlin Noise|

To create a random terrain we need to store a seed, a math formula and set a frequency.

We assign each location on the map a number from 0.0 to 1.0. In this image, 0.0 is black and 1.0 is white.

|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/perlin_noise_terrain.png" alt="Height Formula" width="801" height="403">|

Once you have this values, you interpretate numbers to spawn terrains.

 



|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/todo_2.png" alt="Height Formula" width="500" height="400">|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/todo_2_2.PNG"  alt="Height Formula" width="500" height="400">|
|:---:|:---:|
|1|2|


```ruby
if (value > 0 && value < 0.2) app->render->DrawTexture(app->scene->forestTex,pos.x, pos.y, NULL, scale);
else if (value > 0.2 && value < 0.4) app->render->DrawTexture(app->scene->grassTex, pos.x, pos.y, NULL, scale);
else if (value > 0.4 && value < 0.6)  app->render->DrawTexture(app->scene->sandTex, pos.x, pos.y, NULL, scale);
else if (value > 0.6 && value < 1)  app->render->DrawTexture(app->scene->waterTex, pos.x, pos.y, NULL, scale);
```


|<img src="https://raw.githubusercontent.com/xaita/RandomMapGeneration-Project-II/main/docs/Assets/todo_3.PNG" alt="Height Formula" width="500" height="400">|
|:---:|
|Final result|

If you more information to how implement this algorithm check this video (https://www.youtube.com/watch?v=jv6YT9pPIHw)

## Bibliography
Library used: [Fast Portable Noise Library created by Jordan Peck](https://github.com/Auburn/FastNoiseLite)

A video about [2D Terrain Generation using Perlin Noise](https://www.youtube.com/watch?v=jv6YT9pPIHw) published by [Barney](https://github.com/BarneyWhiteman/CodingChallenges)

[Procedural generation applied to a video game level design](https://upcommons.upc.edu/bitstream/handle/2099.1/26632/109439.pdf?sequence=1&isAllowed=y)  written by Albert Carrión Díaz

Addition information to understand [procedural dungeon](https://www.freecodecamp.org/news/how-to-make-your-own-procedural-dungeon-map-generator-using-the-random-walk-algorithm-e0085c8aa9a/)

