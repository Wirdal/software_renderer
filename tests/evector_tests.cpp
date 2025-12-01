#include "test_framework.h"
#include "math/evector.h"

TEST_SUITE(evector_tests)
{
	// Uniform scale test
	{
		evector<float, 3> vec;
		vec[0] = 1.0f; vec[1] = 2.0f; vec[2] = 3.0f;

		vec = vec * 2.0f;

		CHECK_TRUE(vec[0] == 2.0f);
		CHECK_TRUE(vec[1] == 4.0f);
		CHECK_TRUE(vec[2] == 6.0f);
	}

	// Vector addition test
	{
		evector<float, 2> vec1;
		vec1[0] = 1; vec1[1] = 2;

		evector<float, 2> vec2;
		vec2[0] = 3; vec2[1] = 4;

		vec1 = vec1 + vec2;

		CHECK_TRUE(vec1[0] == 4);
		CHECK_TRUE(vec1[1] == 6);
	}

	// Vector subraction test
	{
		evector<float, 2> vec1;
		vec1[0] = 5; vec1[1] = 7;

		evector<float, 2> vec2;
		vec2[0] = 3; vec2[1] = 8;

		vec1 = vec1 - vec2;

		CHECK_TRUE(vec1[0] == 2);
		CHECK_TRUE(vec1[1] == -1);
	}

	// Magnitude test
	{
		evector<double, 2> vec1;
		vec1[0] = 3.0; vec1[1] = 4.0;

		// we will need to check for an epsilon here in the future
		CHECK_TRUE(vec1.magnitude() == 5.0); // sqrt(3^2 + 4^2) = 5
	}

	// Squared magnitude test
	{
		evector<float, 3> vec1;
		vec1[0] = 2; vec1[1] = 3; vec1[2] = 6;

		int squared_magnitude = vec1.squared_mag();
		CHECK_TRUE(squared_magnitude == 49); // 2^2 + 3^2 + 6^2 = 49
	}

	// normalization test
	{
		evector<float, 3> vec1;
		vec1[0] = 5.0f; vec1[1] = 0.0f; vec1[2] = 0.0f;

		evector<float, 3> normalized_vec = vec1.normalize();
		CHECK_TRUE(normalized_vec[0] == 1.0f);
		CHECK_TRUE(normalized_vec[1] == 0.0f);
		CHECK_TRUE(normalized_vec[2] == 0.0f);
	}

	// Dot product test
	{
		evector<float, 3> vec1;
		vec1[0] = 1.0f; vec1[1] = 2.0f; vec1[2] = 3.0f;

		evector<float, 3> vec2;
		vec2[0] = 4.0f; vec2[1] = 5.0f; vec2[2] = 6.0f;

		float dot_product = vec1.dot(vec2);
		CHECK_TRUE(dot_product == 32.0f); // 1*4 + 2*5 + 3*6 = 32
	}

	// Collinearity test
	{
		evector<float, 3> vec1;
		vec1[0] = 1.0f; vec1[1] = 2.0f; vec1[2] = 3.0f;

		evector<float, 3> vec2;
		vec2[0] = 2.0f; vec2[1] = 4.0f; vec2[2] = 6.0f;

		CHECK_TRUE(vec1.collinear(vec2)); // vec2 is just vec1 scaled by 2
	}

	// Anti-collinearity test
	{
		evector<float, 3> vec1;
		vec1[0] = 1.0f; vec1[1] = 2.0f; vec1[2] = 3.0f;

		evector<float, 3> vec2;
		vec2[0] = -1.0f; vec2[1] = -2.0f; vec2[2] = -3.0f;

		CHECK_TRUE(vec1.anti_collinear(vec2)); // vec2 is vec1 scaled by -1
	}

	// Cross product test
	{
		evector<float, 3> vec1;
		vec1[0] = 1.0f; vec1[1] = 0.0f; vec1[2] = 0.0f;

		evector<float, 3> vec2;
		vec2[0] = 0.0f; vec2[1] = 1.0f; vec2[2] = 0.0f;

		evector<float, 3> cross_product = vec1.cross(vec2);
		CHECK_TRUE(cross_product[0] == 0.0f);
		CHECK_TRUE(cross_product[1] == 0.0f);
		CHECK_TRUE(cross_product[2] == 1.0f); // i x j = k
	}

	// Lerp test
	{
		evector<float, 3> vec1;
		vec1[0] = 0.0f; vec1[1] = 0.0f; vec1[2] = 0.0f;

		evector<float, 3> vec2;
		vec2[0] = 10.0f; vec2[1] = 10.0f; vec2[2] = 10.0f;

		float beta = 0.5f;
		evector<float, 3> lerped_vec = vec1.lerp(vec2, beta);
		CHECK_TRUE(lerped_vec[0] == 5.0f);
		CHECK_TRUE(lerped_vec[1] == 5.0f);
		CHECK_TRUE(lerped_vec[2] == 5.0f); // halfway between vec1 and vec2
	}
}
TEST_SUITE_END