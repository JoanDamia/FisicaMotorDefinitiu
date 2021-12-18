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


Ball* Motor::AddBall(double mass, float x, float y, double vx, double vy, double fy, double fx, double ax, double ay, double fgx, double fgy) {
	Ball* thisball = new Ball(mass, x, y, vx, vy, fy, fx, ax, ay, fgx, fgy);

	pelotas.add(thisball);
	
	return thisball;
}
// Update: draw background
update_status Motor::Update()
{
	
	
if (App->input->GetKey(SDL_SCANCODE_1) == (KEY_REPEAT)) {
	AddBall(5, 5, 5, 1, 1, 0, 0, 0, 0, 0, 0);
		enabled = true;
		
		
	
		

}

	if (pelotas.getFirst() != NULL) {
		
		p2List_item<Ball*>* c = pelotas.getFirst();
		

		while (c != NULL) {
			
			if (enabled == true) {
				App->renderer->DrawCircle(c->data->x + 50, c->data->y, 20, 0, 255, 255);

				// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
				c->data->fx = c->data->fy = 0.0;
				c->data->ax = c->data->ay = 0.0;

				// Step #1: Compute forces

				// Compute Gravity force
				c->data->fgx = c->data->mass * 0.0;
				c->data->fgy = c->data->mass * g; // Let's assume gravity is constant and downwards

				// Add gravity force to the total accumulated force of the ball
				c->data->fx += c->data->fgx;
				c->data->fy += c->data->fgy - c->data->fiy - c->data->fdy;
				
				// Compute Aerodynamic Lift & Drag forces







				//ball. speed = ball.speed(ball.vx - atmosphere.windx, ball.vy - atmosphere.windy);
				//ball. fdrag = 0.5 * atmosphere.density * speed * speed * ball.surface * ball.cd;
				//ball. flift = 0.5 * atmosphere.density * speed * speed * ball.surface * ball.cl;
				//ball. fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this)
				//ball. fdy = flift; // Let's assume Lift is perpendicular with x-axis (in your game, generalize this)



			// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
				/*ball.ax = ball.fx / ball.mass;
				ball.ay = ball.fy / ball.mass;*/

				//ball->ay = ball->vy / dt;
				c->data->ax = c->data->fx / c->data->mass;
				c->data->ay = c->data->fy / c->data->mass;

				// Step #3: Integrate --> from accel to new velocity & new position. 
				// We will use the 2nd order "Velocity Verlet" method for integration.
				// You can also move this code into a subroutine: integrator_velocity_verlet(ball, dt);

				// newton_law(dt);
				// Step #4: solve collisions
				//if (ball.y >= grounde.y)
				//{
				//	// For now, just stop the ball when it reaches the ground.
				//	ball.vx = ball.vy = 0.0;
				//	ball.ax = ball.ay = 0.0;
				//	ball.fx = ball.fy = 0.0;
				//	ball.physics_enabled = false;
				//}
				integrator_velocity_verlet(c->data, dt);
				drag_function(c->data, dt);
				impulsive_function(c->data, dt);
				
			}
			if (c != NULL) {
				c = c->next;
			}
		}
	}

	
	

	

	SDL_Rect a = { grounde.x, grounde.y, 1200, 10 };
	App->renderer->DrawQuad(a, 0, 255, 255);
	//App->renderer->DrawLine(ground.x, ground.y, 20, 0, 255, 255);
	
	
	
	
	
	return UPDATE_CONTINUE;
}
// Integration scheme: Velocity Verlet
// You should modularise all your algorithms into subroutines. Including the ones to compute forces.

update_status Motor::PostUpdate() {
	


	return UPDATE_CONTINUE;
}
bool  Motor::integrator_velocity_verlet(Ball* ball, float dt)
{
	LOG("HASTA LOS HUEVOS ");
	ball->ax += ball->fx / ball->mass;
	ball->vx += ball->ax * dt;
	ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;

	ball->ay += ball->fy / ball->mass;
	ball->vy += ball->ay * dt;
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	
	
	return true;
}


bool  Motor::drag_function(Ball* ball, float dt)
{
	float cd = 0.12;

	ball->fdx = 0.5 * (0.3 * 0.3) * cd;
	ball->fdy = 0.5 * (0.3 * 0.3) * cd;

	return true;
}

bool Motor::impulsive_function(Ball* ball, float dt) {

	ball->fy += 1530;
	ball->fx += 500;

	return true;
}

bool Motor::elastic_function(Ball* ball, float dt) {



	return true;
}

//void Motor::newton_law( float dt)
//{
//	
//
//
//}
//


// Unload assets
bool Motor::CleanUp()
{
	LOG("Unloading player");

	return true;
}