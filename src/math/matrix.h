#pragma once

#include "concepts/concepts.h"
#include "evector.h"

template <size_t Rows, size_t Cols, Floating T>
class EMatrix
{
public:
	
private:
	evector<T, Cols> m_data[Rows];
};
