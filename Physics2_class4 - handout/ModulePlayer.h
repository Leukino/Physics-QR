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

public:
	void Restart();
	void LiveLoss() { lives--; };

	fPoint initial_pos = {452,740};

	int lives = 3;
	int score = 0;
};