#pragma once
#include <cmath>

/*---------------------------------------------------*/
/* Implemetation of reccuring mathematical functions */
/*---------------------------------------------------*/

/*Still left to implement:
	- reLu(x)		-> f(x) = max(0,x)
	- tanH(x)		-> f(x) = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	- binary(x)		-> f(x) = x >= 0 ? 1 : 0

	- reLu'(x)		-> f'(x) = x <= 0 ? 0 : 1
	- tanH'(x)		-> f'(x) = 1 - tanH(x) * tanH(x)
	- binary'(x)	-> f'(x) = 0
*/

double sigmoid(const double x);
double sigmoid_derivative(double x);