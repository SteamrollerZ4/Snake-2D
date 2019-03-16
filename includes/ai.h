#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "walls.h"
#include <stdio.h>

typedef struct cell {
	SDL_bool walkable;
	SDL_Rect worldPosition;
	int g_cost,h_cost;
	
	struct cell *next;
	struct cell *parent;//used for backtracking
} Cell;

void initialiseGrid(int level);
void statistics();
SDL_bool findPath(SDL_Rect start, SDL_Rect dest);
int getDistance(Cell *c1, Cell *c2);
void getNeighbours(SDL_Rect pos);
Cell cellFromPos(SDL_Rect pos);
int fCost(Cell *c);
void addOpenSet(Cell *c);
void removeOpenSet(Cell *c);
void addClosedSet(Cell *c);
int setCount(Cell *c);
SDL_bool contains(Cell *list, Cell *c);
