#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COLOR_SIZE 4

int screen_width, screen_height;
char * fractal_name;
SDL_Renderer * renderer;
int colors[COLOR_SIZE][3] = { 
	{ 0,   7, 100 },
	{ 32, 107, 203 },
	{ 237, 255, 255 },
	{ 255, 170,   0 }
};
short counter_color = 0;
struct color_fractal {
	int r; int g; int b;
} color_fractal;

/*int interpolate(int r1, int g1, int b1, int r2, int g2, int b2, double fraction) {
	return (int) ((r2 - r1) * fraction + r1) << 16 |
                (int) ((g2 - g1) * fraction + g1) << 8 |
                (int) ((b2 - b1) * fraction + b1);
}*/

void mandelbrot(double moveX, double moveY) {
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
			c_re = minCRe + a * (maxCRe - minCRe)/(screen_width - 1) + moveX; //Real part of the complex number
			c_im = maxCIm - b * (maxCIm - minCIm)/(screen_height - 1) - moveY; //Imaginary part of the complex number
			while(n < max_iterations && z1*z1 + z2*z2 <= 4) {
				double z1_new;
				z1_new = z1 * z1 - z2 * z2 + c_re;
				z2 = 2 * z1 * z2 + c_im;
				z1 = z1_new;
				n++;
			}
			//Color the fractal
			if(n == max_iterations) {
				r = color_fractal.r;
				g = color_fractal.g;
				bc = color_fractal.b;
			}
			else {
				r = 0;
				g = 0;
				bc = 0;
			}
			int range_size, color_index, r1, g1, b1, r2, b2, g2;
			double ratio_a, ratio_b;
			range_size = max_iterations/COLOR_SIZE;
			color_index = n/range_size;
			r1 = colors[color_index][0]; g1 = colors[color_index][1]; b1 = colors[color_index][2];
			r2 = colors[color_index + 1][0]; g2 = colors[color_index + 1][1]; b2 = colors[color_index + 1][2];
			ratio_a = (double)((double)(range_size * color_index)/(double)max_iterations);
			ratio_b = (double)((double)(range_size * (color_index + 1))/(double)max_iterations);
			r = (r1 * ratio_a) + (r2 * ratio_b);
			printf("%d ", r);
			g = (g1 * ratio_a) + (g2 * ratio_b);
			bc = (b1 * ratio_a) + (b2 * ratio_b);
			SDL_SetRenderDrawColor(renderer, r, g, bc, 255);
			SDL_RenderDrawPoint(renderer, a, b);
		}
	}
}

void julia(double moveX, double moveY)
{

    /* Real and imaginary with const c to determine shape of Julia new and old,
     change position and zoom, color for pixel */

    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 1;
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
                                r = color_fractal.r;
					  g = color_fractal.g;
					  b = color_fractal.b;
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
	SDL_Window * window = NULL;
	SDL_Event e;
	SDL_Surface * screen_surface = NULL;
	SDL_DisplayMode dm;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	double moveX = 0., moveY = 0.;
	char quit, j = 0, m = 0;
	int w, h;
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
		if(SDL_GetDesktopDisplayMode(0, &dm) != 0) {
			puts("Failed to get display mode");
			return -1;
		}
		w = dm.w;
		h = dm.h;
		if(screen_width > w || screen_height > h) {
			puts("Your window is too big !");
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
		color_fractal.r = colors[0][0];
		color_fractal.g = colors[0][1];
		color_fractal.b = colors[0][2];
		if(strcmp("Julia", fractal_name) == 0) {
			julia(0., 0.);
			j = 1;
            }
            else if(strcmp("Mandelbrot", fractal_name) == 0) {
            	mandelbrot(0., 0.);
            	m = 1;
            }
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
		while(!quit){ 
	    		while(SDL_PollEvent(&e)){ 
	    			if(e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) quit = 1;
	    			if(e.type == SDL_KEYDOWN) {
		    			if(state[SDL_SCANCODE_UP]) {
		    				moveY += 1;
		    				if(j) julia(moveX, moveY);
		    				else if(m) mandelbrot(moveX, moveY);
		    				SDL_RenderPresent(renderer);
						SDL_UpdateWindowSurface(window);
		    			}
		    			else if(state[SDL_SCANCODE_DOWN]) {
		    				moveY -= 1;
		    				if(j) julia(moveX, moveY);
		    				else if(m) mandelbrot(moveX, moveY);
		    				SDL_RenderPresent(renderer);
						SDL_UpdateWindowSurface(window);
		    			}
		    			else if(state[SDL_SCANCODE_LEFT]) {
		    				moveX += 1;
		    				if(j) julia(moveX, moveY);
		    				else if(m) mandelbrot(moveX, moveY);
		    				SDL_RenderPresent(renderer);
						SDL_UpdateWindowSurface(window);
		    			}
		    			else if(state[SDL_SCANCODE_RIGHT]) {
		    				moveX -= 1;
		    				if(j) julia(moveX, moveY);
		    				else if(m) mandelbrot(moveX, moveY);
		    				SDL_RenderPresent(renderer);
						SDL_UpdateWindowSurface(window);
		    			}
		    			else if(state[SDL_SCANCODE_SPACE]) {
		    				if(j) {
		    					mandelbrot(moveX, moveY);
		    					SDL_RenderPresent(renderer);
							SDL_UpdateWindowSurface(window);
		    					m = 1;
		    					j = 0;
		    				}
		    				else if(m) {
		    					julia(moveX, moveY);
		    					SDL_RenderPresent(renderer);
							SDL_UpdateWindowSurface(window);
							j = 1;
		    					m = 0;
		    				}
		    			}
		    			else if(state[SDL_SCANCODE_C]) {
		    				if(counter_color == COLOR_SIZE) counter_color = 0;
		    				else counter_color++;
		    				color_fractal.r = colors[counter_color][0];
		    				color_fractal.g = colors[counter_color][1];
		    				color_fractal.b = colors[counter_color][2];
		    				if(j) julia(moveX, moveY);
		    				else if(m) mandelbrot(moveX, moveY);
		    				SDL_RenderPresent(renderer);
						SDL_UpdateWindowSurface(window);
		    			}
	    			}
	    		} 
    		}
	}
}
