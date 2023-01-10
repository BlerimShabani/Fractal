#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

int screen_width, screen_height;
char * fractal_name;
SDL_Renderer * renderer;

void mandelbrot() {
	double minCRe, maxCRe, minCIm, maxCIm, c_re, c_im, z1, z2;
	int a, b, n, r, g, bc;
	short max_iterations;
	//Define the complex number (values can change)
	minCRe = -2.0;
	maxCRe = 1.2;
	minCIm = -1.2;
	maxCIm = minCIm + (maxCRe - minCRe) * screen_height/screen_width;
	//Max iterations for the display (value can change)
	max_iterations = 30;
	for(a = 0; a < screen_width; a++) {
		for(b = 0; b < screen_height; b++) {
			n = 0;
			z1 = z2 = 0.;
			c_re = minCRe + a * (maxCRe - minCRe)/(screen_width - 1); //Real part of the complex number
			c_im = maxCIm - b * (maxCIm - minCIm)/(screen_height - 1); //Imaginary part of the complex number
			while(n < max_iterations && z1*z1 + z2*z2 <= 4) {
				double z1_new;
				z1_new = z1 * z1 - z2 * z2 + c_re;
				z2 = 2 * z1 * z2 + c_im;
				z1 = z1_new;
				n++;
			}
			//Color the fractal
			if(n == max_iterations) {
				r = 255;
				g = 0;
				bc = 0;
			}
			else {
				r = 0;
				g = 0;
				bc = 0;
			}
			SDL_SetRenderDrawColor(renderer, r, g, bc, 255);
			SDL_RenderDrawPoint(renderer, a, b);
		}
	}
}

int main(int argc, char ** argv) {
	SDL_Window * window = NULL;
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
		if(strcmp("Mandelbrot", fractal_name) == 0) {
			mandelbrot();
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
