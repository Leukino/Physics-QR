#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "SDL/include/SDL_scancode.h"
#include <sstream>
#include <stdlib.h>


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	int radius = 10;
	ball = App->physics->CreateCircle(initial_pos.x, initial_pos.y, radius);
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

void ModulePlayer::LiveLoss()
{
	lives--;
	started = false;
	ball->body->SetTransform({ PIXEL_TO_METERS(initial_pos.x),PIXEL_TO_METERS(initial_pos.y) }, 0.0f);
	ball->body->SetLinearVelocity(zero);
}

update_status ModulePlayer::Update()
{
	if (alive)
	{
		char t[10];
		SDL_itoa(score, t, 10);
		App->window->SetTitle(t);
	}
	else
	{
		App->window->SetTitle("Press SPACE to start");
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !started)
	{
		ball->body->ApplyForceToCenter(up, true);
		started = true;
		if (!alive)
		{
			lives = 3;
			alive = true;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		LiveLoss();
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		score++;
	}
	if (lives == 0)
	{
		score = 0;
		lives = 3;
		alive = false;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	score = 69;
}

