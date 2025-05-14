#include "MathFunctions.h"

double sigmoid(const double x) {
	return 1 / (1 + exp(-x));
}

double sigmoid_derivative(double x) {
	return x * (1 - x);
}