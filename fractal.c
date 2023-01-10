#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

SDL_Window * window = NULL;
SDL_Renderer * renderer;
int screen_width, screen_height;
char * fractal_name;

void julia()
{

    /* Real and imaginary with const c to determine shape of Julia new and old,
     change position and zoom, color for pixel */

    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 1, moveX = 0, moveY = 0;
    int  r,g,b;

    //Maximum iteration that function should stop
    int maxIterations = 300;

    //some values for the constant c, this determines the shape of the Julia Set
    cRe = -0.7;
    cIm = 0.27015;

    //loop through every pixel
    for(int y = 0; y < screen_height; y++)
        for(int x = 0; x < screen_width; x++)
        {
            //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
            newRe = 1.5 * (x - screen_width / 2) / (0.5 * zoom * screen_width) + moveX;
            newIm = (y - screen_height / 2) / (0.5 * zoom * screen_height) + moveY;

            int i;
            //start the iteration process
            for(i = 0; i < maxIterations; i++)
            {
                //remember value of previous iteration
                oldRe = newRe;
                oldIm = newIm;
                //the actual iteration, the real and imaginary part are calculated
                newRe = oldRe * oldRe - oldIm * oldIm + cRe;
                newIm = 2 * oldRe * oldIm + cIm;
                //if the point is outside the circle with radius 2: stop
                if((newRe * newRe + newIm * newIm) > 4) break;
            }
	    //drw pixel in color 
                        if(i == maxIterations) {
                                r = 255;
                                g = 0;
                                b = 0;
                        }
                        else {
                                r = 0;
                                g = 0;
                                b = 0;
                        }
                        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
            
        }
}
int main(int argc, char ** argv) {
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
		if(strcmp(fractal_name, "Mandelbrot") != 0 && strcmp(fractal_name, "Julia") != 0) {
			puts("Unknown fractal");
			return -1;
		}
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

		if(strcmp("Julia", fractal_name) == 0) {
                        julia();
                }
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
		while(!quit){ 
	    		while(SDL_PollEvent(&e)){ 
	    			if(e.type == SDL_QUIT) quit = 1;
	    		} 
    		}
	}
}
