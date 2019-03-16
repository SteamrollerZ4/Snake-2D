default:
	cc -c `sdl2-config --cflags` ./src/main.c -o ./src/main.o
	cc -c ./src/snake.c -o ./src/snake.o
	cc -c ./src/walls.c -o ./src/walls.o
	cc -c `sdl2-config --cflags` ./src/gui.c -o ./src/gui.o
	
link_all:./src/main.o ./src/snake.o ./src/walls.o ./src/gui.o ./src/ai.o
	cc ./src/main.o ./src/snake.o ./src/walls.o ./src/gui.o ./src/ai.o -I/usr/include/SDL2 `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -Wall -g -o snake2d
	
clean:./src/main.o ./src/snake.o ./src/walls.o ./src/gui.o ./src/ai.o
	rm ./src/main.o ./src/snake.o ./src/walls.o ./src/gui.o ./src/ai.o
