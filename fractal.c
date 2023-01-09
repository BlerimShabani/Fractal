#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

int screen_width, screen_height;
char * fractal_name;

int main(int argc, char ** argv) {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer;
	SDL_Event e;
	SDL_Surface * screen_surface = NULL;
	char quit;
	if(argc != 4) {
		puts("Invalid number of arguments");
		return -1;
	}
	else {
		screen_height = atoi(argv[1]);
		screen_width = atoi(argv[2]);
		fractal_name = argv[3];
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			puts("Failed to initialise SDL");
			return - 1;
		}
		window = SDL_CreateWindow( "Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if(!window) {
			puts("Failed to create window");
			return -1;
		}
		screen_surface = SDL_GetWindowSurface(window);
		if(!screen_surface) {
			puts("Failed to get window surface");
			return -1;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if(!renderer) {
			puts("Failed to render");
			return -1;
		}
		//CALL THE FUNCTION TO DISPLAY FRACTAL HERE
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
		while(!quit){ 
	    		while(SDL_PollEvent(&e)){ 
	    			if(e.type == SDL_QUIT) quit = 1;
	    		} 
    		}
	}
}
