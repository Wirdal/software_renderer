#pragma once

#include <algorithm>
#include <cmath>
#include "concepts/concepts.h"
#include "evector.h"

// Stored in row-major order
template <size_t Rows, size_t Cols, Floating T>
class matrix
{
public:

	matrix() = default;

	constexpr matrix<1, 4, T> operator()() requires (Rows == 1 && Cols == 3)
	{
		matrix<1, 4, T> retMat;
		for (size_t ite = 0; ite < Cols; ite++)
		{
			retMat[0][ite] = m_data[0][ite];
		}
		return retMat;
	}

	constexpr evector<T, Cols>& operator[] (size_t index)
	{
		return m_data[index];
	}

	constexpr const evector<T, Cols>& operator[] (size_t index) const
	{
		return m_data[index];
	}

	friend constexpr bool operator==(const matrix<Rows, Cols, T>& lhs, const matrix<Rows, Cols, T>& rhs)
	{
		for (size_t row = 0; row < Rows; row++)
		{
			if (lhs.m_data[row] != rhs.m_data[row])
			{
				return false;
			}
		}
		
		return true;
	}
	
	// The "rows" of LHS are multiplied into the "columns" of RHS
	// The output matrix will be of size LHS rows x RHS cols
	// In general, an element of the output matrix is the dot product of row I from LHS and column J from RHS
	template<size_t RhsCols>
	friend constexpr matrix<Rows, RhsCols, T> operator*(const matrix<Rows, Cols, T>& lhs, const matrix<Cols, RhsCols, T>& rhs)
	{
		matrix<Rows, RhsCols, T> result{};
		
		// For each elem in result matrix
		for (size_t row = 0; row < Rows; row++)
		{
			for (size_t col = 0; col < RhsCols; col++)
			{
				T dot_prod{};
				// For each datum in the inputs
				for (size_t inner = 0; inner < Cols; inner++)
				{
					// Matrix product is the dot between each row of lhs and each column of rhs
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

	// 4x4 transform constructors (homogeneous coordinates)
	// translation(T t) - uniform translation applied to x/y/z (stored in last row in this row-vector convention)
	static constexpr matrix<Rows, Cols, T> translation(T t) requires (Rows == 4 && Cols == 4)
	{
		matrix<Rows, Cols, T> ret = matrix<Rows, Cols, T>::identity();
		ret[3][0] = t;
		ret[3][1] = t;
		ret[3][2] = t;
		return ret;
	}

	// Rotation about X axis
	static constexpr matrix<Rows, Cols, T> rotation_x(T theta) requires (Rows == 4 && Cols == 4)
	{
		matrix<Rows, Cols, T> ret = matrix<Rows, Cols, T>::identity();
		ret[1][1] = std::cos(theta);
		ret[1][2] = std::sin(theta);
		ret[2][1] = -std::sin(theta);
		ret[2][2] = std::cos(theta);
		return ret;
	}

	// Rotation about Y axis
	static constexpr matrix<Rows, Cols, T> rotation_y(T theta) requires (Rows == 4 && Cols == 4)
	{
		matrix<Rows, Cols, T> ret = matrix<Rows, Cols, T>::identity();
		ret[0][0] = std::cos(theta);
		ret[0][2] = -std::sin(theta);
		ret[2][0] = std::sin(theta);
		ret[2][2] = std::cos(theta);
		return ret;
	}

	// Rotation about Z axis
	static constexpr matrix<Rows, Cols, T> rotation_z(T theta) requires (Rows == 4 && Cols == 4)
	{
		matrix<Rows, Cols, T> ret = matrix<Rows, Cols, T>::identity();
		ret[0][0] = std::cos(theta);
		ret[0][1] = std::sin(theta);
		ret[1][0] = -std::sin(theta);
		ret[1][1] = std::cos(theta);
		return ret;
	}

	// Uniform scaling
	static constexpr matrix<Rows, Cols, T> scaling(T s) requires (Rows == 4 && Cols == 4)
	{
		matrix<Rows, Cols, T> ret = matrix<Rows, Cols, T>::identity();
		ret[0][0] = s;
		ret[1][1] = s;
		ret[2][2] = s;
		return ret;
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
	evector<T, Cols> m_data[Rows];
};
