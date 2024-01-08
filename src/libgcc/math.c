#include <math.h>

double modf(double fl, double* integral) {
	double whole = (double)(long long)fl;
	
	if (integral) {
		*integral = whole;
	}

	return fl-whole;
}

double round(double x) {
	double integral = 0.0;
	double frac = modf(x, &integral);

	if (frac >= 0.5) {
		integral += integral > 0 ? 1.0 : -1.0;
	}

	return integral;
}

double floor(double x) {
	double integral = 0.0;
	double frac = modf(x, &integral);
	return integral;
}

#define _EXP_10_E ((double)1.10517091808)
#define _EXP_100_E ((double)1.01005016708)
#define _EXP_1000_E ((double)1.00100050017)
#define _EXP_10000_E ((double)0.00027182818)

double exp(double x) {
	// e^a
	double fa = x;
	unsigned char neg = 0;

	if (x < 0) {
		neg = 1;
		fa *= -1.0;
	}

	double e_a = 1.0;
	double n;

	for (n = 0.0; n + 1.0 <= fa; n++) {
		e_a *= M_E;
	}

	// Calculate e^a
	for (; n + 0.1 <= fa; n += 0.1) {
		e_a *= _EXP_10_E;
	}

	for (; n + 0.01 <= fa; n += 0.01) {
		e_a *= _EXP_100_E;
	}

	for (; n + 0.001 <= fa; n += 0.001) {
		e_a *= _EXP_1000_E;
	}

	for (; n + 0.0001 <= fa; n += 0.0001) {
		e_a *= _EXP_10000_E;
	}

	// Invert if a is negative.
	if (neg) {
		return 1.0 / e_a;
	}

	return e_a;
}


