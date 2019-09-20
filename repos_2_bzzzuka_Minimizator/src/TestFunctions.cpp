#include <iostream>
#include <iomanip>
#include <cmath>
#include "Powell.h"

#define N_max 100
#define M_E 2.71828182845904523536
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

ld f26(Vector& x) {
	return 5*pow(x[0], 2) + 4*x[0]*x[1] + 8*pow(x[1], 2) + 8*x[0] + 14*x[1] + 5;
}

ld f25(Vector& x) {
	return std::pow(x[0], 4) + std::pow(x[1], 4) + std::pow(x[2], 4) - 5.0*(std::pow(x[0], 2) + std::pow(x[1], 2) + std::pow(x[2], 2)) + 11.8;
}

ld f24(Vector& x) {
	return abs(abs(SQR(x[0]) + SQR(x[1])) - 1);
}

ld f23(Vector& x) {
	return abs(abs(SQR(x[0]) + SQR(x[1])) - 1);
}

ld f1(Vector& x) {
	return 1 + x[0] + x[1] - x[0] * x[1] + x[0] * x[0] + x[1] * x[1];
}

ld f2(Vector& x) {
	return 1 + 7 * x[0] + 5 * x[1] + 0.5 * x[0] * x[1] + 3 * x[0] * x[0] + x[1] * x[1];
}

ld f3(Vector& x) {
	return 100 + 7 * x[0] + 5 * x[1] - 10 * x[0] * x[1] + 3 * x[0] * x[0] + 10 * x[1] * x[1];
}

ld f4(Vector& x) {
	return 100 + 7 * x[0] + 5 * x[1] - 10.95 * x[0] * x[1] + 3 * x[0] * x[0] + 10 * x[1] * x[1];
}

ld f5(Vector& x) {
	return 1 + x[0] + x[1] + x[2] + x[0] * x[1] + x[0] * x[2] + x[1] * x[2] + x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
}

ld f6(Vector& x) {
	return 10 * std::pow(x[0], 4) + 15 * std::pow(x[1], 4) + 15 * x[0] * x[1];
}

ld f7(Vector& v) {
	ld x = v[0];
	ld y = v[1];
	return 10 * std::pow(x, 6) + 15 * std::pow(y, 6) - 20 * (std::pow(x, 3) * y + x * std::pow(y, 3));
}

ld f8(Vector& v) {
	auto x = v[0], y = v[1];
	return std::pow(x, 6) + std::pow(y, 6) - 2 * (std::pow(x, 3) * y + x * std::pow(y, 3)) + x * x + y * y;
}

ld f9(Vector& v) {
	auto x = v[0], y = v[1];
	return std::pow(x, 6) + std::pow(y, 6) - 3 * (std::pow(x, 3) * y + x * std::pow(y, 3)) + x * x + y * y;
}

ld f10(Vector& v) {
	auto x = v[0], y = v[1];
	return std::pow(x, 6) + std::pow(y, 6) - 2 * (std::pow(x, 3) * y + x * std::pow(y, 3)) + std::pow(x, 4) + std::pow(y, 4) - x * x - y * y;
}

ld f11(Vector& v) {
	ld fun = std::pow(v[0] - 1, 2) / 4;
	for (unsigned int i = 1; i < v.size(); ++i) {
		fun += std::pow(v[i] - 2 * std::pow(v[i - 1], 2) + 1, 2);
	}
	return fun;
}

ld f12(Vector &v) {
	ld fun = std::pow(v[0] - 1, 2) / 4;
	for (unsigned int i = 1; i < v.size(); ++i) {
		fun += std::abs(v[i] - 2 * std::pow(v[i - 1], 2) + 1);
	}
	return fun;
}

ld f13(Vector &v) {
	ld fun = std::abs(v[0] - 1) / 4;
	for (unsigned int i = 1; i < v.size(); ++i) {
		fun += std::abs(v[i] - 2 * std::pow(v[i - 1], 2) + 1);
	}
	return fun;
}

ld f14(Vector &v) {
	ld fun = 0;
	for (unsigned int i = 1; i < v.size(); i += 2) {
		fun += 100 * std::pow(std::pow(v[i - 1], 2) - v[i], 2) + std::pow(v[i - 1] - 1, 2);
	}
	return fun;
}

ld f15(Vector &v) {
	ld fun = 0;
	for (unsigned int i = 1; i < v.size(); i += 2) {
		fun += 10 * std::abs(std::pow(v[i - 1], 2) - v[i]) + std::abs(v[i - 1] - 1);
	}
	return fun;
}

ld f16(Vector &v) {
	ld fun = 0;
	for (unsigned int i = 0; i < v.size(); ++i) {
		fun += std::pow(v[i], 2);
	}
	return fun;
}

ld f17(Vector &v) {
	ld fun = 10 * v.size();
	for (unsigned int i = 0; i < v.size(); ++i) {
		fun += std::pow(v[i] - 10 * std::cos(2 * M_PI * v[i]), 2);
	}
	return fun;
}

ld f18(Vector &v) {
	return std::pow(std::pow(v[0], 2) + v[1] - 11, 2) + std::pow(std::pow(v[1], 2) + v[0] - 7, 2);
}

ld f19(Vector &v) {
	const int b[] = { 8, 18, 44, 144 };
	ld fun = 0;
	ld g_fun;
	for (int i = 0; i < 4; ++i) {
		g_fun = 0;
		for (int j = 0; j < 4; ++j) {
			g_fun += 1;
			g_fun *= v[i];
		}
		g_fun -= b[i];
		fun += std::pow(g_fun, 2);
	}
	return fun;
}

ld f20(Vector &v) {
	return std::pow(v[1] - 5.1 / (4 * M_PI_2) * std::pow(v[0], 2) + 5 * v[0] / M_PI - 6, 2) +
		10 * (1 - 1 / (8 * M_PI)) * std::cos(v[0]) + 10;
}

ld f21(Vector &v) {
	return std::sin(v[0] + v[1]) + std::pow(v[0] - v[1], 2) - 1.5 * v[0] + 2.5 * v[1] + 1;
}

ld f22(Vector &v) {
	return 0.26 * (std::pow(v[0], 2) + std::pow(v[1], 2)) - 0.48 * v[0] * v[1];
}