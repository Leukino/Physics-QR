#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	PhysBody* sensorSCORE1;
	PhysBody* sensorSCORE2;
	PhysBody* sensorSCORE3;
	PhysBody* sensorSCORE4;
	PhysBody* sensorSCORE5;
	PhysBody* sensorSCORE6;
	PhysBody* sensorSCORE7;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	SDL_Texture* sprites;
	SDL_Rect background;

	PhysBody* paletal;
	PhysBody* paletal_joint;
	PhysBody* paletar;
	PhysBody* paletar_joint;

	SDL_Texture* paletal_tex;
	SDL_Texture* paletar_tex;
};
