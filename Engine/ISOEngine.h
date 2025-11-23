#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#ifndef MODULES_H
#define MODULES_H

bool ISOEngine_Delay(int time, int& Prev_Tick);
int ISOEngine_Random(int upper, int lower);
int ISOEngine_GetWindowWidth(SDL_Window *window);
int ISOEngine_GetWindowHeight(SDL_Window *window);
SDL_Texture* LoadImage(SDL_Window *window, SDL_Renderer *renderer, std::string file);
void RenderScalableTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h);
void RenderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y);

#endif
