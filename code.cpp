#include "Engine/ISOEngine.h"
#include "Engine/Scripts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

int Last_Use = 0;
SDL_Texture* block;
int x, y, dx, dy;
int wwidth, wheight;

void NextPos(SDL_Window *window){
	if((x >= (ISOEngine_GetWindowWidth(window) - 224)) or (x <= 0))
		dx = -dx;
	if((y >= (ISOEngine_GetWindowHeight(window) - 105)) or (y <= 0))
		dy = -dy;
	x += dx;
	y += dy;
}

void start(SDL_Window *window, SDL_Renderer *renderer){
	block = LoadImage(window, renderer, "block.bmp");
	RenderTexture(block, renderer, x, y);
	x = ISOEngine_Random(0, 800);
    y = ISOEngine_Random(0, 800);
	dx = 2;
	dy = 2;
}

void loop(SDL_Window *window, SDL_Renderer *renderer){
	RenderTexture(block, renderer, x, y);
	NextPos(window);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
