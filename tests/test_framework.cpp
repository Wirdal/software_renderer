#include "test_framework.h"

DECLARE_TEST_SUITE(evector_tests);
DECLARE_TEST_SUITE(matrix_tests);

int main()
{
	RUN_TEST_SUITE(evector_tests);
	RUN_TEST_SUITE(matrix_tests);

	return 0;
}