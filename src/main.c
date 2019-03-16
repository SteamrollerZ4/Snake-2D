#include <stdio.h> 
#include <stdlib.h> 
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h> 
#include <SDL2/SDL_timer.h> 
#include <SDL2/SDL_image.h> 
#include <math.h> 
#include "../includes/head.h"
#include "../includes/snake.h"
#include "../includes/walls.h"
#include "../includes/gui.h"
#include "../includes/ai.h"

#define SCREEN_w 640
#define SCREEN_h 480
#define SCREEN_SCALE 1
#define SCREEN_NAME "Snake2d"
#define LIMIT_X_H 640
#define LIMIT_X_L 0
#define LIMIT_Y_H 480
#define LIMIT_Y_L 0

SDL_Rect rectFoodPos;

void displaySnake();
void displayFood();
void setupSpriteSheet();
void randomiseFood(int min, int max);
void eatFood();
void nextLevel();
void updateScore();
void resetSnake();
void createSnake(Head * snakeHead);

void game_init(void); //game object initialisation
void game_quit(void); //game object clean up

//This is the main game object, and it is a singleton
static struct {
  //define "attributes"
  SDL_bool running;
  Head * tail;

  struct {
    unsigned int w;
    unsigned int h;
    const char * name;
    SDL_Window * window;
    SDL_Renderer * renderer;
  }
  screen;

  struct {
    unsigned int n;
    SDL_Surface ** spritesheet;
    SDL_Surface * surfaceBack;
  }
  gfx;

  //define "methods"
  void( * init)(void);
  void( * quit)(void);

}
Game = {
  SDL_FALSE,
  NULL,
  {
    SCREEN_SCALE * SCREEN_w,
    SCREEN_SCALE * SCREEN_h,
    SCREEN_NAME,
    NULL,
    NULL
  },
  {
    0,
    NULL,
    NULL
  },
  game_init,
  game_quit
};

unsigned int lastTime = 0, currentTime;
unsigned int lastSnakeMoveTime = 0, currentSnakeMoveTime;
SDL_bool nextFrame = SDL_FALSE;
int frameWait = 200;
SDL_bool start = SDL_FALSE;
int level = 1, levelChange = 0;
int fruitsEaten = 0;
int points = 0;
char score[8];
SDL_Texture * texureGUI;
SDL_Texture * textureBack;


