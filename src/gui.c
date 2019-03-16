#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/gui.h"

TTF_Font * font;

SDL_Surface* displayGUI(const char *text)
{
 	TTF_Init();
	if (!font)
		font = TTF_OpenFont("../assets/FutureTimeSplitters.otf", 50);
		
	SDL_Color color={255, 255, 255};
	SDL_Surface * scoreText = TTF_RenderText_Solid(font,text, color);
	return scoreText;
}
