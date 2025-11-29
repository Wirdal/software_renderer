#include "test_framework.h"
#include "math/matrix.h"

int main()
{

	TEST_SUITE("Matrix basic operations")
	{
		// Row-major storage test
		/*
			1 2 3
			4 5 6
			7 8 9
			Row 1: 1 2 3, thus the first evector should be (1,2,3)
		*/
		{
			EMatrix<3, 3, float> mat;
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
			EMatrix<3, 3, float> mat;
			mat[0][0] = 1.0f; mat[0][1] = 2.0f;
			mat[1][0] = 3.0f; mat[1][1] = 4.0f;

			CHECK_TRUE(mat[0][0] == 1.0f);
			CHECK_TRUE(mat[0][1] == 2.0f);
			CHECK_TRUE(mat[1][0] == 3.0f);
			CHECK_TRUE(mat[1][1] == 4.0f);

			mat[0][1] = 5.0f; // modify element

			CHECK_TRUE(mat[0][1] == 5.0f);
		}
	}
	TEST_SUITE_END
	return 0;
}