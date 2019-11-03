#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
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

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(initial_pos.x), PIXEL_TO_METERS(initial_pos.y));

	b2Body* b = App->physics->world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
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
	this->pbody->body->SetLinearVelocity(zero);
	this->pbody->body->SetTransform({ PIXEL_TO_METERS(initial_pos.x),PIXEL_TO_METERS(initial_pos.y)}, 0.0f);
	
}

update_status ModulePlayer::Update()
{
	char t[10];
	SDL_itoa(score, t, 10); 
	App->window->SetTitle(t);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && !started)
	{
		this->pbody->body->ApplyForce(up, up,1);
		started = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		LiveLoss();
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		score++;
	}


	return UPDATE_CONTINUE;
}



