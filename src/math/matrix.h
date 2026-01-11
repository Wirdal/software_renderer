#pragma once

#include <algorithm>
#include "concepts/concepts.h"
#include "evector.h"

// Stored in row-major order
template <size_t Rows, size_t Cols, Floating T>
class matrix
{
public:

	matrix() = default;

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
	template<size_t RhsCols>
	friend constexpr matrix<Rows, RhsCols, T> operator*(const matrix<Rows, Cols, T>& lhs, const matrix<Cols, RhsCols, T>& rhs)
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

	static constexpr matrix<Rows, Cols, T> identity() requires Square<Rows, Cols>
	{
		matrix<Rows, Cols, T> retMatrix;
		for (auto& vec : retMatrix.m_data)
		{
			std::fill(std::begin(vec.m_data), std::end(vec.m_data), T{0});
		}
		size_t i = 0;
		while (i < Rows)
		{
			retMatrix.m_data[i][i] = T{1}; // Unsure if this is the best, maybe needs type traits?
			i++;
		}

		return retMatrix;
	}

	// Adds additional dimensions
	template<size_t OutRows, size_t OutCols>
	constexpr matrix<OutRows, OutCols, T> grow() const /* requires Greater<size_t, OutCols, Cols> && Greater<size_t, OutRows, Rows>*/
	{
		matrix<OutRows, OutCols, T> retMatrix;

		// Copy over the data first
		for (size_t row = 0; row < Rows; row++)
		{
			for(size_t col = 0; col < Cols; col++)
			{
				retMatrix[row][col] = m_data[row][col];
			}
		}

		for (size_t row = Rows; row < OutRows; row++)
		{
			for(size_t col = 0; col < OutCols; col++)
			{
				retMatrix[row][col] = T{};
			}
		}

		for (size_t col = Cols; col < OutCols; col++)
		{
			for(size_t row = 0; row < OutRows; row++)
			{
				retMatrix[row][col] = T{};
			}
		}


		return retMatrix;
	}
private:
	evector<T, Rows> m_data[Cols];
};
