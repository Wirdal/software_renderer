#pragma once

#include "concepts/concepts.h"
#include "evector.h"

// Stored in row-major order
template <size_t Rows, size_t Cols, Floating T>
class matrix
{
public:
	evector<T, Rows>& operator[] (size_t index)
	{
		return m_data[index];
	}

	const evector<T, Rows>& operator[] (size_t index) const
	{
		return m_data[index];
	}
private:
	evector<T, Rows> m_data[Cols];
};
