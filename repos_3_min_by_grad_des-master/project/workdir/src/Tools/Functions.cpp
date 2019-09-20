#include <cmath>

#include "Functions.h"

//test func
Real f1(const Vector& x) {
  return 1 + x[0] + x[1] - x[0] * x[1] + x[0] * x[0] + x[1] * x[1];
}

Real f2(const Vector& x) {
  return 1 + 7 * x[0] + 5 * x[1] + 0.5 * x[0] * x[1] + 3 * x[0] * x[0] + x[1] * x[1];
}

Real f3(const Vector& x) {
  return 100 + 7 * x[0] + 5 * x[1] - 10 * x[0] * x[1] + 3 * x[0] * x[0] + 10 * x[1] * x[1];
}

Real f4(const Vector& x) {
  return 100 + 7 * x[0] + 5 * x[1] - 10.95 * x[0] * x[1] + 3 * x[0] * x[0] + 10 * x[1] * x[1];
}

Real f5(const Vector& x) {
  return 1 + x[0] + x[1] + x[2] + x[0] * x[1] + x[0] * x[2] + x[1] * x[2] + x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
}

Real f6(const Vector& x) {
  return 10 * pow(x[0], 4) + 15 * pow(x[1], 4) + 15 * x[0] * x[1];
}

Real f7(const Vector& v) {
  Real x = v[0];
  Real y = v[1];
  return 10 * pow(x, 6) + 15 * pow(y, 6) - 20 * (pow(x, 3) * y + x * pow(y, 3));
}

Real f8(const Vector& v) {
  auto x = v[0], y = v[1];
  return pow(x, 6) + pow(y, 6) - 2 * (pow(x, 3) * y + x * pow(y, 3)) + x * x + y * y;
}

Real f9(const Vector& v) {
  auto x = v[0], y = v[1];
  return pow(x, 6) + pow(y, 6) - 3 * (pow(x, 3) * y + x * pow(y, 3)) + x * x + y * y;
}

Real f10(const Vector& v) {
  auto x = v[0], y = v[1];
  return pow(x, 6) + pow(y, 6) - 2 * (pow(x, 3) * y + x * pow(y, 3)) + pow(x, 4) + pow(y, 4) - x * x - y * y;
}

Real f11(const Vector &v) {
  Real x = v[0], y = v[1];
  return (x*x + y - 11)*(x*x + y - 11) + (y*y + x - 7)*(y*y + x - 7);
}

Real f12(const Vector &v) {
  // (x1+x2+x3+x4-8)^2+(x1^2+x2^2+x3^2+x4^2-18)^2+(x1^3+x2^3+x3^3+x4^3-44)^2+(x1^4+x2^4+x3^4+x4^4-114)^2
  assert(v.size() == 4u);

  Vector b = { 8, 18, 44, 114 };
  Real fun = 0;

  for (int i = 0; i < 4; ++i) {
    Real temp = -b[i];

    for (auto x : v) {
      temp += pow(x, i + 1);
    }

    fun += temp * temp;
  }

  return fun;
}

Real f13(const Vector &v) {
  assert(v.size() == 2u);
  Real x = v[0], y = v[1];
  return pow(y - 5.1 / (4 * M_PI*M_PI)*x*x + 5 / M_PI * x - 6, 2) + 10 * (1 - 1 / (8 * M_PI))*cos(x) + 10;
}

Real f14(const Vector &v) {
  return std::sin(v[0] + v[1]) + pow(v[0] - v[1], 2) + 1.5 * v[0] * v[0] + 2.5 * v[1] * v[1] + 1;
}

Real f15(const Vector &v) {
  return 0.26 * (pow(v[0], 2) + pow(v[1], 2)) - 0.48 * v[0] * v[1];
}

Real f16(const Vector &v) {
  return norm(v) < 1e-16 ? 0 : -(pow(std::sin(v[0] - v[1]), 2) * pow(std::sin(v[0] + v[1]), 2)) / norm(v);
}

Real f17(const Vector &v) {
  Real sum_sqr = 0, sum_sin = 0;

  for (auto x : v) {
    sum_sqr += x * x;
    sum_sin += sin(x)*sin(x);
  }

  return sum_sin - exp(-sum_sqr);
}

Real f18(const Vector &v) {
  return v[0] / 4 + pow(v[0] * v[0] - 2 * v[0] + v[1] * v[1], 2);
}

Real f19(const Vector &v) {
  Real x = v[0], y = v[1];
  return (1.5 - x * (1 - y))*(1.5 - x * (1 - y)) + (2.25 - x * (1 - y * y))*(2.25 - x * (1 - y * y)) + (2.625 - x * (1 - y * y*y))*(2.625 - x * (1 - y * y*y));
}

