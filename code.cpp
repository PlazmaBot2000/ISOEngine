#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Engine/Engine_Modules.h"
#include <iostream>
#include <vector>

const int MOVEMENT_MULT = 10;
const bool LOG = false;

GameObject player, box;
Vector2D Movement;

int WindowWidth, WindowHeight;
std::vector<GameObject *> Scene;


void Draw(SDL_Window *window, SDL_Renderer *renderer){
	player.draw(renderer);
	box.draw(renderer);
}


int start(SDL_Window *window, SDL_Renderer *renderer){	
	SDL_SetRenderDrawColor(renderer, 34, 32, 52, 255);
	SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);
	player.loadTexture(renderer, "Assets/player.png");
	box.loadTexture(renderer, "Assets/box.png");
	
	if(!player.texture or !box.texture){
		std::cerr << "Ошибка загрузки текстур: " << IMG_GetError() << std::endl;
		return 1;
	}
	
	player.Physics.physicsType = PhysicsType::Kinematic;
	box.collider.x = box.collider.y = 500;
	box.collider.width *= 5;
	box.collider.height *= 2;
	box.scaleMode = ScaleMode::Repeat;

	Scene.push_back(&box);
	return 0;
}


int loop(SDL_Window *window, SDL_Renderer *renderer){
    Movement = Engine_GetAxis::All();
	box.collider.angleDegrees += 1;
	player.collider.angleDegrees += 1;
	if(LOG){
		std::cout << "Movement input: \n" << Movement.x << " "<< Movement.y << std::endl
			<< "Time: \n" << SDL_GetTicks() << std::endl;
	}
    
    double length = Movement.magnitude();
	if (player.Physics.physicsType == PhysicsType::Kinematic) {
	    if (length > 0) {
			player.Physics.velocity = {(Movement.x / length) * MOVEMENT_MULT, (Movement.y / length) * MOVEMENT_MULT};
    	} else {
			player.Physics.velocity = {0, 0};    
		}
	}


    player.updatePosition(Scene);
    Draw(window, renderer);

    return 0;
}
