#include "Globals.h"
#include "Application.h"
#include "Motor.h"
#include "math.h"
#include"ModuleInput.h"
#include"Collider.h"
#include"ModuleRender.h"
#include"ModuleAudio.h"
#include"ModulePlayer.h"


Motor::Motor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//collisions
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::BOLA][Collider::Type::BOLA] = true;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BOLA][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::BOLA] = true;
	matrix[Collider::Type::LIMITE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::LIMITE] = true;
	
}
Motor::~Motor()
{}

// Load assets
bool Motor::Start()
{
	LOG("Loading player");


	// Set physics properties of the ball

	//ball.surface = 2; // m^2
	//ball.cd = 0.4;
	//ball.cl = 1.2;
	corazonFx = App->audio->LoadFx("Assets/audio/fx/salto.wav");
	// Set initial velocity of the ball

	/*ball.vx = 5.0;
	ball.vy = 5.0;*/



	return true;
}


Ball* Motor::AddBall(double mass, float x, float y, double vx, double vy, double fy, double fx, double ax, double ay, double fgx, double fgy)
{
	// Create new ball
	Ball* thisball = new Ball(mass, x, y, vx, vy, fy, fx, ax, ay, fgx, fgy);

	// Add it to the list
	pelotas.add(thisball);

	// Return ball (why?)
	return thisball;
}
update_status Motor::PreUpdate() {
	for (uint i = 0; i < MAX_COLLIDERS; i++) {
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; i++) {
		if (colliders[i] == nullptr) {
			continue;
		}
		c1 = colliders[i];
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
			if (colliders[k] == nullptr) {
				continue;
			}
			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
				{
					if (c1->listeners[i] != nullptr)
					{
						c1->listeners[i]->OnCollision(c1, c2);

					}

				}
				for (uint i = 0; i < MAX_LISTENERS; ++i)
				{
					if (c2->listeners[i] != nullptr) {
						c2->listeners[i]->OnCollision(c2, c1);

					}

				}
			}
		}
	}
	return UPDATE_CONTINUE;
}
// Update: compute physics
update_status Motor::Update()
{
	//========================================= DrawCollisions===================================================


	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			App->renderer->DrawQuad(colliders[i]->rect, 0, 255, 255);
			break;
		case Collider::Type::PLAYER: // green
			App->renderer->DrawQuad(colliders[i]->rect, 0, 255, 0);
			break;
		case Collider::Type::LIMITE: // green
			App->renderer->DrawQuad(colliders[i]->rect, 34, 255, 234);
			break;
		case Collider::Type::BOLA: // green
			App->renderer->DrawQuad(colliders[i]->rect, 0, 255, 0);
			break;
		case Collider::Type::GROUND: // green
			App->renderer->DrawQuad(colliders[i]->rect, 0, 255, 0);
			break;


		}
	}
	if (music == true) {
		App->audio->PlayFx(corazonFx);
	}
	// Add new ball if user presses 1
	if (App->input->GetKey(SDL_SCANCODE_1) == (KEY_UP))
	{
		AddBall(1.0f, 5.0f, 10.0f, 15.0f, -15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	p2List_item<Ground*>* gr = anclaje.getFirst();
	// Process all balls
	p2List_item<Ball*>* c = pelotas.getFirst();
	while (c != NULL)
	{
		// Process this ball only if physics enabled
		if (c->data->physics_enabled == true)
		{


			if (App->input->GetKey(SDL_SCANCODE_3) == (KEY_UP))
			{
				euler = false;
				verlet = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_4) == (KEY_UP))
			{
				verlet = false;
				euler = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_F1) == (KEY_UP))
			{
				gforce = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_F2) == (KEY_UP))
			{
				gforce = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_F3) == (KEY_UP))
			{
				dforce = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_F4) == (KEY_UP))
			{
				dforce = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_F5) == (KEY_UP))
			{
				iforce = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_F6) == (KEY_UP))
			{
				iforce = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_F7) == (KEY_UP))
			{
				eforce = true;
			}
			if (App->input->GetKey(SDL_SCANCODE_F8) == (KEY_UP))
			{
				eforce = false;
			}


			// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
			c->data->fx = c->data->fy = 0.0;
			c->data->ax = c->data->ay = 0.0;

			// Step #1: Compute forces

			// Compute Gravity force
			if (gforce == true)
			{
				c->data->fgx = c->data->mass * 0.0;
				c->data->fgy = c->data->mass * g; // Let's assume gravity is constant and downwards
			}
			if (gforce == false)
			{
				c->data->fgx = 0.0;
				c->data->fgy = 0.0;
			}
			// Add gravity force to the total accumulated force of the ball
			c->data->fx += c->data->fgx;
			c->data->fy += c->data->fgy;
			//c->data->fy += c->data->fgy - c->data->fiy - c->data->fdy;

			// Compute Aerodynamic Lift & Drag forces

			if (dforce == true)
			{
				drag_function(c->data, dt);
			}
			if (dforce == false)
			{
				c->data->fdx = 0.0;
				c->data->fdy = 0.0;
			}

			// Add impulsive force

			if (eforce == true)
			{
				//elastic_function(c->data, dt, anchor, float b);
			}
			// Add impulsive force
			if (iforce == true)
			{
				if (App->input->GetKey(SDL_SCANCODE_1) == (KEY_UP))
				{
					impulsive_function(c->data, dt, rot);
				}
			}


			// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
			c->data->ax = c->data->fx / c->data->mass;
			c->data->ay = c->data->fy / c->data->mass;

			// Step #3: Integrate --> from accel to new velocity & new position. 
			// We will use the 2nd order "Velocity Verlet" method for integration.

			if (verlet == true)
			{
				integrator_velocity_verlet(c->data, dt);
			}
			if (euler == true)
			{
				integrator_velocity_euler(c->data, dt);
			}


			// Step #4: solve collisions

			// Bounce with the ground
			if ((c->data->y + 20) > grounde.y)
			{
				// Solve clipping: teleport ball to ground surface
				// TODO: you can do something else instead of a simple TP
				c->data->y = grounde.y - 20;

				// Ellastic collision with the ground
				c->data->vy = -c->data->vy; // Reverse vertical velocity

				// Apply friction & restitution
				c->data->vx *= 0.9; // Friction
				c->data->vy *= 0.8; // Restitution
			}
		}

		// Next ball
		c = c->next;

	}
	//elastic force (springs)
	while (gr != NULL) {
		elastic_function(c->data, dt, gr->data, b);
		gr = gr->next;
	}

	music = false;

	// Continue playing
	return UPDATE_CONTINUE;
}

