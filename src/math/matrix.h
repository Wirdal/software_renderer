#pragma once

#include "concepts/concepts.h"
#include "evector.h"

// Stored in row-major order
template <size_t Rows, size_t Cols, Floating T>
class EMatrix
{
public:
	evector<T, Cols>& operator[] (size_t index)
	{
		return m_data[index];
	}

	const evector<T, Cols>& operator[] (size_t index) const
	{
		return m_data[index];
	}
private:
	evector<T, Rows> m_data[Cols];
};
