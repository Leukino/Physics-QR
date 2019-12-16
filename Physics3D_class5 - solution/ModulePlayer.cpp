#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle1(NULL), vehicle2(NULL)
{
	turn1 = acceleration1 = brake1 = turn2 = acceleration2 = brake2 = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

void ModulePlayer::Render()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle1 = App->physics->AddVehicle(car);
	vehicle2 = App->physics->AddVehicle(car);
	vehicle1->SetPos(18, 2, 30);
	vehicle2->SetPos(16, 2, 30);
	vehicle1->info.color = Blue;
	vehicle2->info.color = Red;

	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn1 = acceleration1 = brake1 = turn2 = acceleration2 = brake2 = 0.0f;
	//PLAYER1 LUL

	if ((vehicle1->GetPosition().x > 128) && (checks1 == 0 || checks1 == 2 || checks1 == 4))
		checks1++;
	if ((vehicle2->GetPosition().x > 128) && (checks2 == 0 || checks2 == 2 || checks2 == 4))
		checks2++;
	if ((vehicle1->GetPosition().x < 18) && (checks1 == 1 || checks1 == 3 || checks1 == 5))
		checks1++;
	if ((vehicle2->GetPosition().x < 18) && (checks2 == 1 || checks2 == 3 || checks2 == 5))
		checks2++;

	LOG("%d %d", checks1, checks2);

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (vehicle1->GetKmh() < 60)
		{
			acceleration1 = MAX_ACCELERATION;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn1 < TURN_DEGREES)
			turn1 +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn1 > -TURN_DEGREES)
			turn1 -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT)
			acceleration1 = -MAX_ACCELERATION / 2;
		else {
			brake1 = BRAKE_POWER;
			App->audio->PlayFx(2);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		vehicle1->Push(0.0f, 2000.0f, 0.0f);
		App->audio->PlayFx(1);
	}
	//PLAYER2 LUL
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (vehicle2->GetKmh() < 60)
		{
			acceleration2 = MAX_ACCELERATION;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn2 < TURN_DEGREES)
			turn2 += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn2 > -TURN_DEGREES)
			turn2 -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			acceleration2 = -MAX_ACCELERATION/2;
		else
		{
			brake2 = BRAKE_POWER;
			App->audio->PlayFx(2);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		vehicle2->Push(0.0f, 2000.0f, 0.0f);
		App->audio->PlayFx(1);
	}

	vehicle1->ApplyEngineForce(acceleration1);
	vehicle1->Turn(turn1);
	vehicle1->Brake(brake1);

	vehicle2->ApplyEngineForce(acceleration2);
	vehicle2->Turn(turn2);
	vehicle2->Brake(brake2);

	vehicle1->Render();
	vehicle2->Render();

	char title[80];
	sprintf_s(title, "Player 1 %d/3 laps   Player 2 %d/3 laps ", checks1 / 2, checks2 / 2);

	if (App->player->checks1 == 6)
	{
		sprintf_s(title, "PLAYER 1 WINS PLAYER 1 WINS PLAYER 1 WINS PLAYER 1 WINS PLAYER 1 WINS");
		App->audio->PlayMusic("ff7.ogg", 0.0f);
	}
	else if (App->player->checks2 == 6)
	{
		sprintf_s(title, "PLAYER 2 WINS PLAYER 2 WINS PLAYER 2 WINS PLAYER 2 WINS PLAYER 2 WINS");
		App->audio->PlayMusic("ff7.ogg", 0.0f);
	}
	App->window->SetTitle(title);
	vec3 vecc1 = vehicle1->GetPosition();
	vec3 vecc2 = vehicle2->GetPosition();
	LOG("P1: %f, %f, %f", vecc1.x, vecc1.y, vecc1.z);
	//LOG("P2: %f, %f, %f", vecc2.x, vecc2.y, vecc2.z);
	return UPDATE_CONTINUE;
}

