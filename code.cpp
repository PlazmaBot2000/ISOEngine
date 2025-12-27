#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Engine/Engine_Modules.h"
#include <iostream>
#include <vector>

const int MOVEMENT_MULT = 10;

GameObject player, box;

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
	
	player.physicsType = PhysicsType::Static;
	box.physicsType = PhysicsType::Static;
	box.collider.x = box.collider.y = 500;
	box.collider.width *= 2;
	box.collider.height *= 2;
	box.scaleMode = ScaleMode::Repeat;

	Scene.push_back(&box);
	return 0;
}


int loop(SDL_Window *window, SDL_Renderer *renderer){
    Vector2D Movement = { (double)Engine_GetAxis::X(), (double)Engine_GetAxis::Y() };
    
    double length = Movement.magnitude();
	if (player.physicsType == PhysicsType::Dynamic) {
		if (length > 0) {
        	player.applyForce({(Movement.x / length), (Movement.y / length)});
		}
	} else if (player.physicsType == PhysicsType::Kinematic) {
	    if (length > 0) {
			player.velocity = {(Movement.x / length) * MOVEMENT_MULT, (Movement.y / length) * MOVEMENT_MULT};
    	} else {
			player.velocity = {0, 0};    
		}
	}


    player.updatePosition(Scene);
    Draw(window, renderer);

    return 0;
}
