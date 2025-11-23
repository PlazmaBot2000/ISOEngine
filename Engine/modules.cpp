#include "ISOEngine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

int ISOEngine_GetWindowWidth(SDL_Window *window){
	int w;
	SDL_GetWindowSize(window, &w, nullptr);
	return w;
}

int ISOEngine_GetWindowHeight(SDL_Window *window){
	int h;
	SDL_GetWindowSize(window, nullptr, &h);
	return h;
}

SDL_Texture* LoadImage(SDL_Window *window, SDL_Renderer *renderer, std::string file){
   SDL_Surface *loadedImage = nullptr;
   SDL_Texture *texture = nullptr;
    loadedImage = SDL_LoadBMP(file.c_str());
   if (loadedImage != nullptr){
       texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
       SDL_FreeSurface(loadedImage);
   }
   else
       std::cout << SDL_GetError() << std::endl;
   return texture;
}

void RenderScalableTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void RenderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y){
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	RenderScalableTexture(texture, renderer, x, y, w, h);
}
