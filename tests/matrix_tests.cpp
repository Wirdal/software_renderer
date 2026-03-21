#include "test_framework.h"
#include "math/matrix.h"
#include "math/transformation_matrix.h"
#include <iostream>
#include <numbers>

#include <vector>

template<size_t Rows, size_t Cols, typename T>
static void print_matrix(const matrix<Rows, Cols, T>& mat)
{
	for (size_t row = 0; row < Rows; row++)
	{
		for (size_t col = 0; col < Cols; col++)
		{
			std::cout << mat[row][col] << " ";
		}
		std::cout << "\n";
	}
}

template<typename T, size_t Dimensions>
static void print_vector(const evector<T, Dimensions>& vec)
{
	for (size_t i = 0; i < Dimensions; i++)
	{
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";
}

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
		matrix<4, 4, float> mat = matrix<4, 4, float>::translation(10.0f);

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
		matrix<4, 4, float> mat = matrix<4,4,float>::rotation_x(std::numbers::pi_v<float>);

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};

		matrix<1, 4, float> after = before.transpose() * mat;

		constexpr evector<float, 4> afterXCheck{1.0f, -2.0f, -3.0f, 1.0f};
		CHECK_TRUE(after == afterXCheck.transpose());
	}
	
	// Rotation about Y
	{
		matrix<4, 4, float> mat = matrix<4,4,float>::rotation_y(std::numbers::pi_v<float>);

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};

		matrix<1, 4, float> after = before.transpose() * mat;

		constexpr evector<float, 4> afterXCheck{-1.0f, 2.0f, -3.0f, 1.0f};
		CHECK_TRUE(after == afterXCheck.transpose());
	}

	// Rotation about Z
	{
		matrix<4, 4, float> mat = matrix<4,4,float>::rotation_z(std::numbers::pi_v<float>);

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};

		matrix<1, 4, float> after = before.transpose() * mat;

		constexpr evector<float, 4> afterXCheck{-1.0f, -2.0f, 3.0f, 1.0f};
		CHECK_TRUE(after == afterXCheck.transpose());
	}
	// Test inverse after?

	// Scale
	// uniform scale
	// Can be be done in _any_ order if uniform and rotation
	{
		matrix<4, 4, float> mat = matrix<4,4,float>::scaling(5.0f);

		evector<float, 4> before{1.0f, 2.0f, 3.0f, 1.0f};
		matrix<1, 4, float> after = before.transpose() * mat;

		CHECK_TRUE(after[0][0]= 5.0f);
		CHECK_TRUE(after[1][1]= 10.0f);
		CHECK_TRUE(after[2][2]= 15.0f);
		CHECK_TRUE(after[3][3]= 1.0f);
	}

	// Change of basis
	// Transform two points to world-space coordinates
	{
		// The two points are at -1, 1, 0 and 1, 1, -1
		// This is almost a line across 0,0,0 in model space
		evector<float, 4> first{-1.0f, 1.0f, 0.0f, 1.0f};
		evector<float, 4> second{-1.0f, 1.0f, -1.0f, 1.0f};
		
		std::vector<evector<float, 4>> points {first, second};

		// But really, we need to place this in a translation matrix
		matrix<4, 4, float> worldTranslation = matrix<4, 4, float>::identity();
		// The actual place they are in world space is 0, 0, -20
		worldTranslation[3][0] = 0.0f;
		worldTranslation[3][1] = 0.0f;
		worldTranslation[3][2] = -20.0f;

		// So essentially we need to translate each point by dotting them  with the world translation
		for (auto&& point : points)
		{
			auto vec = point.transpose() * worldTranslation;
			point[0] = vec[0][0];
			point[1] = vec[0][1];
			point[2] = vec[0][2];
		}

		// Then, translate it to "view" space, which is at 0, 0, 5

		// HOWEVER, we need to invert the translation, which is done by negating the translation vector, so we need to translate by 0, 0, -5 instead of 0, 0, 5
		// This is because we are translating the world, not the camera, so we need to translate in the opposite direction of the camera's movement
		matrix<4, 4, float> camera = matrix<4, 4, float>::identity();
		camera[3][0] = 0.0f;
		camera[3][1] = 0.0f;
		camera[3][2] = -5.0f;

		for (auto&& point : points)
		{
			auto vec = point.transpose() * camera;
			point[0] = vec[0][0];
			point[1] = vec[0][1];
			point[2] = vec[0][2];
		}


		CHECK_TRUE(points[0][0] == -1.0f);
		CHECK_TRUE(points[0][1] == 1.0f);
		CHECK_TRUE(points[0][2] == -25.0f);

		CHECK_TRUE(points[1][0] == -1.0f);
		CHECK_TRUE(points[1][1] == 1.0f);
		CHECK_TRUE(points[1][2] == -26.0f);
	}
}
TEST_SUITE_END