#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
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
	App->audio->PlayMusic("pinball/music.ogg");
	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png");
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	sprites = App->textures->Load("pinball/Sprites.png");

	background = { 0,0,482,857 };

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT+ 50, SCREEN_WIDTH, 50);
	sensorSCORE1 = App->physics->CreateRectangleSensor(175, 45, 25, 25);
	sensorSCORE2 = App->physics->CreateRectangleSensor(233, 54, 25, 25);
	sensorSCORE3 = App->physics->CreateRectangleSensor(295, 49, 25, 25);
	sensorSCORE4 = App->physics->CreateRectangleSensor(248,263, 25, 25);
	sensorSCORE5 = App->physics->CreateRectangleSensor(137,372, 25, 25);
	sensorSCORE6 = App->physics->CreateRectangleSensor(348,371, 25, 25);
	sensorSCORE7 = App->physics->CreateRectangleSensor(248,471, 25, 25);
	App->physics->CreateRectangle(452, 765, 20, 30, b2_staticBody);
	App->physics->CreateRectangle(472, 755, 20, 30, b2_staticBody);
	App->physics->CreateRectangle(432, 755, 20, 30, b2_staticBody);
	/*
																	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                __...----..							   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                             .-'           `-.						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                            /        .---.._  \						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                            |        |   \  \ |						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                             `.      |    | | |        _____		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       hardcode                `     '    | | /    _.-`      `.		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       yikes                    \    |  .'| //'''.'            \	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                 `---'_(`.||.`.`.'    _.`.'''-. \	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                    _(`'.    `.`.`'.-'  \\     \ \	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                   (' .'   `-._.- /      \\     \ |	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                  ('./   `-._   .-|       \\     ||	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                  ('.\ | | 0') ('0 __.--.  \`----'/	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                             _.--('..|   `--    .'  .-.  `. `--..'	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
               _..--..._ _.-'    ('.:|      .  /   ` 0 `   \		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            .'         .-'        `..'  |  / .^.           |		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
           /         .'                 \ '  .             `._		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        .'|             veuras hardcode  `.  \`...____.----._.'		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
      .'.'|         .   a continuació         \ |    |_||_||__|		   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
     //   \         |   estàs advertit  _.-'| |_ `.   \				   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
     ||   |         |                     /\ \_| _  _ |				   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
     ||   |         /.     .              ' `.`.| || ||				   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
     ||   /        ' '     |        .     |   `.`---'/				   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
   .' `.  |       .' .'`.   \     .'     /      `...'				   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 .'     \  \    .'.'     `---\    '.-'   |							   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
)/\ / /)/ .|    \             `.   `.\   \							   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 )/ \(   /  \   |               \   | `.  `-.						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  )/     )   |  |             __ \   \.-`    \						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
         |  /|  )  .-.      //' `-|   \  _   /						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        / _| |  `-'.-.\     ||    `.   )_.--'						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
        )  \ '-.  /  '|     ''.__.-`\  | 							   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       /  `-\  '._|--'               \  `.							   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       \    _\                       /    `---.						   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       /.--`  \                      \    .''''\					   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       `._..._|                       `-.'  .-. |					   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                                        '_.'-./.'					   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
																	   AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	*/
	
	int exteriorchain[40*2] = //chain 1 (exterior)
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
	
	int interiordreta[26*2] = //chain 2 (paleta dreta container + interior dreta)
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
	};

	int paletaesquerra[7*2] = //chain 3 (paleta esquerra container
	{
	171, 789,
	54, 756,
	54, 627,
	59, 627,
	59, 739,
	169, 772,
	171, 789
	};
	
	int triangleesquerra[4*2] =	//chain 4 (triangle esquerra)
	{
	107, 621,
	147, 717,
	105, 704,
	107, 621
	};
	
	int triangledret[4*2] =	//chain 5 (triangle dret)
	{
	375, 621,
	335, 717,
	376, 704,
	375, 621
	};
	int interiorcurvaesq[23 * 2] = //chain 6 (interior curva esquerra)
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
	};
	int interiorcentral[33 * 2] = //chain 7 (interior curva central)
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
	};
	
	int lpalettechain[4 * 2] =
	{
		10, 1, 63, 53, 1, 22, 10, 1
	};

	App->physics->CreateChain(0, 0, exteriorchain, 40 * 2, b2_staticBody, nullptr);
	App->physics->CreateChain(0, 0, interiordreta, 26 * 2, b2_staticBody, nullptr);
	App->physics->CreateChain(0, 0, paletaesquerra, 7 * 2, b2_staticBody, nullptr);
	App->physics->CreateChain(0, 0, triangleesquerra, 4 * 2, b2_staticBody, nullptr);
	App->physics->CreateChain(0, 0, triangledret, 4 * 2, b2_staticBody, nullptr);
	App->physics->CreateChain(0, 0, interiorcurvaesq, 23 * 2, b2_staticBody, nullptr);
	App->physics->CreateChain(0, 0, interiorcentral, 33 * 2, b2_staticBody, nullptr);
	b2MassData* massdata = new b2MassData();
	//massdata->center.Set(lpalettechain[0] + 3, lpalettechain[1] + 14);
	massdata->I = 0;
	massdata->mass = 1;
	paletal = App->physics->CreateRectangle(220, 776, 55, 20, b2_dynamicBody);
	paletar = App->physics->CreateRectangle(402, 776, 55, 20, b2_dynamicBody);
	//paletal = App->physics->CreateChain(0, 0, lpalettechain, 4 * 2, b2_dynamicBody, massdata);
	//paletal->body->SetGravityScale(0);

	paletal_joint = App->physics->CreateCircle(200, 800, 2, b2_staticBody);
	paletar_joint = App->physics->CreateCircle(275, 800, 2, b2_staticBody);
	//left
	b2RevoluteJointDef revoluteJointDef_left;
	revoluteJointDef_left.bodyA = paletal_joint->body;
	revoluteJointDef_left.bodyB = paletal->body;
	revoluteJointDef_left.collideConnected = false;

	revoluteJointDef_left.enableLimit = true;
	revoluteJointDef_left.lowerAngle = -25 * DEGTORAD;
	revoluteJointDef_left.upperAngle =  20 * DEGTORAD;

	revoluteJointDef_left.localAnchorA.Set(0, 0);
	revoluteJointDef_left.localAnchorB.Set(PIXEL_TO_METERS(-3), PIXEL_TO_METERS(0));
	b2RevoluteJoint* left_flipper_joint;
	left_flipper_joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&revoluteJointDef_left);
	//right
	b2RevoluteJointDef revoluteJointDef_right;
	revoluteJointDef_right.bodyA = paletar_joint->body;
	revoluteJointDef_right.bodyB = paletar->body;
	revoluteJointDef_right.collideConnected = false;

	revoluteJointDef_right.enableLimit = true;
	revoluteJointDef_right.lowerAngle = -25 * DEGTORAD;
	revoluteJointDef_right.upperAngle = 20 * DEGTORAD;

	revoluteJointDef_right.localAnchorA.Set(0, 0);
	revoluteJointDef_right.localAnchorB.Set(PIXEL_TO_METERS(3), PIXEL_TO_METERS(0));
	b2RevoluteJoint* right_flipper_joint;
	right_flipper_joint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&revoluteJointDef_right);

	delete massdata;

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
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->audio->PlayMusic("pinball/hidden_music.ogg");
	paletal->body->ApplyAngularImpulse(0.5f, true);
	paletar->body->ApplyAngularImpulse(-0.5f, true);
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
		paletal->body->ApplyAngularImpulse(-2, true);
	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
		paletar->body->ApplyAngularImpulse(2, true);

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(452, 740, 10, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}
	
	LOG("angle %f", paletar->body->GetAngle() * RADTODEG);		
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		paletal->body->SetAngularVelocity(0);
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
	
	//App->audio->PlayFx(bonus_fx);
	PhysBody* body = bodyB;
	if (body == 
		App->scene_intro->sensorSCORE1||
		App->scene_intro->sensorSCORE2||
		App->scene_intro->sensorSCORE3||
		App->scene_intro->sensorSCORE4||
		App->scene_intro->sensorSCORE5||
		App->scene_intro->sensorSCORE6||
		App->scene_intro->sensorSCORE7)
	{
		App->player->score += 10;
		App->audio->PlayFx(bonus_fx);
	}
}
