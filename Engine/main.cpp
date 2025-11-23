#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include "ISOEngine.h"
#include "Scripts.h"

const int TARGET_FPS = 60;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;
const int WINDOW_WIDTH = 1900;
const int WINDOW_HEIGHT = 1190;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
	
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_SetWindowTitle(window, "ISOEngine");
	SDL_SetRenderDrawColor(renderer, 34, 32, 52, 255);

	bool game_is_running = true;
	start(window, renderer);
    while (game_is_running) { // main game loop
		Uint32 frameStart = SDL_GetTicks();

		SDL_Event event; // handle window closing
		while (SDL_PollEvent(&event) != 0) {
			if (SDL_QUIT==event.type || (SDL_KEYDOWN==event.type && SDLK_ESCAPE==event.key.keysym.sym))
				game_is_running = false; // quit
		}
		SDL_RenderClear(renderer); // re-draw the window

		loop(window, renderer);
        SDL_RenderPresent(renderer);

		int frameTime = SDL_GetTicks() - frameStart;
		if (FRAME_DELAY_MS > frameTime) {
            SDL_Delay(FRAME_DELAY_MS - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	IMG_Quit();
    SDL_Quit();
    return 0;
}
