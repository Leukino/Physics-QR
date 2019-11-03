#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
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
	int pox = 159;
	int poy = 175;
	ball = App->physics->CreateCircle(initial_pos.x, initial_pos.y, radius, b2_dynamicBody);
	ball->listener = App->scene_intro;
	bouncers[0] = App->physics->CreateCircle(pox, poy, 20, b2_staticBody);
	pox += 164;
	bouncers[1] = App->physics->CreateCircle(pox, poy, 20, b2_staticBody);
	pox = 245;
	poy = 134;
	bouncers[2] = App->physics->CreateCircle(pox, poy, 20, b2_staticBody);

	sprite = App->textures->Load("pinball/Sprites.png");
	ball_spr = { 483,1,25,25 };
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
	if (METERS_TO_PIXELS(ball->body->GetPosition().y) > 836 || METERS_TO_PIXELS(ball->body->GetPosition().x) > 452)
	{
		LiveLoss();
	}

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
	SDL_Rect r = {0,0,30,30};

	App->renderer->Blit(sprite, METERS_TO_PIXELS(ball->body->GetPosition().x) - 13, METERS_TO_PIXELS(ball->body->GetPosition().y) -13, &ball_spr);

	LOG("%d   %d", METERS_TO_PIXELS(ball->body->GetPosition().x), METERS_TO_PIXELS(ball->body->GetPosition().y));
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	score = 69;
}

