#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#ifndef MODULES_H
#define MODULES_H

class ISOEngine_GetAxis{
	public:
		static int x();
		static int y();
};

bool ISOEngine_Delay(int time, int& Prev_Tick);
int ISOEngine_Random(int upper, int lower);
int ISOEngine_GetWindowWidth(SDL_Window *window);
int ISOEngine_GetWindowHeight(SDL_Window *window);
int ISOEngine_GetTextureWidth(SDL_Texture *texture);
int ISOEngine_GetTextureHeight(SDL_Texture *texture);

SDL_Texture* LoadImage(SDL_Window *window, SDL_Renderer *renderer, const char* file_path);
void RenderScalableTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h);
void RenderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);

#endif
