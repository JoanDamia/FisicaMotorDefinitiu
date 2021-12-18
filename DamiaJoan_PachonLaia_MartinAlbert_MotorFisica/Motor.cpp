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
	p2List_item<Ball*>* c = pelotas.getFirst();
	
	while(c != NULL) {
		Ball* p = c->data;
		if (enabled == true) {
			App->renderer->DrawCircle(p->x + 50, p->y, 20, 0, 255, 255);

			// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
			p->fx = p->fy = 0.0;
			p->ax = p->ay = 0.0;

			// Step #1: Compute forces

			// Compute Gravity force
			p->fgx = p->mass * 0.0;
			p->fgy = p->mass * g; // Let's assume gravity is constant and downwards

			// Add gravity force to the total accumulated force of the ball
			p->fx += p->fgx;
			p->fy += p->fgy;
			p->fy -= p->fdy;
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
			p->ax = p->fx / p->mass;
			p->ay = p->fy / p->mass;

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
			integrator_velocity_verlet(p, dt);
			drag_function(p, dt);
			impulsive_function(p, dt);

		}
		if (c != NULL) {
			c = c->next;
		}
	}


	
	if (App->input->GetKey(SDL_SCANCODE_1) == (KEY_DOWN)) {
		
		AddBall(5, 5, 5, 1, 1, 0, 0, 0, 0, 0, 0);
		enabled = true;
		
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
	ball->vx += ball->ax * dt;
	//ball->vy += ball->ay * dt;
	ball->vy += ball->fy * dt;
	//ball->x += ball->vx * dt + 0.5 * ball->ax * dt * dt;
	//ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	ball->y += ball->vy * dt + 0.5 * ball->ay * dt * dt;
	
	
	return true;
}


bool  Motor::drag_function(Ball* ball, float dt)
{
	float cd = 0'05;
	/*ball->x = sqrt(0'5* density * (ball->vx * ball->vx) * s * cd);
	ball->fdx = 0'5* density * (ball->vx * ball->vx) * s * cd;*/

	ball->vy += sqrt((2 * ball->fdy) / cd);
	//ball->fdy = 0'5* density * (ball->vy * ball->vy) * s * cd;
	//ball->y = (sqrt(ball->fdy *2/ density * s * cd))/dt;
	return true;
}

bool Motor::impulsive_function(Ball* ball, float dt) {

	/*ball->fy += 100;
	ball->fx += 50;*/

	ball->vy += ball->fy * dt;
	ball->vx += ball->fx * dt;

	ball->y -= ball->vy * dt;
	ball->x += ball->vx * dt;

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