Real f20(const Vector &v) {
  Real x1 = v[0], x2 = v[1], x3 = v[2], x4 = v[3];
  return 100 * (x2 - x1 * x1)*(x2 - x1 * x1) + (1 - x1)*(1 - x1) + 90 * (x4 - x3 * x3)*(x4 - x3 * x3) + (1 - x3)*(1 - x3) + 10.1*(x2 - 1)*(x2 - 1) + 10.1*(x4 - 1)*(x4 - 1);
}

Real f21(const Vector &v) {
  return pow(v[0] * v[0] - v[1], 2) + pow(1 - v[0], 2);
}

Real f22(const Vector &v) {
  return pow(v[0] * v[0] - v[1], 2) + 100 * pow(1 - v[0], 2);
}

Real f23(const Vector &v) {
  return 100 * pow(v[1] - pow(v[0], 3), 2) + pow(1 - v[0], 2);
}

Real f24(const Vector &v) {
  Real x1 = v[0], x2 = v[1], x3 = v[2], x4 = v[3];
  return pow((x1 + 10 * x2), 2) + 5 * pow((x4 - x3), 2) + pow((x2 - 2 * x3), 4) + pow(10 * (x1 - x4), 4);
}
Real f25(const Vector &v) {
  Real x1 = v[0], x2 = v[1], x3 = v[2], x4 = v[3];
  
  return pow(x1*x1 - x2 + 1, 4) + 100 * pow(x2 - x3, 6) +
    pow(std::tan(x3 - x4), 4) + pow(x1, 8) + pow(x4 - 1, 2);
}

Real f26(const Vector &v) {
  Real fun = 0;
  
  for (unsigned int i = 0; i < v.size(); ++i) {
    fun += pow(v[i], 2);
  }

  return fun;
}

Real f27(const Vector& v) {
  Real fun = pow(v[0] - 1, 2) / 4;
  
  for (unsigned int i = 1; i < v.size(); ++i) {
    fun += pow(v[i] - 2 * pow(v[i - 1], 2) + 1, 2);
  }

  return fun;
}

Real f28(const Vector &v) {
  Real fun = 0;

  for (unsigned int i = 1; i < v.size(); i += 2) {
    fun += 100 * pow(pow(v[i - 1], 2) - v[i], 2) + pow(v[i - 1] - 1, 2);
  }

  return fun;
}

Real f29(const Vector &v) {
  Real fun = 0;

  for (int i = 0; i < (int)v.size() - 1; i++) {
    fun += v[i] * v[i] + 2 * v[i + 1] * v[i + 1] - 0.3 * std::cos(3 * M_PI * v[i]) -
      0.4 * std::cos(4 * M_PI * v[i + 1]) + 0.7;
  }

  return fun;
}

Real f30(const Vector &v) {
  Real fun = 0;

  for (unsigned int i = 1; i < v.size(); i++) {
    fun += v[i] * v[i];
  }

  fun *= 1000000;
  fun += v[0] * v[0];

  return fun;
}

Real f31(const Vector &x) {
  return (1 + pow((x[0] + x[1] + 1), 2)*(19 - 14 * x[0] + 3 * x[0] * x[0] - 14 * x[1] + 6 * x[0] * x[1] + 3 * x[1] * x[1]))*
  (30 + pow((2 * x[0] - 3 * x[1]), 2)*(18 - 32 * x[0] + 12 * x[0] * x[0] + 48 * x[1] - 36 * x[0] * x[1] + 27 * x[1] * x[1]));
}

Real f32(const Vector &x) {
  return (100 * sqrt(abs(x[1] - 0.01*x[0] * x[0])) + 0.01*abs(x[0] + 10));
}

Real f33(const Vector &v) {
  Real fun = 0, fun1 = 0, fun2 = 0;

  for (unsigned int i = 0; i < v.size(); i++) {
   fun1 = fun1 + std::cos(5 * M_PI*v[i]);
   fun2 = fun2 + v[i] * v[i];
  }

  fun = (-0.1)*fun1 + fun2;
  
  return fun;
}

Real f34(const Vector &x) {
  return (100 * pow(x[1] - 0.01*x[0] * x[0] + 1, 2) + 0.01*(x[0] + 10)*(x[0] + 10));
}

Real f35(const Vector& v) {
  assert(v.size() == 2u);
  return abs(v[0] * v[0] + v[1] * v[1] + v[0] * v[1]) + abs(sin(v[0])) + abs(sin(v[1]));
}

