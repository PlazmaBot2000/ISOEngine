#include <SDL2/SDL.h>
#include "Engine_Modules.h"

int Engine_GetAxis::Y(){
	const Uint8 *keyState = SDL_GetKeyboardState(NULL);
	return std::max(-1, std::min((keyState[SDL_SCANCODE_S] - keyState[SDL_SCANCODE_W] + keyState[SDL_SCANCODE_DOWN] - keyState[SDL_SCANCODE_UP]), 1));
}

int Engine_GetAxis::X(){
	const Uint8 *keyState = SDL_GetKeyboardState(NULL);
	return std::max(-1, std::min((keyState[SDL_SCANCODE_D] - keyState[SDL_SCANCODE_A] + keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]), 1));
}

//Delay для использования в Update
bool Engine_Delay(int time, int& Prev_Tick){
	int diff = SDL_GetTicks() - Prev_Tick;
	if(diff >= time){
		Prev_Tick = SDL_GetTicks();
		return true;
	}
	return false;
}
//Псевдослучайное число в интервале
int Engine_Random(int upper, int lower){
	return (rand() % (upper-lower) + 1);
}