int main(int argc, char ** argv) {
  Game.init();
  SDL_Event event;

  updateScore();

  rectFoodPos.w = 16;
  rectFoodPos.h = 16;

  textureBack = SDL_CreateTextureFromSurface(
    Game.screen.renderer, Game.gfx.surfaceBack);

  texureGUI = SDL_CreateTextureFromSurface(
    Game.screen.renderer, displayGUI(score));

  SDL_Rect guiRect;
  guiRect.x = 600;
  guiRect.y = 20;
  guiRect.w = 20;
  guiRect.h = 20;

  if (textureBack == NULL) {
    printf("Error creating texture: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Rect rectBack = {
    0,
    0,
    640,
    480
  };

  Head * snakeHead = (Head * ) malloc(sizeof(Head));
  createSnake(snakeHead);
  resetSnake();
  randomiseFood(SCREEN_h - 16, SCREEN_w - 16);

  while (Game.running) {
    if (nextFrame == SDL_TRUE) {

      if (detectCollision(rectFoodPos, Game.tail) == SDL_TRUE) {
        srand(SDL_GetTicks());
        eatFood();
        if (levelChange != level) {
          textureBack = SDL_CreateTextureFromSurface(
            Game.screen.renderer, Game.gfx.surfaceBack);
          levelChange = level;
        }

        randomiseFood(SCREEN_h - 16, SCREEN_w - 16);

      }
      while (SDL_PollEvent( & event)) {
        switch (event.type) {
        case SDL_QUIT:
          Game.running = SDL_FALSE;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
          case SDL_SCANCODE_0:
            start = SDL_FALSE;
            break;
          case SDL_SCANCODE_W:
          case SDL_SCANCODE_UP:
            if (start == SDL_FALSE)
              start = SDL_TRUE;
            if (requestTurn('U', Game.tail)) {
              makeTurn('U', Game.tail);
            }
            break;
          case SDL_SCANCODE_A:
          case SDL_SCANCODE_LEFT:
            if (start == SDL_FALSE)
              start = SDL_TRUE;
            if (requestTurn('L', Game.tail)) {
              makeTurn('L', Game.tail);
            }
            break;
          case SDL_SCANCODE_S:
          case SDL_SCANCODE_DOWN:
            if (start == SDL_FALSE)
              start = SDL_TRUE;
            if (requestTurn('D', Game.tail)) {
              makeTurn('D', Game.tail);
            }
            break;
          case SDL_SCANCODE_D:
          case SDL_SCANCODE_RIGHT:
            if (start == SDL_FALSE)
              start = SDL_TRUE;
            if (requestTurn('R', Game.tail)) {
              makeTurn('R', Game.tail);
            }
            break;
          default:
            {}
          }
          break;
        default:
          {}
        }
      }
	SDL_RenderClear(Game.screen.renderer);
	
	SDL_RenderCopy(Game.screen.renderer, 
	textureBack, NULL, &rectBack);
	
	
	displayFood();
	
	displaySnake();
	
	SDL_RenderCopy(Game.screen.renderer, 
	texureGUI, NULL, &guiRect);
	
	
	SDL_RenderPresent(Game.screen.renderer);
    }
    
    currentTime = SDL_GetTicks();
    
    currentSnakeMoveTime = SDL_GetTicks();

    if (currentTime > lastTime + 200) {
      lastTime = currentTime;
      nextFrame = SDL_TRUE;
    } else
      nextFrame = SDL_FALSE;

    if (currentSnakeMoveTime > lastSnakeMoveTime + frameWait) {
    
      lastSnakeMoveTime = currentSnakeMoveTime;
      
      if (start == SDL_TRUE) {
        moveForward(snakeHead -> dir, Game.tail);
        if (checkWallCollision(level, 
        & snakeHead -> headRect) == SDL_TRUE) {
		//game over you collided with the wall
		printf("Game over\n");
		resetSnake();
		points = 0;
		start = 0;
		updateScore();
		level=1;
		nextLevel();
        textureBack = SDL_CreateTextureFromSurface(
        Game.screen.renderer, Game.gfx.surfaceBack);
        }
      }
    }
  }

  SDL_DestroyTexture(textureBack);
  Game.quit();
  return EXIT_SUCCESS;
}

void resetSnake() {
  while (Game.tail -> next -> next != NULL) {
    free(Game.tail);
    Game.tail = Game.tail -> next;
  }
  Game.tail -> headRect.x = SCREEN_w / 2;
  Game.tail -> headRect.y = SCREEN_h / 2;

  Game.tail -> dir = 'U';

  Game.tail -> next -> headRect.x = SCREEN_w / 2;
  Game.tail -> next -> headRect.y = SCREEN_h / 2 - 16;
  Game.tail -> next -> dir = 'U';
  
  initialiseGrid(level);
}

void createSnake(Head * snakeHead) {

  if (!Game.tail)
    Game.tail = (Head * ) malloc(sizeof(Head));

  Game.tail -> headRect.x = SCREEN_w / 2;
  Game.tail -> headRect.y = SCREEN_h / 2;
  Game.tail -> headRect.w = 16;
  Game.tail -> headRect.h = 16;

  Game.tail -> dir = 'U';

  Game.tail -> next = snakeHead;
  snakeHead -> next = NULL;

  snakeHead -> headRect.x = SCREEN_w / 2;
  snakeHead -> headRect.y = SCREEN_h / 2 - 16;
  snakeHead -> headRect.w = 16;
  snakeHead -> headRect.h = 16;
  snakeHead -> dir = 'U';
}

