#pragma once

#include "concepts/concepts.h"
#include "evector.h"

// Stored in row-major order
template <size_t Rows, size_t Cols, Floating T>
class matrix
{
public:
	constexpr evector<T, Rows>& operator[] (size_t index)
	{
		return m_data[index];
	}

	constexpr const evector<T, Rows>& operator[] (size_t index) const
	{
		return m_data[index];
	}

	friend constexpr matrix<Rows, Cols, T> operator*(const matrix<Rows, Cols, T>& lhs, const matrix<Rows, Cols, T>& rhs)
	{
		matrix<Rows, Cols, T> result{};

		for (size_t row = 0; row < Rows; row++)
		{
			for (size_t col = 0; col < Cols; col++)
			{
				// Matrix product is the dot between each row of lhs and each column of rhs
				T dot_prod = lhs[row].dot(rhs[col]);
				result[row][col] = dot_prod;
			}
		}
		return result;
	}


private:
	evector<T, Rows> m_data[Cols];
};
