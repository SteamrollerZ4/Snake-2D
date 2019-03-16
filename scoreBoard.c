#include <SDL2/SDL.h>
#include <mysql/mysql.h>
#include "scoreBoard.h"


SDL_bool updateBoard()
{
    MYSQL *conn;
    
    conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("Error %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(SDL_FALSE);
    }

    if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 8889, NULL, 0)) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        exit(SDL_FALSE);
    }
    
    
    if (mysql_query(conn, "create database testdb")) {
        printf("Error %u: %s",mysql_errno(conn), mysql_error(conn));
        exit(SDL_FALSE);
    }

    mysql_close(conn);
    
    return SDL_TRUE;
}

/*
checkScore returns SDL_FALSE is score isn't good enough for top 10,
else returns SDL_TRUE
*/
SDL_bool checkScore(int score)
{
	char *query = "SELECT * FROM score_board";
	
}

//returns the position that the passed score must take
int insertionSort(int score)
{

}

//uploads the passed score into the position specified
SDL_bool uploadScore(int position, int score, char *name)
{

}