void updateScore() {
  sprintf(score, "%d", points);
  texureGUI = SDL_CreateTextureFromSurface(
    Game.screen.renderer, displayGUI(score));
}

void nextLevel() {
  switch (level) {
  case 1:
  
    Game.gfx.surfaceBack = IMG_Load(
"../assets/level1.png");
    
    break;
  case 2:
  
    Game.gfx.surfaceBack = IMG_Load(
"../assets/level2.png");
    
    break;
  case 3:
  
    Game.gfx.surfaceBack = IMG_Load(
"../assets/level3.png");
    
    break;
  case 4:
  
    Game.gfx.surfaceBack = IMG_Load(
"../assets/level4.png");
    
    break;
  case 5:
    Game.gfx.surfaceBack = IMG_Load(
"../assets/level5.png");
    
    break;
  default:
    {}
  }
  resetSnake();
}

void randomiseFood(int min, int max) {
  while(1){
	  srand(SDL_GetTicks());
	  rectFoodPos.x = rand() % max;
	  rectFoodPos.y = rand() % min;
	  
	  //make the coordinates of food pos a multiple of 16
	  int remainder = rectFoodPos.x % 16;
	  if(rectFoodPos.x>320)rectFoodPos.x-=remainder;
	  else{rectFoodPos.x+=(16-remainder);}
	  
	  remainder = rectFoodPos.y % 16;
	  
	  if(rectFoodPos.y>240)rectFoodPos.y-=remainder;
	  else{rectFoodPos.y+=(16-remainder);}
	  
	  if (checkWallCollision(level,&rectFoodPos) == SDL_FALSE) 
	  {
	  	Head *head = Game.tail;
	  	while(head->next != NULL)
	  		head = head->next;
	  	
	  	//printf("Head pos:(%d,%d)\n",head->headRect.x, head->headRect.y);
	  	//printf("Food pos:(%d,%d)\n",rectFoodPos.x, rectFoodPos.y);
	    //if(findPath(head->headRect, rectFoodPos) == SDL_TRUE)
	    //	printf("Path found");       	
       	
       		   
	  	break;
	  }
  }

}

void eatFood() {
  points++; //Add points
  Head * node = (Head * ) malloc(sizeof(Head));
  updateScore();

  switch (Game.tail -> dir) {
  case 'D':
    node -> headRect.x = Game.tail -> headRect.x;
    node -> headRect.y = Game.tail -> headRect.y - STEP;
    break;
  case 'U':
    node -> headRect.x = Game.tail -> headRect.x;
    node -> headRect.y = Game.tail -> headRect.y + STEP;
    break;
  case 'L':
    node -> headRect.x = Game.tail -> headRect.x + STEP;
    node -> headRect.y = Game.tail -> headRect.y;
    break;
  case 'R':
    node -> headRect.x = Game.tail -> headRect.x - STEP;
    node -> headRect.y = Game.tail -> headRect.y;
    break;
  default:
    {}
  }
  node -> next = Game.tail;
  Game.tail = node;

  fruitsEaten++;
  if (fruitsEaten >= 12) {
    level++;
    nextLevel();
    fruitsEaten = 0;
  }
  /*wrap around and go back to
   first level with greater speed*/
  if (level > 5) {
    //increase speed
    level = 0;
  }
}

