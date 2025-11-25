#include "Engine/ISOEngine.h"
#include "Engine/Scripts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

SDL_Texture* block;
int x, y, dx, dy;
float speed = 2;

void NextPos(SDL_Window *window){
	if((x + ISOEngine_GetTextureWidth(block) >= 1920) or (x <= 0)){
		dx = -dx;
	}
	if((y + ISOEngine_GetTextureHeight(block) >= 1200) or (y <= 0)){
		dy = -dy;
	}
	x += dx * speed;
	y += dy * speed; 
}

void start(SDL_Window *window, SDL_Renderer *renderer){
	block = LoadImage(window, renderer, "block.png");
	x = ISOEngine_Random(0, ISOEngine_GetWindowWidth(window) - ISOEngine_GetTextureWidth(block));
    y = ISOEngine_Random(0, ISOEngine_GetWindowHeight(window) - ISOEngine_GetTextureHeight(block));
	dx = 1;
	dy = 1;
}

void loop(SDL_Window *window, SDL_Renderer *renderer){
	RenderTexture(block, renderer, x, y);
	NextPos(window);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
