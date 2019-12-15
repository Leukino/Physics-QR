#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

void ModuleSceneIntro::CreateWall(float x, float y, float width, float length)
{
	Cube c(width, 2.5f, length);
	c.SetPos(x+(width/2), 1.25f, y + (length / 2));
	walls[wallcounter] = c;
	Pwalls[wallcounter] = App->physics->AddBody(c, 0.0f);
	wallcounter++;
}

bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	App->audio->LoadFx("jump.wav");
	bool ret = true;

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));

	App->camera->LookAt(vec3(74.9f, 0.0f, 75.0f));

	CreateWall(0.0f, 0.0f, 150.0f, 15.0f);
	CreateWall(0.0f, 10.0f, 15.0f, 140.0f);
	CreateWall(135.0f, 10.0f, 15.0f, 140.0f);
	CreateWall(10.0f, 135.0f, 140.0f, 15.0f);

	CreateWall(20.0f, 20.0f, 20.0f, 110.0f);
	CreateWall(50.0f, 100.0f, 50.0f, 40.0f);
	CreateWall(40.0f, 70.0f, 70.0f, 20.0f);
	CreateWall(110.0f, 20.0f, 20.0f, 110.0f);
	CreateWall(50.0f, 10.0f, 10.0f, 50.0f);
	CreateWall(70.0f, 20.0f, 10.0f, 50.0f);
	CreateWall(90.0f, 10.0f, 10.0f, 50.0f);

	Cube cealling(140.0f, 2.5f, 140.0f);
	cealling.SetPos(10.0f + (140.0f / 2), 5.0f, 10.0f + (140.0f / 2));
	App->physics->AddBody(cealling, 0.0f);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//player1position = App->player->vehicle1->GetPosition();
	//player2position = App->player->vehicle2->GetPosition();
	//vec3 sharedpos(player1position.x + player2position.x, 0, player1position.z + player2position.z);
	//App->camera->Look({ 0, 50, 0 }, sharedpos / 2);
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < MAX_WALLS; i++)
	{
		walls[i].Render();
	}
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

