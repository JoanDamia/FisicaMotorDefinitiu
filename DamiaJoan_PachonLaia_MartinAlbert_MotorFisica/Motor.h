#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "Collider.h"

#define MAX_COLLIDERS 1000

class Ground {
public:
	double ground;
	int x=0;
	int y=50;
};
class Ball
{

public:
	
	// Position
	// You could also use an array/vector
	int x = 0;
	int y = 0;

	// Velocity
	double vx=0;
	double vy=0;

	// Acceleration
	double ax;
	double ay;

	// Force (total) applied to the ball
	float fx;
	float fy;

	// Mass
	double mass= 100;

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
	double fiy;
	double fix;
	

	// Has physics enabled?
	bool physics_enabled = true;

	Ball() {
		this->mass = 0;
		this->x = 0;
		this->y = 0;
		this->vx = 0;
		this->vy = 0;
		this->fy = 0;
		this->fx = 0;
		this->ax = 0;
		this->ay = 0;
		this->fgx = 0;
		this->fgy = 0;
		this->fdx = 0;
		this->fdy = 0;
		this->fiy = 0;
		this->fix = 0;
		
	}

	Ball(double mass, float x, float y, double vx, double vy, double fy, double fx, double ax, double ay, double fgx, double fgy) {
		this->mass = mass;
		this->x = x;
		this->y = y;
		this->vx = vx;
		this->vy = vy;
		this->fy = fy;
		this->fx = fx;
		this->ax = ax;
		this->ay = ay;
		this->fgx = fgx;
		this->fgy = fgy;
		this->fdx = fdx;
		this->fdy = fdy;
		this->fiy = fiy;
		this->fix = fix;
		
	}
	 ~Ball() {

	}
};

class Motor : public Module
{
public:
	p2List<Ball*>pelotas;
	Ball ball;
	Ground grounde;
	Motor(Application* app, bool start_enabled = true);
	virtual ~Motor();

	bool Start();
	update_status Update();
	update_status PreUpdate();
	update_status PostUpdate();
	Ball* AddBall(double mass, float x, float y, double vx, double vy, double fy, double fx, double ax, double ay, double fgx, double fgy);
	bool CleanUp();
	bool integrator_velocity_verlet(Ball* ball, float dt);
	bool integrator_velocity_euler(Ball* ball, float dt);
	bool drag_function(Ball* ball, float dt);
	bool impulsive_function(Ball* ball, float dt, int rot);
	bool elastic_function(Ball* ball, float dt, Ground* anchor, float b);
	//void newton_law(Ball* ball, float dt);
	void ComputeForces(Ball* ball, float dt);
public:
	int rot = 0;
	float dt =0.016;
	float g = 200.0f;
	bool enabled = true;
	bool verlet = true;
	bool euler = false;
	bool gforce = true;
	bool dforce = true;
	bool iforce = true;
	bool eforce = true;
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	void DebugDraw();
	uint corazonFx = 0;
	bool music = false;
	bool debug = false;
};
