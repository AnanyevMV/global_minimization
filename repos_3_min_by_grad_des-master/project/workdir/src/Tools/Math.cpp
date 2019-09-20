#include <cmath>

#include "Math.h"

// Вектор-градиент
Vector grad(Function f, const Vector& point, const Real h) {

  int n = (int)point.size();
  Vector right = point;
  Vector left = point;
  Vector answer(n);

  for (int i = 0; i < n; ++i) {
    right[i] += h;
    left[i] -= h;
    answer[i] = (f(right) - f(left)) / (2 * h);
    right[i] -= h;
    left[i] += h;
  }

  return answer;
}

// Перегрузка векторных операторов
std::ostream& operator<<(std::ostream& os, const Vector& v) {
  for (auto& it : v) {
    os << std::setprecision(8) << std::fixed << std::setw(16) << it;
  }

  return os;
}

Vector& operator*=(Vector& v, const Real value) {
  for (auto & it : v) {
    it *= value;
  }

  return v;
}

Vector operator*(const Real value, Vector v) {
  return v *= value;
}

Vector operator*(Vector v, const Real value) {
  return v *= value;
}

Vector operator-(Vector v) {
  return v *= -1;
}

Vector& operator+=(Vector & v1, const Vector& v2) {
  assert(v1.size() == v2.size());

  for (int i = 0; i < (int)v1.size(); ++i) {
    v1[i] += v2[i];
  }

  return v1;
}

Vector& operator-=(Vector & v1, const Vector& v2) {
  return v1 += -v2;
}

Vector operator+(Vector v1, const Vector& v2) {
  return v1 += v2;
}

Vector operator-(Vector v1, const Vector& v2) {
  return v1 -= v2;
}

// Покомпонентное умножение
Vector operator*(Vector v1, const Vector& v2) {
  assert(v1.size() == v2.size());

  for (int i = 0; i < (int)v1.size(); ++i) {
    v1[i] = v1[i] * v2[i];
  }

  return v1;
}

// Проверка нулевого вектора
bool is_zero(const Vector& v) {
  for (const auto & it : v) {
    if (std::abs(it) > COMPARE_EPS) {
      return false;
    }
  }

  return true;
}

// Cкалярное произведение
Real dot(const Vector& v1, const Vector& v2) {
  assert(v1.size() == v2.size());
  Real sum = 0;

  for (int i = 0; i < (int)v1.size(); ++i) {
    sum += v1[i] * v2[i];
  }

  return sum;
}

// Норма
Real norm(const Vector& v) {
  return std::sqrt(dot(v, v));
}

// Покомпонентное деление вектора на вектор
Vector operator/(Vector v1, const Vector& v2) {
  assert(v1.size() == v2.size());

  for (int i = 0; i < (int)v1.size(); ++i) {
    v1[i] = v1[i] / v2[i];
  }

  return v1;
}

// Покомпонентный квадратный корень
Vector sqrt(Vector v1) {
  for (int i = 0; i < (int)v1.size(); ++i) {
    v1[i] = sqrt(v1[i]);
  }

  return v1;
}

// Сглаживание нулевого вектора
Vector notNull(Vector v1) {

  for (int i = 0; i < (int)v1.size(); ++i) {
    if (v1[i] == 0)
      v1[i] = v1[i] + COMPARE_EPS;
  }

  return v1;
}
