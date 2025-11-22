#include "Engine/ISOEngine.h"
#include "Engine/Scripts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

int Last_Use = 0;
SDL_Texture* block; 
int x, y, dx, dy;
void start(SDL_Window *window, SDL_Renderer *renderer){
	block = LoadImage(window, renderer, "block.bmp");
	x = ISOEngine_Random(0, 800);
	dx = 1;
    y = ISOEngine_Random(0, 800);
	dy = 1;
}

void loop(SDL_Window *window, SDL_Renderer *renderer){
	RenderTexture(block, renderer, x, y);
	if(ISOEngine_Delay(0, Last_Use)){
		if((x == 900) or (x == 0))
			dx = -dx;
		if((y == 900) or (y == 0))
			dy = -dy;
		x += dx;
		y += dy;
	}
}
