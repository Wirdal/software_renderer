#include "test_framework.h"
#include "math/matrix.h"
#include <iostream>

TEST_SUITE(matrix_tests)
{
	// Row-major storage test
	/*
		1 2 3
		4 5 6
		7 8 9
		Row 1: 1 2 3, thus the first evector should be (1,2,3)
	*/
	{
		matrix<3, 3, float> mat;
		mat[0][0] = 1.0f; mat[0][1] = 2.0f; mat[0][2] = 3.0f;
		mat[1][0] = 4.0f; mat[1][1] = 5.0f; mat[1][2] = 6.0f;
		mat[2][0] = 7.0f; mat[2][1] = 8.0f; mat[2][2] = 9.0f;
		CHECK_TRUE(mat[0][0] == 1.0f);
		CHECK_TRUE(mat[0][1] == 2.0f);
		CHECK_TRUE(mat[0][2] == 3.0f);
		CHECK_TRUE(mat[1][0] == 4.0f);
		CHECK_TRUE(mat[1][1] == 5.0f);
		CHECK_TRUE(mat[1][2] == 6.0f);
		CHECK_TRUE(mat[2][0] == 7.0f);
		CHECK_TRUE(mat[2][1] == 8.0f);
		CHECK_TRUE(mat[2][2] == 9.0f);
	}

	// Access and modify test
	{
		matrix<3, 3, float> mat;
		mat[0][0] = 1.0f; mat[0][1] = 2.0f;
		mat[1][0] = 3.0f; mat[1][1] = 4.0f;

		CHECK_TRUE(mat[0][0] == 1.0f);
		CHECK_TRUE(mat[0][1] == 2.0f);
		CHECK_TRUE(mat[1][0] == 3.0f);
		CHECK_TRUE(mat[1][1] == 4.0f);

		mat[0][1] = 5.0f; // modify element

		CHECK_TRUE(mat[0][1] == 5.0f);
	}

	//  Matrix product test
	{
		matrix<3, 3, float> first;
		first[0][0] = 1.0f; first[0][1] = 2.0f; first[0][2] = 3.0f;
		first[1][0] = 4.0f; first[1][1] = 5.0f; first[1][2] = 6.0f;
		first[2][0] = 7.0f; first[2][1] = 8.0f; first[2][2] = 9.0f;

		matrix<3, 3, float> second;
		second[0][0] = 10.0f; second[0][1] = 11.0f; second[0][2] = 12.0f;
		second[1][0] = 13.0f; second[1][1] = 14.0f; second[1][2] = 15.0f;
		second[2][0] = 16.0f; second[2][1] = 17.0f; second[2][2] = 18.0f;

		matrix<3, 3, float> result = first * second;

		// Check all elements of the result matrix for correctness
		CHECK_TRUE(result[0][0] == (1.0f*10.0f + 2.0f*13.0f + 3.0f*16.0f)); // 1*10 + 2*13 + 3*16 = 10 + 26 + 48 = 84
		CHECK_TRUE(result[0][1] == (1.0f*11.0f + 2.0f*14.0f + 3.0f*17.0f)); // 1*11 + 2*14 + 3*17 = 11 + 28 + 51 = 90
		CHECK_TRUE(result[0][2] == (1.0f*12.0f + 2.0f*15.0f + 3.0f*18.0f)); // 1*12 + 2*15 + 3*18 = 12 + 30 + 54 = 96

		CHECK_TRUE(result[1][0] == (4.0f*10.0f + 5.0f*13.0f + 6.0f*16.0f)); // 4*10 + 5*13 + 6*16 = 40 + 65 + 96 = 201
		CHECK_TRUE(result[1][1] == (4.0f*11.0f + 5.0f*14.0f + 6.0f*17.0f)); // 4*11 + 5*14 + 6*17 = 44 + 70 + 102 = 216
		CHECK_TRUE(result[1][2] == (4.0f*12.0f + 5.0f*15.0f + 6.0f*18.0f)); // 4*12 + 5*15 + 6*18 = 48 + 75 + 108 = 231

		CHECK_TRUE(result[2][0] == (7.0f*10.0f + 8.0f*13.0f + 9.0f*16.0f)); // 7*10 + 8*13 + 9*16 = 70 + 104 + 144 = 318
		CHECK_TRUE(result[2][1] == (7.0f*11.0f + 8.0f*14.0f + 9.0f*17.0f)); // 7*11 + 8*14 + 9*17 = 77 + 112 + 153 = 342
		CHECK_TRUE(result[2][2] == (7.0f*12.0f + 8.0f*15.0f + 9.0f*18.0f)); // 7*12 + 8*15 + 9*18 = 84 + 120 + 162 = 366
	}
}
TEST_SUITE_END