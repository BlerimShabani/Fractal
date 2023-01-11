CC=gcc

NAME=fractal

FLAGS=-Wall

SDL=`sdl2-config --cflags --libs`

INSTALL=apt-get install libsdl2-dev

all:
	sudo $(INSTALL)
	$(CC) $(FLAGS) $(NAME).c -o $(NAME) $(SDL)

clean:
	rm -rf *.o
