#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include"Collider.h"

class PhysBody;

struct Object
{
	SDL_Texture* graphic;
	PhysBody* body;
	uint fx;

	Object() : graphic(NULL), body(NULL)
	{}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	 ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	//coordenadas player
	int xplayer = 600;
	int yplayer = 420;

	SDL_Texture* MunecoKun;
	SDL_Texture* CanonKun;
	SDL_Texture* Lose;
	SDL_Texture* Win;
	int rotacion = 0;
	//coordenadas cañon
	int x = 0;
	int y = 450;
	void OnCollision(Collider* c1, Collider* C2);
	bool dead = false;
	bool win = false;
	int vidas = 3;
	Collider* player = nullptr;
	Collider* ground = nullptr;
	Collider* limite = nullptr;
	Collider* bola = nullptr;
	//coordenadas limite
	int Limx = 1000;
	int Limy = 450;
	bool Limite = false;
};