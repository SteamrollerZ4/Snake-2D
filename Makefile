default:
	cc -c `sdl2-config --cflags` main.c -o main.o
	cc -c snake.c -o snake.o
	cc -c walls.c -o walls.o
	cc -c `sdl2-config --cflags` gui.c -o gui.o
	cc -c ai.c -o ai.o
	cc -c `sdl2-config --cflags` `mysql_config --cflags` scoreBoard.c -o scoreBoard.o
	
link_all:main.o snake.o walls.o gui.o ai.o scoreBoard.o
	cc main.o snake.o walls.o gui.o ai.o scoreBoard.o -I/usr/include/mysql -I/usr/include/SDL2 `sdl2-config --cflags --libs` `mysql_config --libs` -lmysqlclient -lSDL2_image -lSDL2_ttf -Wall -g -o snake2d 
	
clean:main.o snake.o walls.o gui.o ai.o scoreBoard.o
	rm main.o snake.o walls.o gui.o ai.o scoreBoard.o
