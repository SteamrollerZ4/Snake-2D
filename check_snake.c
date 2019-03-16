#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "head.h"

void makeTurn(char dir, Head *tail);

//used by both head and body elements
void moveForward(char dir, Head *tail);

//head requests to turn
SDL_bool requestTurn(char dir, Head *tail);

//stop game if snake collides
SDL_bool detectCollision(SDL_Rect rectFoodPos, Head *tail);

void randomiseFood(int min, int max) ;

START_TEST(test_collision)
{
	Head tail;
	tail.headRect.x=0;
	tail.headRect.y=0;
	tail.dir = 'U';
	tail.next = NULL;
	
	SDL_Rect pos;
	
	pos.x = 0;
	pos.y = 0;
	
	ck_assert_msg (detectCollision(pos, &tail) == SDL_TRUE, "Collision occured");

}
END_TEST

START_TEST(test_request_turn)
{
/*
This tests the request turn function which governs that the direction which the snake wants to turn in is legal. This test makes sure that the function behaves in a predictable manner. The test inputs all possible direction including the illegal ones.
*/
	Head head;
	head.headRect.x=0;
	head.headRect.y=0;
	head.next = NULL;
	
	head.dir = 'U';	//facing up
	//requesting to turn left
	ck_assert_msg (requestTurn('L', &head) == SDL_TRUE, 
"Facing up, and failed to turn left");
	//requesting to turn right
	ck_assert_msg (requestTurn('R', &head) == SDL_TRUE, 
"Facing up, and failed to turn right");
	//requesting to turn up
	ck_assert_msg (requestTurn('U', &head) == SDL_TRUE, 
"Facing up, and failed to turn up");
	//requesting to turn down
	ck_assert_msg (requestTurn('D', &head) == SDL_TRUE, 
"Facing up, and failed to turn down");
	
	head.dir = 'R'; //facing right
	//requesting to turn up
	ck_assert_msg (requestTurn('U', &head) == SDL_TRUE, 
"Facing right, and failed to turn up");
	//requesting to turn right
	ck_assert_msg (requestTurn('R', &head) == SDL_TRUE, 
"Facing right, and failed to turn right");
	//requesting to turn down
	ck_assert_msg (requestTurn('D', &head) == SDL_TRUE, 
"Facing right, and failed to turn down");
	//requesting to turn left
	ck_assert_msg (requestTurn('L', &head) == SDL_TRUE, 
"Facing right, and failed to turn left");
	
	head.dir = 'D';//facing down
	//requesting to to up
	ck_assert_msg (requestTurn('U', &head) == SDL_TRUE, 
"Facing down, and failed to turn up");
	//requesting to turn right
	ck_assert_msg (requestTurn('R', &head) == SDL_TRUE, 
"Facing down, and failed to turn right");
	//requesting to turn down
	ck_assert_msg (requestTurn('D', &head) == SDL_TRUE,
"Facing down, and failed to turn down");
	//requesting to turn left
	ck_assert_msg (requestTurn('L', &head) == SDL_TRUE, 
"Facing down, and failed to turn left");
	
	head.dir ='L';//facing left
	//requesting to to up
	ck_assert_msg (requestTurn('U', &head) == SDL_TRUE, 
"Facing left, and failed to turn up");
	//requesting to turn right
	ck_assert_msg (requestTurn('R', &head) == SDL_TRUE, 
"Facing left, and failed to turn right");
	//requesting to turn down
	ck_assert_msg (requestTurn('D', &head) == SDL_TRUE, 
"Facing left, and failed to turn down");
	//requesting to turn left
	ck_assert_msg (requestTurn('L', &head) == SDL_TRUE, 
"Facing left, and failed to turn left");
}
END_TEST


Suite *test_snake_suite(void)
{
	Suite *suite;
	TCase *core;
	
	suite = suite_create("Snake");
	
	core = tcase_create("Core");
	
	tcase_add_test(core, test_collision);
	tcase_add_test(core, test_request_turn);
	
	suite_add_tcase(suite, core);
	
	return suite;
}

int main(void)
{
	int failed;
	Suite *suite;
	SRunner *runner;
	
	suite = test_snake_suite();
	runner = srunner_create(suite);
	
	srunner_run_all(runner, CK_VERBOSE);
	failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	
	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}