void moveForward(char dir, Head * tail) {
  Head * snakeHead = tail;
  while (snakeHead -> next != NULL) {
    snakeHead = snakeHead -> next;
  }

  Head * node = tail;
  while (node -> next != NULL) {
    node -> headRect = node -> next -> headRect;
    node -> dir = node -> next -> dir;
    node = node -> next;
  }

  switch (dir) {
  case 'R':
    snakeHead  -> headRect.x += STEP;
    //wrap snake around the screen
    if (snakeHead -> headRect.x > LIMIT_X_H - 8)
      snakeHead -> headRect.x = LIMIT_X_L;
    break;
  case 'L':
    snakeHead -> headRect.x -= STEP;
    //wrap snake around the screen
    if (snakeHead -> headRect.x < LIMIT_X_L)
      snakeHead -> headRect.x = LIMIT_X_H - 16;
    break;
  case 'U':
    snakeHead -> headRect.y -= STEP;
    //wrap snake around the screen
    if (snakeHead -> headRect.y < LIMIT_Y_L)
      snakeHead -> headRect.y = LIMIT_Y_H - 16;
    break;
  case 'D':
    snakeHead -> headRect.y += STEP;
    //wrap snake around the screen
    if (snakeHead -> headRect.y > LIMIT_Y_H - 8)
      snakeHead -> headRect.y = LIMIT_Y_L;
    break;
  default:
    {}
  }
}

void displayFood() {
  SDL_Texture * texture = SDL_CreateTextureFromSurface(
Game.screen.renderer, *(Game.gfx.spritesheet + 15));

  SDL_RenderCopy(Game.screen.renderer, texture, NULL, 
  &rectFoodPos);

  
  SDL_DestroyTexture(texture);
}

