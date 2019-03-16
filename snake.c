#include <SDL2/SDL.h>
#include "snake.h"
#include "head.h"

SDL_bool requestTurn(char dir, Head *tail)
{
	Head *head;
	    while(tail != NULL)
	    {
		head=tail;
		tail = tail->next;
	    }
    if(head->dir == 'U' || head->dir == 'D')
    {
        if(dir == 'L' || dir == 'R')
        {
            head->dir = dir;
            return SDL_TRUE;
        }
    }
    else if(head->dir == 'L' || head->dir == 'R')
    {
        if(dir == 'U' || dir == 'D')
        {
            head->dir = dir;
            return SDL_TRUE;
        }        
    }
    return SDL_FALSE;
}

void makeTurn(char dir, Head* tail)
{
	Head *head;
	    while(tail != NULL)
	    {
		head=tail;
		tail = tail->next;
	    }
    switch(dir)
    {
        case 'U':
            head->dir = 'U';
            break;
        case 'R':
            head->dir = 'R';
            break;
        case 'D':
            head->dir = 'D';
            break;
        case 'L':
            head->dir = 'L';
            break;
        default:{}                
    }
}

SDL_bool detectCollision(SDL_Rect rectFoodPos, Head *tail)
{
	Head *snakeHead;
        while(tail != NULL)
        {
            snakeHead=tail;
            tail = tail->next;
        }
	SDL_Rect a = rectFoodPos;
	SDL_Rect b = snakeHead->headRect; 

	//The sides of the rectangles 
	int rightA, rightB; 
	int topA, topB; 
	//Calculate the sides of rect A 
	rightA = a.x + a.w; 
	topA = a.y; 
	//Calculate the sides of rect B 
	rightB = b.x + b.w; 
	topB = b.y; 

	if((abs(rightA-rightB)<=16) && (abs(topA-topB)<=16))
    		return SDL_TRUE;
	else
		return SDL_FALSE;
}



