#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/walls.h"

SDL_bool checkWallCollision(int level, SDL_Rect *pos)
{
	switch(level)
	{
		case 1://There are no walls on level 1
			return SDL_FALSE;
		break;
		case 2://Box
		if(pos->x <= 0 || pos->x >= 624 || 
		pos->y <= 0 || pos->y >= 462)
		{
			return SDL_TRUE;
		}else
		{
			return SDL_FALSE;
		}
		break;
		case 3://cross
		if(pos->x == 320 || pos->y == 240)
		{
			return SDL_TRUE;
		}else
		{
			return SDL_FALSE;
		}
		break;
		case 4://Pool table
		
		break;
		case 5://Apartment
		
		break;
		default:{}
	}
}
