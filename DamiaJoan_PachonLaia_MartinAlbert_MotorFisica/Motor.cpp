#include "Globals.h"
#include "Application.h"
#include "Motor.h"
#include "math.h"
#include"ModuleInput.h"

Motor::Motor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

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

// Update: compute physics
update_status Motor::Update()
{
	// Add new ball if user presses 1
	if (App->input->GetKey(SDL_SCANCODE_1) == (KEY_UP))
	{
		AddBall(1.0f, 5.0f, 10.0f, 15.0f, -15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	// Process all balls
	p2List_item<Ball*>* c = pelotas.getFirst();
	while (c != NULL)
	{
		// Process this ball only if physics enabled
		if (c->data->physics_enabled == true)
		{
			// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
			c->data->fx = c->data->fy = 0.0;
			c->data->ax = c->data->ay = 0.0;

			// Step #1: Compute forces

			// Compute Gravity force
			c->data->fgx = c->data->mass * 0.0;
			c->data->fgy = c->data->mass * g; // Let's assume gravity is constant and downwards

			// Add gravity force to the total accumulated force of the ball
			c->data->fx += c->data->fgx;
			c->data->fy += c->data->fgy;
			//c->data->fy += c->data->fgy - c->data->fiy - c->data->fdy;

			// Compute Aerodynamic Lift & Drag forces
			drag_function(c->data, dt);

			// Add impulsive force
			//impulsive_function(c->data, dt);

			// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
			c->data->ax = c->data->fx / c->data->mass;
			c->data->ay = c->data->fy / c->data->mass;

			// Step #3: Integrate --> from accel to new velocity & new position. 
			// We will use the 2nd order "Velocity Verlet" method for integration.
			integrator_velocity_verlet(c->data, dt);

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

	// Continue playing
	return UPDATE_CONTINUE;
}

update_status Motor::PostUpdate()
{
	// Draw all balls in the list
	p2List_item<Ball*>* c = pelotas.getFirst();
	while (c != NULL)
	{
		App->renderer->DrawCircle(c->data->x + 50, c->data->y, 20, 0, 255, 255);
		c = c->next;
	}

	// Draw ground
	SDL_Rect a = { grounde.x, grounde.y, 1200, 10 };
	App->renderer->DrawQuad(a, 0, 255, 255);

	return UPDATE_CONTINUE;
}


// Integration scheme: Velocity Verlet
bool  Motor::integrator_velocity_verlet(Ball* ball, float dt)
{
	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	ball->vx += ball->ax * dt;

	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->vy += ball->ay * dt;


	return true;
}


bool  Motor::drag_function(Ball* ball, float dt)
{
	float cd = 0.12;

	ball->fdx = 0.5 * (0.3 * 0.3) * cd;
	ball->fdy = 0.5 * (0.3 * 0.3) * cd;

	return true;
}

bool Motor::impulsive_function(Ball* ball, float dt)
{
	ball->fy += 1530; // TOO HIGH!
	ball->fx += 500;

	return true;
}

bool Motor::elastic_function(Ball* ball, float dt, Ground* anchor, float b)
{
	return true;
}


// Unload assets
bool Motor::CleanUp()
{
	LOG("Unloading player");

	return true;
}