#include "Engine/ISOEngine.h"
#include "Engine/Scripts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

SDL_Texture* block;
int x, y, dx, dy;
float speed = 2;

void NextPos(SDL_Window *window){
	dx = ISOEngine_GetAxis::x();
	dy = ISOEngine_GetAxis::y();
	x += dx * speed;
	y += dy * speed;
}

void start(SDL_Window *window, SDL_Renderer *renderer){
	block = LoadImage(window, renderer, "block.bmp");
	RenderTexture(block, renderer, x, y);
	x = ISOEngine_Random(0, 800);
    y = ISOEngine_Random(0, 800);
	dx = 0;
	dy = 0;
}

void loop(SDL_Window *window, SDL_Renderer *renderer){
	RenderTexture(block, renderer, x, y);
	NextPos(window);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
