#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"


class Ground {
public:
	double ground;
	double x;
	double y;
};
class Ball
{
public:
	// Position
	// You could also use an array/vector
	float x;
	float y;

	// Velocity
	double vx;
	double vy;

	// Acceleration
	double ax;
	double ay;

	// Force (total) applied to the ball
	double fx;
	double fy;

	// Mass
	double mass;

	// Aerodynamics stuff
	double surface; // Effective wet surface
	double cl; // Lift coefficient
	double cd; // Drag coefficient


	double fgx;
	double fgy;
	double speed;
	double fdrag;
	double flift;
	double fdx; // Let's assume Drag is aligned with x-axis (in your game, generalize this)
	double fdy; // Let's assume Lift is perpendicular with x-axis (in your game, generalize this)
	
	// Has physics enabled?
	bool physics_enabled = true;

	

	
};

class Motor : public Module
{
public:

	Ball ball;
	Ground grounde;
	Motor(Application* app, bool start_enabled = true);
	virtual ~Motor();

	bool Start();
	update_status Update();
	bool CleanUp();
	bool integrator_velocity_verlet(Ball* ball, double dt);
public:
	float dt =1/60;
	

	
};