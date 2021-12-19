#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModuleRender.h"
#include"ModuleTextures.h"
#include"Collider.h"
#include"Motor.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	CanonKun = App->textures->Load("Assets/sprites/CanonKun.png");
	MunecoKun = App->textures->Load("Assets/sprites/MunecoKun.png");
	Lose= App->textures->Load("Assets/sprites/lose.png");
	Win = App->textures->Load("Assets/sprites/win.png");
	LOG("Loading player");

	player =App->motor->AddCollider({ xplayer, yplayer, 30, 30 }, Collider::Type::PLAYER, this);
	ground = App->motor->AddCollider({ 0, 480,1200, 10 }, Collider::Type::GROUND, this);
	limite = App->motor->AddCollider({ Limx, Limy, 2, 80 }, Collider::Type::LIMITE, this);
	bola = App->motor->AddCollider({ App->motor->ball.x, App->motor->ball.y, 24, 24 }, Collider::Type::BOLA, this);
	

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	player->SetPos(xplayer+30, yplayer+30 );
	ground->SetPos(0,500);
	limite->SetPos(Limx, Limy);
	bola->SetPos(App->motor->ball.x, App->motor->ball.y);
	
	if (App->motor->ball.x == xplayer  ) {
		vidas -= 1;
	}
	if (vidas <= 0) {
		dead = true;
	}
	if (dead == true) {
		App->renderer->Blit(Lose, xplayer - 500, yplayer - 400, NULL, 1.0f, 0);
	}
	if (win == true) {
		App->renderer->Blit(Win, xplayer - 500, yplayer - 400, NULL, 1.0f, 0);
	}
	if (dead == false) {
		App->renderer->Blit(MunecoKun, xplayer, yplayer, NULL, 1.0f, 0);

	}

	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)&&Limite==false) {
		xplayer -= 5;
	}
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && Limite == false) {
		xplayer += 5;
	}

	if ((App->input->GetKey(SDL_SCANCODE_UP)) == (KEY_REPEAT)) {
		rotacion += 1;
	}

	if ((App->input->GetKey(SDL_SCANCODE_DOWN)) == (KEY_REPEAT)) {
		rotacion -= 1;
	}
	if (rotacion > 90)
	{
		rotacion = 90;
	}
	if (rotacion < 0)
	{
		rotacion = 0;
	}
	SDL_Rect a = { x, y, 50, 50 };
	App->renderer->Blit(CanonKun,x, y,NULL,1.0f, -rotacion);
	Limite = false;
	//INSTA WIN
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP) {
		win = true;
		
	}
	//INSTA LOSE

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_UP) {
		dead = true;
		
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{

	if (c1->type == Collider::Type::PLAYER && c2->type == Collider::Type::BOLA) {
		dead = true;
		
		
	}

	if (c1->type == Collider::Type::BOLA && c2->type == Collider::Type::GROUND) {
		
	}


	if (c1->type == Collider::Type::PLAYER && c2->type == Collider::Type::LIMITE) {
		Limite = true;
		xplayer -= 1;
	}


	

	
}

