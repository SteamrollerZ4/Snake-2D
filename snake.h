#ifndef SNAKE_H
#define SNAKE_H
#include "head.h"

#define STEP 14

void makeTurn(char dir, Head *tail);

//used by both head and body elements
void moveForward(char dir, Head *tail);

//head requests to turn
SDL_bool requestTurn(char dir, Head *tail);

//stop game if snake collides
SDL_bool detectCollision(SDL_Rect rectFoodPos, Head *tail);

void randomiseFood(int min, int max) ;
#endif /* SNAKE_H */
