#pragma once

#include <type_traits>

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept Floating = std::is_floating_point_v<T>;

template <size_t D>
concept ThreeDimensional = (D == 3);

template <size_t D>
concept ValidDimensions = ((D == 3) || (D == 2));