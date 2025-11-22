#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include "ISOEngine.h"
#include "Scripts.h"

const int TARGET_FPS = 60;
const int FRAME_DELAY_MS = 1000 / TARGET_FPS;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
	
    if (SDL_CreateWindowAndRenderer(1000, 1000, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
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
		SDL_SetRenderDrawColor(renderer, 34, 32, 52, 255);
        SDL_RenderPresent(renderer);

		int frameTime = SDL_GetTicks() - frameStart;
		if (FRAME_DELAY_MS > frameTime) {
            SDL_Delay(FRAME_DELAY_MS - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
