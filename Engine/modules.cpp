#include "ISOEngine.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

bool ISOEngine_Delay(int time, int& Prev_Tick){
	int diff = SDL_GetTicks() - Prev_Tick;
	if(diff >= time){
		Prev_Tick = SDL_GetTicks();
		return true;
	}
	return false;
}

int ISOEngine_Random(int upper, int lower){
	return (rand() % (upper-lower) + 1);
}
