#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png");
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	sprites = App->textures->Load("pinball/Sprites.png");

	background = { 0,0,482,857 };

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT+ 50, SCREEN_WIDTH, 50);
	App->physics->CreateRectangle(452, 765, 20, 30, b2_staticBody);
	App->physics->CreateRectangle(472, 755, 20, 30, b2_staticBody);
	App->physics->CreateRectangle(432, 755, 20, 30, b2_staticBody);

	//chains
	
	//chain 1 (exterior)
	int exteriorchain[100] =
	{
	195, 856,
	195, 847,
	7, 793,
	7, 593,
	10, 584,
	16, 573,
	26, 564,
	35, 561,
	53, 561,
	54, 549,
	59, 539,
	69, 530,
	79, 525,
	89, 524,
	13, 294,
	7, 259,
	7, 219,
	12, 186,
	26, 147,
	44, 113,
	66, 83,
	90, 58,
	119, 37,
	152, 21,
	178, 12,
	207, 7,
	237, 5,
	258, 5,
	290, 9,
	312, 15,
	342, 27,
	364, 40,
	389, 57,
	406, 72,
	424, 93,
	439, 115,
	453, 139,
	464, 168,
	475, 215,
	475, 849,
	};
	

	
/*
	chain 2 (paleta dreta container + interior dreta)
	{
	286, 856, 
	286, 847, 
	422, 807, 
	422, 759, 
	308, 791, 
	308, 774, 
	422, 736, 
	422, 455, 
	367, 433, 
	415, 289, 
	421, 264, 
	422, 235, 
	418, 204, 
	409, 174, 
	395, 145, 
	377, 120, 
	356, 99, 
	331, 81, 
	335, 75, 
	358, 91, 
	379, 111, 
	399, 136, 
	413, 163, 
	424, 192, 
	428, 223,
	428, 846
	}
	chain 3 (paleta esquerra container
	{
	171, 789, 
	54, 756, 
	54, 627, 
	59, 627, 
	59, 739, 
	169, 772, 
	171, 789
	}
	chain 4 (triangle esquerra)
	{
	107, 621, 
	147, 717, 
	105, 704,
	107, 621
	}
	chain 5 (triangle dret)
	{
	375, 621, 
	335, 717, 
	376, 704, 
	375, 621
	}
	chain 6 (interior curva esquerra)
	{
	133, 516, 
	60, 289, 
	54, 262, 
	52, 233, 
	56, 205, 
	63, 180, 
	73, 155, 
	88, 129, 
	105, 107, 
	124, 90, 
	143, 78, 
	146, 84, 
	124, 100, 
	106, 117, 
	93, 132, 
	82, 152, 
	73, 171, 
	65, 196, 
	61, 221, 
	61, 248, 
	64, 277, 
	140, 511, 
	133, 516
	}
	chain 7 (interior curva central)
	{
	364, 293, 
	373, 269, 
	376, 239, 
	374, 215, 
	365, 183, 
	352, 162, 
	331, 137, 
	304, 119, 
	279, 109, 
	251, 103, 
	226, 102, 
	195, 109, 
	172, 121, 
	151, 136, 
	136, 152, 
	124, 170, 
	113, 191, 
	121, 193, 
	133, 166, 
	150, 146, 
	167, 132, 
	195, 118, 
	222, 110, 
	256, 110, 
	286, 119, 
	318, 135, 
	340, 156, 
	355, 182, 
	366, 210, 
	369, 234, 
	368, 257, 
	358, 284, 
	364, 293
	}
	*/

	
	App->physics->CreateChain(0, 0, exteriorchain, 80, b2_staticBody);
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
		LOG("Move left lol");
	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
		LOG("Move right lol");

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(452, 740, 10));
		circles.getLast()->data->listener = this;
	}

	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	
	//HERE WE GONNA BLIT SHITS
	App->renderer->Blit(sprites, 0, 0, &background);
	LOG("M p meters: %d, %d", App->input->GetMouseX(), App->input->GetMouseY());
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
