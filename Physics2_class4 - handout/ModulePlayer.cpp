#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	App->physics->CreateRectangle(452, 765, 20, 30, b2_staticBody);
	LOG("Loading player");
	return true;
}

void ModulePlayer::Restart()
{
	lives = 3;
	score = 0;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	return UPDATE_CONTINUE;
}



