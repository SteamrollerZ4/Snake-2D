#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <mysql/mysql.h>
#include "ai.h"
#include "walls.h"
#include <stdio.h>
#include <math.h>

Cell grid[40][30];//grid that stores the state of the level 

/*openSet list is implemented as a queue*/
Cell *openSet;//the set of nodes to be evaluated
Cell *closedSet;//the set of nodes already evaluated
Cell *neighbours;
	
void initialiseGrid(int level)
{
	int x, y;
	for(x=0;x<40;x++)
	for(y=0;y<30;y++)
	{
		grid[x][y].worldPosition.x = x*16;
		grid[x][y].worldPosition.y = y*16;
		//length of each cell
		grid[x][y].worldPosition.w = 16;
		grid[x][y].worldPosition.h = 16;
		
		grid[x][y].next = NULL;
		grid[x][y].parent = NULL;
		
		if(checkWallCollision(level, &grid[x][y].worldPosition) == SDL_FALSE)
		{
			grid[x][y].walkable = SDL_TRUE;
		}
		else
		{
			grid[x][y].walkable = SDL_FALSE;
		}
	}
}


/*statistics function prints a summary of the total 
number of free cells and the total number of occupied cells*/
void statistics()
{
	int x, y, free = 0, occupied = 0;
	for(x=0;x<40;x++)
	for(y=0;y<30;y++){
		if(grid[x][y].walkable == SDL_TRUE)
			free++;
		else
			occupied++;
	}
	
	printf("free cells : %d\n", free);
	printf("occupied cells : %d\n", occupied);
}

//A* search algorithm
SDL_bool findPath(SDL_Rect start, SDL_Rect dest)
{
	Cell startCell = cellFromPos(start);
	Cell destCell = cellFromPos(dest);
	
	//clear the openSet and closedSet
	openSet = NULL;
	closedSet = NULL;
	
	addOpenSet(&startCell);

	while(setCount(openSet) > 0)
	{
		Cell *current = openSet;//this is the starting node
	
		int i; Cell* temp = openSet;
		temp=temp->next;//skip the first element
		for(i=1;i<setCount(openSet);i++)
		{
			if(fCost(temp) < fCost(current) || fCost(temp) == fCost(current) && (temp)->h_cost < current->h_cost)
			{
				current = temp;
			}
			temp=temp->next;
		}

		removeOpenSet(current);
		addClosedSet(current);
	
		//destination has been reached
		if(current->worldPosition.x == destCell.worldPosition.x &&
		current->worldPosition.y == destCell.worldPosition.y){
		return SDL_TRUE;
		}
		
		getNeighbours(current->worldPosition);
		
		Cell* temp2 = neighbours;
		int x=0;
		for (; x<4 && temp2 != NULL;x++)
		{
			if(temp2->worldPosition.x > 640 || temp2->worldPosition.y > 480){
				printf("Out of grid array bounds(%d,%d)\n",temp2->worldPosition.x,temp2->worldPosition.y);
				return;
			}
			if(grid[temp2->worldPosition.x/16][temp2->worldPosition.y/16].walkable == SDL_FALSE || contains(closedSet, temp2) == SDL_TRUE)
			{

			}else{
		
				int newMovementCostToNeighbours = current->g_cost + getDistance(current, temp2);
			
				if(newMovementCostToNeighbours < temp2->g_cost || contains(openSet,temp2) == SDL_FALSE)
				{
					temp2->g_cost = newMovementCostToNeighbours;
					temp2->h_cost = getDistance(temp2, &destCell);
					temp2->parent = current;
				
					if(contains(openSet,temp2) == SDL_FALSE)
					{
						addOpenSet(temp2);
					}
				}
			}
			temp2 = temp2->next;
		}
	}
}

//calculates the distance between two cells
int getDistance(Cell *c1, Cell *c2)
{
	int x = abs(c1->worldPosition.x - c2->worldPosition.x);
	int y = abs(c1->worldPosition.y - c2->worldPosition.y);
	
	return (x+y);
}

//returns the neighbours of the passed SDL_Rect
void getNeighbours(SDL_Rect pos)
{
	neighbours = NULL;
	SDL_Rect *positions = (SDL_Rect*) malloc(sizeof(SDL_Rect)*4);
	(positions)->x = pos.x+16;
	(positions)->y = pos.y;
	(positions + 1)->x = pos.x-16;
	(positions + 1)->y = pos.y;
	(positions + 2)->x = pos.x;
	(positions + 2)->y = pos.y-16;
	(positions + 3)->x = pos.x;
	(positions + 3)->y = pos.y+16;
	
	Cell c1 = cellFromPos(*positions);
	Cell c2 = cellFromPos(*(positions+1));
	c1.next=&c2;
	Cell c3 = cellFromPos(*(positions+2));
	c2.next=&c3;
	Cell c4 = cellFromPos(*(positions+3));
	c3.next=&c4;
	neighbours = &c1;
}

//creates a Cell given a SDL_Rect
Cell cellFromPos(SDL_Rect pos)
{
	Cell myCell;
	myCell.worldPosition = pos;
	myCell.walkable = SDL_TRUE;
	myCell.next = NULL;
	myCell.parent = NULL;
	return myCell;
}

int fCost(Cell *c)
{
return c->g_cost + c->h_cost;
}

//add an element to open set
void addOpenSet(Cell *c)
{
	if(openSet == NULL)//list is empty
		openSet = c;
	else{//enqueue new cell onto the list
		Cell *temp = openSet ;
		openSet = c;
		c->next = temp;
	}
}

//remove element from the open list
void removeOpenSet(Cell *c)
{

	if(openSet == NULL || c == NULL)
		return;
	
	Cell *temp = openSet;
	int i;
	Cell* prev;
	
	for(i=0;i<setCount(temp);i++)
	{
		if(temp->worldPosition.x == c->worldPosition.x &&
		temp->worldPosition.y == c->worldPosition.y)
		{
			//remove this cell
			if(prev == NULL)
				openSet = openSet->next;
			else{
				prev->next = temp->next;
				temp->next = NULL;
			}
			break;
		}
		prev = temp;
		temp = temp->next;
	}
	
}

//add cell to closed set
void addClosedSet(Cell *c)
{
	if(closedSet == NULL)//list is empty
		closedSet = c;
	else{//enqueue new cell onto the list
		Cell *temp = closedSet ;
		closedSet = c;
		c->next = temp;
	}
}

int setCount(Cell *c)
{
	Cell* temp = c;
	int count = 0;
	while(temp != NULL)
	{
		count++;
		temp=temp->next;
	}
	return count;	
}

//returns true if list contains the cell c
SDL_bool contains(Cell *list, Cell *c)
{
	if(list == NULL || c == NULL)
		return SDL_FALSE;
		
	Cell *temp = list;
	while(temp != NULL)
	{
		if(temp->worldPosition.x == c->worldPosition.x &&
		temp->worldPosition.y == c->worldPosition.y)
		{
			return SDL_TRUE;
		}
		temp=temp->next;
	}
	return SDL_FALSE;
}
