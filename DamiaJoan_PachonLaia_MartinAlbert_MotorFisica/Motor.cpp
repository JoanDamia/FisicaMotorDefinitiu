#include "Globals.h"
#include "Application.h"
#include "Motor.h"


Motor::Motor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Motor::~Motor()
{}

// Load assets
bool Motor::Start()
{
	LOG("Loading player");
	Ball ball;

	// Set physics properties of the ball
	ball.mass = 10; // kg
	ball.surface = 2; // m^2
	ball.cd = 0.4;
	ball.cl = 1.2;

	// Set initial velocity of the ball

	ball.vx = 5.0;
	ball.vy = 5.0;
	Ground grounde;

	grounde.y = 9;
	grounde.x = 9;
	return true;
}

// Unload assets
bool Motor::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status Motor ::Update()
{
	
	// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
	ball.fx = ball.fy = 0.0;
	ball.ax = ball.ay = 0.0;

	// Step #1: Compute forces

		// Compute Gravity force
	ball.fgx = ball.mass * 0.0;
	ball.fgy = ball.mass * 10.0; // Let's assume gravity is constant and downwards

	// Add gravity force to the total accumulated force of the ball
	ball.fx += ball.fgx;
	ball.fy += ball.fgy;

	// Compute Aerodynamic Lift & Drag forces

	//ball. speed = ball.speed(ball.vx - atmosphere.windx, ball.vy - atmosphere.windy);
	//ball. fdrag = 0.5 * atmosphere.density * speed * speed * ball.surface * ball.cd;
	//ball. flift = 0.5 * atmosphere.density * speed * speed * ball.surface * ball.cl;
	//ball. fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this)
	//ball. fdy = flift; // Let's assume Lift is perpendicular with x-axis (in your game, generalize this)

	// Add gravity force to the total accumulated force of the ball
	//ball.fx += ball.fdx;
	//ball.fy += ball.fdy;

	// Other forces
	// ...

// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
	/*ball.ax = ball.fx / ball.mass;
	ball.ay = ball.fy / ball.mass;*/


	
	// Step #3: Integrate --> from accel to new velocity & new position. 
	// We will use the 2nd order "Velocity Verlet" method for integration.
	// You can also move this code into a subroutine: integrator_velocity_verlet(ball, dt);
	 integrator_velocity_verlet(&ball, dt);
	 newton_law(&ball, dt);
	// Step #4: solve collisions
	if (ball.y > grounde.y)
	{
		// For now, just stop the ball when it reaches the ground.
		ball.vx = ball.vy = 0.0;
		ball.ax = ball.ay = 0.0;
		ball.fx = ball.fy = 0.0;
		ball.physics_enabled = false;
	}
	SDL_Rect a = { grounde.x, grounde.y, 1200, 10 };
	App->renderer->DrawQuad(a,20,0,255,255);
	App->renderer->DrawCircle(ball.x, ball.y, 20, 0, 255, 255);
	//App->renderer->DrawLine(ground.x, ground.y, 20, 0, 255, 255);

	return UPDATE_CONTINUE;
}
// Integration scheme: Velocity Verlet
// You should modularise all your algorithms into subroutines. Including the ones to compute forces.
bool  Motor::integrator_velocity_verlet(Ball* ball, double dt)
{
	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->vx += ball->ax * dt;
	ball->vy += ball->ay * dt;
	return true;
}

void Motor::newton_law(Ball* ball, float dt)
{
	ball->ay = ball->vy / dt;
	//ball->ax = ball->fx / ball->mass;
	ball->fy = ball->ay * ball->mass;
	LOG("VX= %d, VY= %d ", ball->vx, ball->vy);


}



//void Motor::ComputeForces(Ball* ball, float dt)
//{
//
//	// Compute Gravity force
//	ball->fgx = ball->mass * 0.0;
//	ball->fgy = ball->mass * g; // Let's assume gravity is constant and downwards
//
//   // Add gravity force to the total accumulated force of the ball
//	ball->fx += ball->fgx;
//	ball->fy += ball->fgy;
//
//	LOG("VX= %d, VY= %d ", ball->vx, ball->vy);
//}