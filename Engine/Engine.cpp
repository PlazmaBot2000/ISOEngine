#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <toml++/toml.hpp>
#include "Scripts.h"

toml::table Engine_Config;

int main(int argc, char* argv[]) {
    
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
        setvbuf(stdout, nullptr, _IOFBF, 1000); 
		SetProcessDPIAware();
    #endif

    std::setlocale(LC_ALL, ".UTF8");


	try {
        Engine_Config = toml::parse_file("Assets/Config/Engine_Config.toml");
    } catch (const toml::parse_error& err) {
        std::cerr << "Error parsing file: " << err.description() << std::endl;
        std::cerr << "  Occurred at: " << err.source() << std::endl;
        return 1;
    }

	int TARGET_FPS = Engine_Config["TARGET_FPS"].value_or(60);
	int FRAME_DELAY_MS = 1000 / TARGET_FPS;
	int WINDOW_WIDTH = Engine_Config["WINDOW_WIDTH"].value_or(1920);
	int WINDOW_HEIGHT = Engine_Config["WINDOW_HEIGHT"].value_or(1080);


    if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
	
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
        std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

	SDL_RenderSetVSync(renderer, Engine_Config["VSYNC"].value_or(0));
    
    if(std::string(Engine_Config["FULLSCREEN"].value_or("WINDOWED")) == "BORDERLESS") {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }else if(std::string(Engine_Config["FULLSCREEN"].value_or("WINDOWED")) == "FULLSCREEN") {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

	SDL_SetWindowTitle(window, "Engine");
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	bool game_is_running = true;
	if(start(window, renderer) != 0) game_is_running = false;

    while (game_is_running) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_RenderClear(renderer);

        if (loop(window, renderer) != 0) {
            game_is_running = false;
        }

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