update_status Motor::PostUpdate()
{
	//SHOW COLLIDERS
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		if (debug != true) {
			debug = false;
		}
		else {
			debug = true;
		}
	}
	// Draw all balls in the list
	p2List_item<Ball*>* c = pelotas.getFirst();
	while (c != NULL)
	{
		App->renderer->DrawCircle(c->data->x + 50, c->data->y+450, 20, 0, 255, 255);
		music = true;
		c = c->next;
	}

	// Draw ground
	SDL_Rect a = { grounde.x, grounde.y+450, 1200, 10 };
	App->renderer->DrawQuad(a, 0, 255, 255);
	
	return UPDATE_CONTINUE;
}

//=========================== FUNCIONES DE FISICA =================================================================================

// Integration scheme: Velocity Verlet
bool  Motor::integrator_velocity_verlet(Ball* ball, float dt)
{
	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	ball->vx += ball->ax * dt;

	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->vy += ball->ay * dt;


	return true;
}

bool  Motor::integrator_velocity_euler(Ball* ball, float dt)
{
	ball->x += ball->x + ball->vx * dt;
	ball->vx += ball->vx + ball->ax * dt;

	ball->y += ball->y + ball->vy * dt;
	ball->vy += ball->vy + ball->ay * dt;

	return true;
}

bool  Motor::drag_function(Ball* ball, float dt)
{
	float cd = 0.12;

	ball->fdx = 0.5 * (0.3 * 0.3) * cd;
	ball->fdy = 0.5 * (0.3 * 0.3) * cd;

	return true;
}


bool Motor::impulsive_function(Ball* ball, float dt, int rot)
{
	rot = App->player->rotacion;
	
	ball->fy -= 20000;
	ball->fx += 20000 * cos(rot);

	return true;
}

bool Motor::elastic_function(Ball* ball, float dt, Ground* anchor, float b)
{
	float k = 5.0;
	float Fk;
	float Dx;
	float Dvx;

	Dx = ball->x -= anchor->x;

	Dvx = (ball->vx += ball->vy) + (ball->vx += ball->vy) * (anchor->ground - (anchor->x += anchor->y));

	Fk = -k * Dx - (b * Dvx);

	//Fk = -k * DeltaX - b * deltaV
	return true;
}
//==============================================================================================================================

void Motor::DebugDraw()
{
	

}

Collider* Motor::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

// Unload assets
bool Motor::CleanUp()
{
	LOG("Unloading player");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}