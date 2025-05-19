#include "MathFunctions.h"

double sigmoid(const double x) {
	return 1 / (1 + exp(-x));
}

double sigmoid_derivative(const double x) {
	return x * (1 - x);
}


double reLu(const double x) {
	return x >= 0 ? x : 0;
}

double reLu_derivative(const double x) {
	return x <= 0 ? 0 : 1;
}


double tanH(const double x) {
	return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}
double tanH_derivative(const double x) {
	return 1 - (x * x);
}


double binary(const double x) {
	return x >= 0 ? 1 : 0;
}
double binary_derivative(const double x) {
	return 0;
}