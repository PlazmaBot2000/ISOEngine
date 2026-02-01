#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Engine/Engine_Modules.h"
#include <iostream>
#include <vector>

const int MOVEMENT_MULT = 10;
const bool LOG = false;

GameObject player, box;
Vector2D Movement;

int WindowWidth, WindowHeight = 0;
std::vector<GameObject *> Scene;

CinematicCamera camera;


void Draw(SDL_Window *window, SDL_Renderer *renderer){
	player.texture.draw(renderer, camera.x, camera.y);
	box.texture.draw(renderer, camera.x, camera.y);
}


int start(SDL_Window *window, SDL_Renderer *renderer){	
	SDL_SetRenderDrawColor(renderer, 34, 32, 52, 255);
	SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

	player.texture.loadTexture(renderer, "Assets/Textures/player.png");
	box.texture.loadTexture(renderer, "Assets/Textures/box.png");

	if(!player.texture.texture or !box.texture.texture){
		std::cerr << "Ошибка загрузки текстур: " << IMG_GetError() << std::endl;
		return 1;
	}
	 
    player.x = 100; player.y = 100;
    player.width = player.texture.width;
    player.height = player.texture.height;
    player.physics.type = PhysicsType::Kinematic;
    player.addCollider(static_cast<float>(player.width), static_cast<float>(player.height), static_cast<float>(player.angle));
	camera.Target = &player;
	camera.x = player.x + (player.width - WindowWidth) / 2.0f;
	camera.y = player.y + (player.height - WindowHeight) / 2.0f;

    box.x = 400; box.y = 400;
    box.width = box.texture.width * 5.0f;
    box.height = box.texture.height * 2.0f;
    box.texture.scaleMode = ScaleMode::Repeat;
    box.physics.type = PhysicsType::Static;
    box.addCollider(static_cast<float>(box.width), static_cast<float>(box.height), static_cast<float>(box.angle));

    Scene.push_back(&box);
    return 0;
}


int loop(SDL_Window *window, SDL_Renderer *renderer){
    Movement = Engine_GetAxis::All();
    
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) return 1;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return 1;
            }
        }
    }

    Movement = Engine_GetAxis::All();
    
    double mag = Movement.magnitude();
    if (mag > 0.001) {
        player.physics.velocity.x = (Movement.x / mag) * MOVEMENT_MULT;
        player.physics.velocity.y = (Movement.y / mag) * MOVEMENT_MULT;
    } else {
        player.physics.velocity = {0, 0};
    }
    
    box.update({});
	player.update(Scene);
	camera.update(window, Movement);
	
	if(LOG){
		std::cout << "Movement input: \n" << Movement.x << " "<< Movement.y << std::endl
			<< "Time: \n" << SDL_GetTicks() << std::endl;
	}

    Draw(window, renderer);
    return 0;
}
