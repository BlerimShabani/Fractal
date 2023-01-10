#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

SDL_Window * window = NULL;
SDL_Renderer * renderer;
int screen_width, screen_height;
char * fractal_name;

void mandelbrot() {
	double minCRe, maxCRe, minCIm, maxCIm, c_re, c_im, z1, z2;
	int a, b, n;
	short max_iterations;
	z1 = z2 = 0.;
	n = 0;
	//Define the complex number (values can change)
	minCRe = -2.0;
	maxCRe = 1.2;
	minCIm = -1.2;
	maxCIm = minCIm + (maxCRe - minCRe) * screen_height/screen_width;
	//Max iterations for the display (value can change)
	max_iterations = 200;
	for(a = 0; a < screen_width; a++) {
		for(b = 0; b < screen_height; b++) {
			c_re = minCRe + a * (maxCRe - minCRe)/(screen_width - 1); //Real part of the complex number
			c_im = maxCIm - b * (maxCIm - minCIm)/(screen_height - 1); //Imaginary part of the complex number
			while(n < max_iterations && z1*z1 + z2*z2 <= 4) {
				double z1_new;
				z1_new = z1 * z1 + z2 * z2 + c_re;
				z2 = 2 * z1 * z2 + c_im;
				z1 = z1_new;
				n++;
			}
			//TODO : Coloring the fractal
		}
	}
}

void julia()
{

    /* Real and imaginary with const c to determine shape of Julia new and old,
     change position and zoom, color for pixel */

    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 1, moveX = 0, moveY = 0;
    ColorRGB color;

    //Maximum iteration that function should stop
    int maxIterations = 300;

    //some values for the constant c, this determines the shape of the Julia Set
    cRe = -0.7;
    cIm = 0.27015;

    //loop through every pixel
    for(int y = 0; y < screen_heigh; y++)
        for(int x = 0; x < screen_width; x++)
        {
            //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
            newRe = 1.5 * (x - screen_width / 2) / (0.5 * zoom * screen_width) + moveX;
            newIm = (y - screen_heigh / 2) / (0.5 * zoom * screen_high) + moveY;

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
            //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
            color = HSVtoRGB(ColorHSV(i % 256, 255, 255 * (i < maxIterations)));
            //draw the pixel
            //pset(x, y, color);
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
		if(strcmp("Mandelbrot", fractal_name) == 0) {
			mandelbrot();
		}

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
