#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "../includes/walls.h"


//function prototypes to be tested


START_TEST(test_walls)
{	
	int level; //level being tested
	SDL_Rect pos; //position being evalueted
	
	level = 1;
	//Checking positions without walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_FALSE, 
"Warning collision occured");
	
	//Checking positions with walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_TRUE, 
"Warning collision did not occured");
	
	level = 2;
	//Checking positions without walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_FALSE, 
"Warning collision occured");
	
	//Checking positions with walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_TRUE, 
"Warning collision did not occured");
	
	level = 3;
	//Checking positions without walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_FALSE, 
"Warning collision occured");
	
	//Checking positions with walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_TRUE, 
"Warning collision did not occured");
	
	level = 4;
	//Checking positions without walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_FALSE, 
"Warning collision occured");
	
	//Checking positions with walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_TRUE, 
"Warning collision did not occured");
	
	level = 5;
	//Checking positions without walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_FALSE, 
"Warning collision occured");
	
	//Checking positions with walls
	ck_assert_msg (checkWallCollision(level, pos) == SDL_TRUE, 
"Warning collision did not occured");
}
END_TEST

Suite * test_walls_suite()
{
	Suite *suite;
	TCase *core;
	
	suite = suite_create("Snake");
	
	core = tcase_create("Core");
	
	tcase_add_test(core, test_walls);
	
	suite_add_tcase(suite, core);
	
	return suite;
}


int main(void)
{
	int failed;
	Suite *suite;
	SRunner *runner;
	
	suite = test_walls_suite();
	runner = srunner_create(suite);
	
	srunner_run_all(runner, CK_VERBOSE);
	failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	
	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}
