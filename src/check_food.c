#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "../includes/walls.h"


//function prototypes to be tested


START_TEST(test_collision)
{	

}
END_TEST

Suite* test_food_suite()
{

}

int main(void)
{
	int failed;
	Suite *suite;
	SRunner *runner;
	
	suite = test_food_suite();
	runner = srunner_create(suite);
	
	srunner_run_all(runner, CK_VERBOSE);
	failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	
	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}
