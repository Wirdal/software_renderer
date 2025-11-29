#pragma once
#include <iostream>

#define CHECK_TRUE(condition) \
	do { \
		total++; \
		if (!(condition)) { \
			std::cerr << "Assertion failed: " << #condition << ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
		} else { success++; } \
	} while (0)

#define CHECK_FALSE(condition) \
	do { \
		total++; \
		if (condition) { \
			std::cerr << "Assertion failed: " << #condition << ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
		} else { success++; } \
	} while (0)

#define TEST_SUITE(name) \
	{ \
		int success = 0; \
		int total = 0; \
		std::cout << "Running test suite: " << name << std::endl;

#define TEST_SUITE_END \
		std::cout << "Test suite results: " << success << "/" << total << " passed" << std::endl; \
	}