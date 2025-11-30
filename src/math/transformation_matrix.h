#pragma once

#include "concepts/concepts.h"
#include "matrix.h"


// All defined in 4x4 matrices for 3D transformations
template <Floating T>
using transformation_matrix = matrix<4, 4, T>;

template <Floating T>
using scaling_matrix = transformation_matrix<T>;

template <Floating T>
using rotation_matrix = transformation_matrix<T>;

template <Floating T>
using translation_matrix = transformation_matrix<T>;