void setupSpriteSheet() {
  SDL_Surface * surface = IMG_Load(
"../assets/Snake.png");

  if (surface == NULL) {
    printf("Error while openning spritesheet:\n%s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  int n = ((surface -> w / 16) * (surface -> h / 16) + 1);

  Game.gfx.n = n;
  Game.gfx.spritesheet = 
(SDL_Surface ** ) malloc(sizeof(SDL_Surface * ) * n);
  
  Game.gfx.surfaceBack = (SDL_Surface * ) malloc(sizeof(SDL_Surface));

  Game.gfx.surfaceBack = IMG_Load(
"../assets/level1.png");

  if (Game.gfx.surfaceBack == NULL) {
    printf("Error openning image: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  int i, x, y;
  SDL_Rect rect = {
    0,
    0,
    16,
    16
  };
  for (i = 0; i < n; i++) {
    Game.gfx.spritesheet[i] = 
SDL_CreateRGBSurface(0, 16, 16, 32, 0x00, 0x00, 0x00, 0x00);

    SDL_SetColorKey(Game.gfx.spritesheet[i], 1, 0xFF00FF);
    
    SDL_FillRect(Game.gfx.spritesheet[i], 0, 0xFF00FF);
    
    if (i != 0) {
      x = (i - 1) % (surface -> w / 16);
      y = (i - x) / (surface -> w / 16);
      rect.x = x * 16;
      rect.y = y * 16;
      SDL_BlitSurface(surface, & rect, Game.gfx.spritesheet[i], 
NULL);
  
    }
  }
  SDL_FreeSurface(surface);
}

void game_init(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error initialisation SDL %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  unsigned int w = Game.screen.w;
  unsigned int h = Game.screen.h;
  const char * name = Game.screen.name;

  Game.screen.window = SDL_CreateWindow(
    name,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    w, h, 0
  );

  if (Game.screen.window == NULL) {
    printf("Couldn't create window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  Game.screen.renderer = SDL_CreateRenderer(
    Game.screen.window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (Game.screen.renderer == NULL) {
    printf("Couldn't create renderer: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  setupSpriteSheet();

  Game.running = SDL_TRUE;
}

void game_quit(void) {
  int i;
  for (i = 0; i < Game.gfx.n; i++)
    SDL_FreeSurface(Game.gfx.spritesheet[i]);

  SDL_FreeSurface(Game.gfx.surfaceBack);

  free(Game.gfx.spritesheet);
  Game.gfx.spritesheet = NULL;

  free(Game.tail);

  SDL_DestroyRenderer(Game.screen.renderer);
  Game.screen.renderer = NULL;

  SDL_DestroyWindow(Game.screen.window);
  Game.screen.window = NULL;

  SDL_Quit();
  Game.running = SDL_FALSE;
}

void displaySnake() {
  Head * current = Game.tail;

  SDL_Texture * texture;

  char * prevChar = NULL;
  while (current != NULL) {
    switch (current -> dir) {
    case 'U':
      if (!prevChar) { //this is the tail
        prevChar = (char * ) malloc(sizeof(char));
        texture = SDL_CreateTextureFromSurface(
          Game.screen.renderer, *(Game.gfx.spritesheet + 5));
        if (texture == NULL) {
          printf("Error creating texture: %s\n", SDL_GetError());
          exit(EXIT_FAILURE);
        }
      } else {
        if (!current  -> next) { //this is the head
          texture = SDL_CreateTextureFromSurface(
            Game.screen.renderer, *(Game.gfx.spritesheet + 1));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        } else { //not the head! so perform pattern recognition
          if ( * prevChar == 'L')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 9));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          } else if ( * prevChar == 'R')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 12));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          } else if ( * prevChar == 'U')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 13));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        }

      }
      * prevChar = 'U';
      break;
    case 'D':
      if (!prevChar) { //this is the tail
        prevChar = (char * ) malloc(sizeof(char));
        texture = SDL_CreateTextureFromSurface(
          Game.screen.renderer, *(Game.gfx.spritesheet + 7));
        if (texture == NULL) {
          printf("Error creating texture: %s\n", SDL_GetError());
          exit(EXIT_FAILURE);
        }
      } else {

        if (!current -> next) { //this is the head
          texture = SDL_CreateTextureFromSurface(
            Game.screen.renderer, *(Game.gfx.spritesheet + 3));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        } else { //not the head! so perform pattern recognition
          if ( * prevChar == 'L')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 10));
          else if ( * prevChar == 'R')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 11));
          else if ( * prevChar == 'D')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 13));
        }
      }
      * prevChar = 'D';
      break;
    case 'L':
      if (!prevChar) { //this is the tail
        prevChar = (char * ) malloc(sizeof(char));
        texture = SDL_CreateTextureFromSurface(
          Game.screen.renderer, *(Game.gfx.spritesheet + 8));
        if (texture == NULL) {
          printf("Error creating texture: %s\n", SDL_GetError());
          exit(EXIT_FAILURE);
        }
      } else {

        if (!current -> next) { //this is the head
          texture = SDL_CreateTextureFromSurface(
            Game.screen.renderer, *(Game.gfx.spritesheet + 4));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        } else { //not the head! so perform pattern recognition
          if ( * prevChar == 'L')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 14));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          } else if ( * prevChar == 'D')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 12));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          } else if ( * prevChar == 'U')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 11));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        }
      }
      * prevChar = 'L';
      break;
    case 'R':
      if (!prevChar) { //this is the tail
        prevChar = (char * ) malloc(sizeof(char));
        texture = SDL_CreateTextureFromSurface(
          Game.screen.renderer, *(Game.gfx.spritesheet + 6));
        if (texture == NULL) {
          printf("Error creating texture: %s\n", SDL_GetError());
          exit(EXIT_FAILURE);
        }
      } else {

        if (!current -> next) { //this is the head
          texture = SDL_CreateTextureFromSurface(
            Game.screen.renderer, *(Game.gfx.spritesheet + 2));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        } else { //not the head! so perform pattern recognition
          if ( * prevChar == 'R')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 14));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          } else if ( * prevChar == 'D')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 9));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          } else if ( * prevChar == 'U')
            texture = SDL_CreateTextureFromSurface(
              Game.screen.renderer, *(Game.gfx.spritesheet + 10));
          if (texture == NULL) {
            printf("Error creating texture: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
          }
        }
      }
      * prevChar = 'R';
      break;
    default:
      {}
    }
    
    SDL_RenderCopy(Game.screen.renderer, texture, 
    NULL, &current -> headRect);

    current = current -> next;
  }

  SDL_DestroyTexture(texture);
  
}
