#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Engine/Engine_Modules.h"
#include <unordered_map>
#include <iostream>
#include <vector>

std::unordered_map<int, Tile*> TileLib;
std::vector<std::vector<int>> map = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};
std::vector<int> rowMap = {1, 2, 1, 2, 1, 2, 1, 2};

int MOVEMENT_MULT = 5;

GameObject player;
Tile floorTile;
Vector2D Movement;

int WindowWidth, WindowHeight = 0;
std::vector<GameObject*> Scene;
CinematicCamera camera;

void Move() {
    Movement = Engine_GetAxis::All();
    double mag = Movement.magnitude();

    if (mag > 0.001) {
        player.physics.velocity.x = (Movement.x / mag) * MOVEMENT_MULT;
        player.physics.velocity.y = (Movement.y / mag) * MOVEMENT_MULT;
    } else {
        player.physics.velocity = {0, 0};
    }
}


void Draw(SDL_Window* window, SDL_Renderer* renderer) {
	ISODraw_Rectangle(300, 300, renderer, map, TileLib, camera.x, camera.y);
    player.texture.draw(renderer, camera.x, camera.y);
}

int start(SDL_Window* window, SDL_Renderer* renderer) {	
    SDL_SetRenderDrawColor(renderer, 34, 32, 52, 255);
    SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

    player.texture.loadTexture(renderer, "Assets/Textures/player.png");
    floorTile.texture.loadTexture(renderer, "Assets/Textures/tile.png");

    if (!player.texture.texture || !floorTile.texture.texture) {
        std::cerr << "Ошибка загрузки текстур: " << IMG_GetError() << std::endl;
        return 1;
    }

    floorTile.texture.width *= 4;
    floorTile.texture.height *= 4;
	floorTile.scale = 4;
    
    floorTile.offsetX = 16;
    floorTile.offsetY = 8;

    TileLib[0] = &floorTile;
	TileLib[1] = &floorTile;

    player.x = 0; 
    player.y = 0;	
    player.width = 64;
    player.height = 64;
    player.physics.type = PhysicsType::Kinematic;
    player.addCollider(player.width, player.height, player.angle);
	
    camera.Target = &player;
    camera.lookAhead = 150;

    return 0;
}

int loop(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) return 1;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) return 1;
            if (event.key.keysym.sym == SDLK_LSHIFT) MOVEMENT_MULT = 10;
        }
        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LSHIFT) MOVEMENT_MULT = 5;
        }
    }
	
    Move();
    
    player.update(Scene);
    camera.update(window, Movement);

    Draw(window, renderer);
    return 0;
}
