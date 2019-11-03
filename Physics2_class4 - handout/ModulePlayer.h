#pragma once
#include "Module.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	void Restart();
	void LiveLoss();

	b2Vec2 initial_pos = {452,740};
	b2Vec2 up = { 0,-300 };
	b2Vec2 zero = { 0,0 };

	PhysBody* ball = new PhysBody();
	int lives = 0;
	int score = 0;

	bool alive = false;
	bool started = false;
};