Real f36(const Vector &x) {
  return pow((2 * pow(x[0], 3)*x[1] - pow(x[1], 3)), 2) + pow((6 * x[0] - x[1] * x[1] + x[1]), 2);
}

// ThreeHumpCamel
Real f37(const Vector &v) {
  return (2 * pow(v[0], 2) - 1.05 * pow(v[0], 4) + pow(v[0], 6) / 6 + v[0] * v[1] + pow(v[1], 2));
}

// SixHumpCamel
Real f38(const Vector &v) {
  return (4 - 2.1*pow(v[0], 2) + pow(v[0], 4) / 3)*pow(v[0], 2) + v[0] * v[1] + (-4 + 4 * pow(v[1], 2))*pow(v[1], 2);
}

// Branin01
Real f39(const Vector &v) {
  return pow((-1.275*pow(v[0], 2) / pow(M_PI, 2) + 5 * v[0] / M_PI + v[1] - 6), 2) + (10 - 5 / (4 * M_PI))*cos(v[0]) + 10;
}

// Branin02
Real f40(const Vector &v) {
  return pow((-1.275*pow(v[0], 2) / pow(M_PI, 2) + 5 * v[0] / M_PI + v[1] - 6), 2) + (10 - 5 / (4 * M_PI))*std::cos(v[0])*std::cos(v[1]) + std::log(pow(v[0], 2) + pow(v[1], 2) + 1) + 10;
}

// RotatedEllipse01 7x*x-6*sqrt(3)*x*y+13y*y
Real f41(const Vector &v) {
  Real x = v[0], y = v[1];
  return 7 * x*x - 6 * std::sqrt(3)*x*y + 13 * y*y;
}

// EggCrate
Real f42(const Vector &v) {
  Real x = v[0], y = v[1];
  Real sin_x = sin(x), sin_y = sin(y);
  return x * x + y * y + 25 * (sin_x*sin_x + sin_y * sin_y);
}

// RotatedEllipse02 x*x-x*y+y*y
Real f43(const Vector& v) {
  //-x1*x2(72-2x1-2x2)
  assert(v.size() == 2u);
  Real x = v[0], y = v[1];
  return x * x - x * y + y * y;
}

// Bird (x-y)^2+exp((1-sin(x))^2)cos(y)+exp((1-cos(y))^2)*sin(x)
Real f44(const Vector& v) {
  assert(v.size() == 2u);
  Real x = v[0], y = v[1];
  return (x - y)*(x - y) + exp(pow(1 - sin(x), 2))*cos(y) + exp(pow(1 - cos(y), 2))*sin(x);
}

// Hosaki (1-8x+7x^2-7.0/3*x^3+1.0/4*x^4)*y^2*exp(-y)
Real f45(const Vector& v) {
  assert(v.size() == 2u);
  Real x = v[0], y = v[1];
  return (1 - 8 * x + 7 * x*x - 7.0 / 3 * x*x*x + 1.0 / 4 * x*x*x*x)*y*y*std::exp(-y * y);
}

// El-Attar-Vidyasagar-Dutta
Real f46(const Vector &x) {
  return pow((x[0] * x[0] + x[1] - 10), 2) + pow((x[0] + x[1] * x[1] - 7), 2) + pow((x[0] * x[0] + pow(x[1], 3) - 1), 2);
}

//Ursem01
Real f47(const Vector &x) {
  return -sin(2 * x[0] - 0.5*M_PI) - 3 * cos(x[1]) + 0.5*x[0] * x[0];
}

//Alpine 1
Real f48(const Vector &v) {
  Real fun = 0;

  for (unsigned int i = 0; i < v.size(); i++) {
    fun = fun + std::abs(v[i] * sin(v[i]) + 0.1*v[i]);
  }

  return fun;
}

// Levy13
Real f49(const Vector &v) {
  return pow((v[0] - 1), 2)*(pow(std::sin(3 * M_PI*v[1]), 2) + 1) + pow((v[1] - 1), 2)*(pow(std::sin(2 * M_PI*v[1]), 2) + 1) + pow(std::sin(3 * M_PI*v[0]), 2);
}

// Mishra08
Real f50(const Vector &v) {
  return 0.001*pow((std::abs(pow(v[0], 10) - 20 * pow(v[0], 9) + 180 * pow(v[0], 8) - 960 * pow(v[0], 7) + 3360 * pow(v[0], 6) - 8064 * pow(v[0], 5) + 13340 * pow(v[0], 4) - 15360 * pow(v[0], 3) + 11520 * pow(v[0], 2) - 5120 * v[0] + 2624)*std::abs(pow(v[1], 4) + 12 * pow(v[1], 3) + 54 * pow(v[1], 2) + 108 * v[1] + 81)), 2);
}
