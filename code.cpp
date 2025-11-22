#include "Engine/ISOEngine.h"
#include "Engine/Scripts.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

SDL_Rect rect;
int Last_Use = 0;

void start(SDL_Window *window, SDL_Renderer *renderer){
	rect.x = ISOEngine_Random(0, 800);
    rect.y = ISOEngine_Random(0, 800);
    rect.w = 200;
	rect.h = 200;
}

void loop(SDL_Window *window, SDL_Renderer *renderer){
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);

	if(ISOEngine_Delay(0, Last_Use)){
		rect.x += 1;
		rect.y += 1;
	}
}
