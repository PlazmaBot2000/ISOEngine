#include "ISOEngine/headers.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

SDL_Rect rect;

void start(SDL_Renderer *renderer){
	rect.x = 250;
    rect.y = 150;
    rect.w = 200;
	rect.h = 200;
}

void loop(SDL_Renderer *renderer){
	rect.x -= 1;
	rect.y -= 1;
	rect.w += 2;
	rect.h += 2;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);
	sleep(1);
}
