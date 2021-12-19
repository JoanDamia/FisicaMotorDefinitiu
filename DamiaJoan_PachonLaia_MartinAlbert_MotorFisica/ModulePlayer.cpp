#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModuleRender.h"
#include"ModuleTextures.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	CanonKun = App->textures->Load("Assets/sprites/CanonKun.png");
	LOG("Loading player");
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
	if ((App->input->GetKey(SDL_SCANCODE_UP)) == (KEY_REPEAT)) {
		rotacion += 1;
	}

	if ((App->input->GetKey(SDL_SCANCODE_DOWN)) == (KEY_REPEAT)) {
		rotacion -= 1;
	}
	SDL_Rect a = { x, y, 50, 50 };
	App->renderer->Blit(CanonKun,x, y,NULL,1.0f, -rotacion);
	

	return UPDATE_CONTINUE;
}



