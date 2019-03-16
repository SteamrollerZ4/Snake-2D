#include <SDL2/SDL.h>
#include <mysql/mysql.h>

SDL_bool updateBoard();
SDL_bool checkScore(int score);
int insertionSort(int score);
SDL_bool uploadScore(int position, int score, char *name);
