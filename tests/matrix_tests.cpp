#include "test_framework.h"
#include "math/matrix.h"
#include "math/transformation_matrix.h"
#include <iostream>
#include <numbers>

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

	// Identity Matrix
	{
		auto id = matrix<3, 3, float>::identity();
	
		CHECK_TRUE(id[0][0] == 1.0f);
		CHECK_TRUE(id[1][1] == 1.0f);
		CHECK_TRUE(id[2][2] == 1.0f);

		CHECK_TRUE(id[2][0] == 0.0f);

		// Fails to compile
		// auto id = matrix<3, 2, float>::identity();
	}

	// Growth
	{
		auto id = matrix<3, 3, float>::identity();
		matrix<4, 4, float> growth = id.grow<4,4>();

		// Identity should be the same
		CHECK_TRUE(growth[0][0] == 1.0f);
		CHECK_TRUE(growth[1][1] == 1.0f);
		CHECK_TRUE(growth[2][2] == 1.0f);

		CHECK_TRUE(growth[2][0] == 0.0f);
		// But there should be more as well

		CHECK_TRUE(growth[3][3] == 0.0f);

	}

	// Translation via homogenous coords
	// This can be done simply by taking an identity matrix and setting the last row to the desired translation
	{
		translation_matrix<float> mat = matrix<4, 4, float>::identity();
		// set up the tranlations
		mat[3][0] = 10.0f; mat[3][1] = 10.0f; mat[3][2] = 10.0f;

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};

		// Then we can dot product

		// 1x4 dot 4x4 matrix = 1x4 matrix
		matrix<1, 4, float> after = before.transpose() * mat;

		// With the identity matrix, this should be equivelent to 
		// = r_x + t_x, r_y + t_y, r_z + t_z, 1
		CHECK_TRUE(after[0][0] == 11.0f);
		CHECK_TRUE(after[0][1] == 12.0f);
		CHECK_TRUE(after[0][2] == 13.0f);
		CHECK_TRUE(after[0][3] == 1.0f);

		// Inversion of the matrix, which pretty much reverse the translations, can be done by inverting the vertex t

		before = {-1.0f, -2.0f, -3.0f, 1.0f};
		after = before.transpose() * mat;
		CHECK_TRUE(after[0][0] == 9.0f);
		CHECK_TRUE(after[0][1] == 8.0f);
		CHECK_TRUE(after[0][2] == 7.0f);
		CHECK_TRUE(after[0][3] == 1.0f);
	}

	// Rotation via hoogenous coordinates
	// Tests rotation about different axies
	// I can combine translation and rotation in their own columns...
	{
		translation_matrix<float> mat = matrix<4, 4, float>::identity();

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};

		// Rotation about x axis with angel phi
		constexpr float pi = std::numbers::pi_v<float>;
		mat[1][1] = std::cos(pi);
		mat[1][2] = std::sin(pi);
		mat[2][1] = -std::sin(pi);
		mat[2][2] = std::cos(pi);

		matrix<1, 4, float> after = before.transpose() * mat;

		constexpr evector<float, 4> afterXCheck{1.0f, -2.0f, -3.0f, 1.0f};
		CHECK_TRUE(after == afterXCheck.transpose());
	}
	
	// Rotation about Y
	{
		translation_matrix<float> mat = matrix<4, 4, float>::identity();

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};

		// Rotation about y axis with angel phi
		constexpr float pi = std::numbers::pi_v<float>;
		mat[0][0] = std::cos(pi);
		mat[0][2] = -std::sin(pi);
		mat[2][0] = std::sin(pi);
		mat[2][2] = std::cos(pi);

		matrix<1, 4, float> after = before.transpose() * mat;

		constexpr evector<float, 4> afterXCheck{-1.0f, 2.0f, -3.0f, 1.0f};
		CHECK_TRUE(after == afterXCheck.transpose());
	}
}
TEST_SUITE_END