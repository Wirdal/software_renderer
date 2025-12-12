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

	
	// The "rows" of LHS are multiplied into the "columns" of RHS
	// The output matrix will be of size LHS rows x RHS cols
	// In general, an element of the output matrix is the dot product of row I from LHS and column J from RHS
	friend constexpr matrix<Rows, Cols, T> operator*(const matrix<Rows, Cols, T>& lhs, const matrix<Cols, Rows, T>& rhs)
	{
		matrix<Rows, Cols, T> result{};
		
		// For each elem in result matrix
		for (size_t row = 0; row < Rows; row++)
		{
			for (size_t col = 0; col < Cols; col++)
			{
				T dot_prod{};
				// For each dataum in the inputs
				for (size_t inner = 0; inner < Rows; inner++)
				{
					// Matrix product is the dot between each row of lhs and each column of rhs
					// Since these are both represented in row-major order, we cannot just _dot_ each
					dot_prod += lhs[row][inner] * rhs[inner][col];
				}

				result[row][col] = dot_prod;
			}
		}
		return result;
	}


private:
	evector<T, Rows> m_data[Cols];
};
