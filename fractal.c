

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

int screen_width, screen_height;
char * fractal_name;
SDL_Renderer * renderer;

void mandelbrot(double moveX, double moveY, double zoom) {
	double minCRe, maxCRe, minCIm, maxCIm, c_re, c_im, z1, z2;
	int a, b, n, r, g, bc;
	short max_iterations;
	//double zoom;
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
			c_re = minCRe + a * (maxCRe - minCRe)/(screen_width*zoom*0.5 - 1) + moveX; //Real part of the complex number
			c_im = maxCIm - b * (maxCIm - minCIm)/(screen_height*zoom*0.5 - 1) - moveY; //Imaginary part of the complex number
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

void julia(double moveX, double moveY, double zoom)
{

    /* Real and imaginary with const c to determine shape of Julia new and old,
     change position and zoom, color for pixel */

    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    
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
	    // pixel in color 
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

double mouseX(int a, double moveX)
{
	if(a>screen_width/2)
        moveX +=(double)(a/screen_width);
        else if(a>screen_width/2)
         moveX -=(double)(a/screen_width);
	return moveX;
} 

double mouseY(int b, double moveY)
{
        if(b>screen_height/2)
        moveY +=(double)(b/screen_height);
        else if(b>screen_height/2)
         moveY -=(double)(b/screen_height);
	return moveY;
        
} 

int main(int argc, char ** argv) {
	SDL_Window * window = NULL;
	SDL_Event e;
	SDL_Surface * screen_surface = NULL;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	double moveX = 0., moveY = 0.;
	char quit, j = 0, m = 0;
	double zoom = 1.;

	int a,b;
	a =(int)moveX;
        b=(int)moveY;
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
		if(strcmp("Julia", fractal_name) == 0) {
			julia(0., 0.,1.);
			j = 1;
            }
            else if(strcmp("Mandelbrot", fractal_name) == 0) {
            	mandelbrot(0., 0.,1.);
            	m = 1;
            }
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
		while(!quit){ 
	    		while(SDL_PollEvent(&e)){ 
	    			if(e.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) quit = 1;
	    			else if(state[SDL_SCANCODE_UP]) {
	    				moveY += 1;
	    				if(j) julia(moveX, moveY, zoom);
	    				else if(m) mandelbrot(moveX, moveY,zoom);
	    				SDL_RenderPresent(renderer);
					SDL_UpdateWindowSurface(window);
	    			}
	    			else if(state[SDL_SCANCODE_DOWN]) {
	    				moveY -= 1;
	    				if(j) julia(moveX, moveY, zoom);
	    				else if(m) mandelbrot(moveX, moveY,zoom);
	    				SDL_RenderPresent(renderer);
					SDL_UpdateWindowSurface(window);
	    			}
	    			else if(state[SDL_SCANCODE_LEFT]) {
	    				moveX += 1;
	    				if(j) julia(moveX, moveY, zoom);
	    				else if(m) mandelbrot(moveX, moveY,zoom);
	    				SDL_RenderPresent(renderer);
					SDL_UpdateWindowSurface(window);
	    			}
	    			else if(state[SDL_SCANCODE_RIGHT]) {
	    				moveX -= 1;
	    				if(j) julia(moveX, moveY, zoom);
	    				else if(m) mandelbrot(moveX, moveY,zoom);
	    				SDL_RenderPresent(renderer);
					SDL_UpdateWindowSurface(window);
	    			}
				//Mouse moving....
				if(e.type == SDL_MOUSEWHEEL)
                                {
                                        if(e.wheel.y>0){
                                        
                                        if(j){
					SDL_GetMouseState(&a,&b);
					printf("%d %d\n",a,b);
					
					moveX = mouseX(a,moveX);
					moveY = mouseY(b,moveY);
					julia(moveX, moveY,zoom++);	
					}
                                        else if(m){
					SDL_GetMouseState(&a,&b);
					printf("%d %d\n",a,b);

					moveX = mouseX(a,moveX);
                                        moveY = mouseY(b,moveY);
					mandelbrot(moveX, moveY, zoom++);
					}
                                        SDL_RenderPresent(renderer);
                                        SDL_UpdateWindowSurface(window);

                                        } else if(e.wheel.y<0){
                                         
                                        if(j){
					SDL_GetMouseState(&a,&b);
					moveX = mouseX(a,moveX);
                                        moveY = mouseY(b,moveY);
					julia(moveX, moveY, zoom--);
					}
                                        else if(m){
					SDL_GetMouseState(&a,&b);
					moveX = mouseX(a,moveX);
                                        moveY = mouseY(b,moveY);
				        mandelbrot(moveX, moveY,zoom--);
					}
                                        SDL_RenderPresent(renderer);
                                        SDL_UpdateWindowSurface(window);

                                        }else if(e.wheel.x>0) {
					
                                        if(j){
					SDL_GetMouseState(&a,&b);
					julia(moveX, moveY,zoom++);
					}
                                        else if(m){
					SDL_GetMouseState(&a,&b);
					mandelbrot(moveX, moveY,zoom--);
					}
                                        SDL_RenderPresent(renderer);
                                        SDL_UpdateWindowSurface(window);
					}
                                        else if(e.wheel.x<0) {
					
                                        if(j){
					SDL_GetMouseState(&a,&b);
					julia(moveX, moveY,zoom--);}
                                        else if(m){
					SDL_GetMouseState(&a,&b);
					mandelbrot(moveX, moveY,zoom--);}
                                        SDL_RenderPresent(renderer);
                                        SDL_UpdateWindowSurface(window);
					}
                                }
	    		} 
    		}
	}
}

