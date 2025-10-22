#pragma once
#include "vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

class TestVector {
public:
	// Static method to run all tests
	static void runTests();
	static void testScalarMultiplicationOperator();
	static void testOperatorSum();
	static void testOperatorSumEqual();
	static void testOperatorSubtraction();

	static void testNorm();
	static void testNormSquared();
	static void testDotProduct();
	static void testCrossProduct();
	static void testComponentProduct();
	static void testNormalization();
};
