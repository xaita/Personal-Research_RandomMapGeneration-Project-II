#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "RandomDungeonGenerator.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	SDL_Texture* floorTex;
	SDL_Texture* wallTex;
	SDL_Texture* doorTex;
	SDL_Texture* fondoTex;

	// Todo
	Dungeon dungeon;
private:




};

#endif // __SCENE_H__