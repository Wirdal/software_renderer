#pragma once

#include <stddef.h>
#include <cmath>
#include "concepts/concepts.h"

template <size_t Rows, size_t Cols, Floating T>
class matrix;

/*
Most game programmers use "vector" to refer to points as well as vectors in the algebraic sense

Points operate differently, semantically, when added or subtracte1 to vectors/points
direction + direction = direction
direction - direction = direction
point + direction = point
point - point = direction
point + point = _nonsense_
Not sure how I can represent this in C++ without re-writing the entire thing
*/


// Conventionally	2d = x,y
// 					3d = x,y,z
template <Floating T = float, size_t Dimensions = 3>
struct evector // evector for euclidean vector
{
	template <size_t Rows, size_t Cols, Floating MT>
	friend class matrix;
	// Epsilon is one in one hundredth of the smallest representable value
	static constexpr T epsilon { std::numeric_limits<T>::epsilon() * T{100} };

	static constexpr bool approximately_equal(T a, T b)
	{
		return (epsilon >= std::fabs(a - b) ||  std::fabs(a - b) <= epsilon);
	}

	// Modifies input vector
	friend constexpr evector& operator*(evector& vec, T scalar)
	{
		for (T& ite : vec.m_data)
		{
			ite = ite * scalar;
		}
		return vec;
	}

	friend constexpr evector& operator*(T scalar, evector& vec)
	{
		return vec * scalar;
	}

	friend constexpr evector& operator+(evector& vec, const evector& other)
	{
		for (size_t i = 0; i < Dimensions; ++i)
		{
			vec.m_data[i] = vec.m_data[i] + other[i];
		}
		return vec;
	}

	friend constexpr evector& operator-(evector& vec, const evector& other)
	{
		for (size_t i = 0; i < Dimensions; ++i)
		{
			vec.m_data[i] = vec.m_data[i] - other[i];
		}
		return vec;
	}

	// Returns new vector
	friend constexpr evector operator*(const evector& vec, T scalar)
	{
		evector result = vec;
		for (T& ite : result.m_data)
		{
			ite = ite * scalar;
		}
		return result;
	}

	friend constexpr evector operator*(T scalar, const evector& vec)
	{
		return vec * scalar;
	}

	friend constexpr evector operator+(const evector& vec, const evector& other)
	{
		evector result = vec;
		for (size_t i = 0; i < Dimensions; ++i)
		{
			result.m_data[i] = result.m_data[i] + other[i];
		}
		return result;
	}

	friend constexpr evector operator-(const evector& vec, const evector& other)
	{
		evector result = vec;
		for (size_t i = 0; i < Dimensions; ++i)
		{
			result.m_data[i] = result.m_data[i] - other[i];
		}
		return result;
	}

	constexpr T& operator[] (size_t index)
	{
		// Check if index is out of bounds...
		return m_data[index];
	}

	constexpr const T& operator[] (size_t index) const
	{
		// Check if index is out of bounds...
		return m_data[index];
	}

	// Probably needs to be a float?
	constexpr T magnitude() const
	{
		T total{};
		for (const T ite : m_data)
		{
			total += std::pow(ite, 2);
		}
		return (std::pow(total, (T{1} / T{2})));
	}

	// Only to be used when comparing relative length. Discards all directional data
	constexpr T squared_mag() const
	{
		T total{};
		for (const T ite : m_data)
		{
			total += std::pow(ite, 2);
		}
		return total;
	}

	constexpr evector normalize() const
	{
		return ((*this) * ( (T{1.0}/magnitude()) ));
	}

	// Product of the length of this and other, and the cosine of the angle between them
	// cosine is the ratio of the length of the adjacent leg to that of the hypotenuse
	constexpr T dot(const evector& other) const
	{
		T total{};
		for (size_t i = 0; i < Dimensions; i++)
		{
			total += m_data[i] * other[i];
		}
		return total;
	}

	// This is the length of this projected onto other
	constexpr T project_onto(const evector& other) const
	{
		evector normalized_other = other.normalize();
		T dot_product = dot(normalized_other);
		return normalized_other * dot_product;
	}

	// Dot product tests 

	// The angle between this and the other vector is exactly 0 degrees
	constexpr bool collinear(const evector& other) const
	{
		T this_mag = magnitude();
		T other_mag = other.magnitude();

		T dot_val = dot(other);

		return approximately_equal(dot_val, this_mag * other_mag); // Check for epsilon
	}

	// Collinear but opposite direction
	constexpr bool anti_collinear(const evector& other) const
	{
		T this_mag = magnitude();
		T other_mag = other.magnitude();

		T dot_val = dot(other);

		return approximately_equal(dot_val, -(this_mag * other_mag)); // Check for epsilon
	}

	constexpr bool perpendicular(const evector& other) const
	{
		T dot_val = dot(other);
		return approximately_equal(dot_val, T{0}); // Check for epsilon
	}

	// The angle between them is less than 90 degrees
	constexpr bool same_direction(const evector& other) const
	{
		T dot = dot(other);
		return dot > T{0}; // Check for epsilon?
	}

	// The angle between them is greater than 90 degrees
	constexpr bool opposite_direction(const evector& other) const
	{
		T dot = dot(other);
		return dot < T{0}; // Check for epsilon?
	}

	// Cross product finds the vector perpendicular to both this and other
	constexpr evector cross(const evector& other) const requires ThreeDimensional<Dimensions>
	{
		evector result{};
		result[0] = m_data[1] * other[2] - m_data[2] * other[1];
		result[1] = m_data[2] * other[0] - m_data[0] * other[2];
		result[2] = m_data[0] * other[1] - m_data[1] * other[0];
		return result;
	}

	constexpr evector lerp(const evector& other, T beta) const
	{
		if (beta < T{0} || beta > T{1})
		{
			// Handle error - beta out of range
			return evector{};
		}
		return 	((T{1} - beta) * (*this)) + (beta* other);
	}

	// This return type is not complete, I think this is implementation dependant if this works or not
	// if it does not work, might need to not do header only stuff
	constexpr matrix<1, Dimensions, T> transpose() const
	{
		matrix<1, Dimensions, T> retMat;

		for (size_t ite = 0; ite < Dimensions; ite++)
		{
			retMat[0][ite] = m_data[ite];
		}
		return retMat;
	};
// private:
	T m_data[Dimensions];
};