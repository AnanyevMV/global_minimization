#pragma once

#include "Data.h"

// Вектор-градиент
Vector grad(Function f, const Vector& point, const Real h);
std::ostream& operator<<(std::ostream& os, const Vector& v);

// Перегрузка векторных операций
Vector& operator*=(Vector& v, const Real value);

Vector operator*(const Real value, Vector v);

Vector operator*(Vector v, const Real value);

Vector operator-(Vector v);

Vector& operator+=(Vector & v1, const Vector& v2);

Vector& operator-=(Vector & v1, const Vector& v2);

Vector operator+(Vector v1, const Vector& v2);

Vector operator-(Vector v1, const Vector& v2);

// Покомпонентное умножение векторов
Vector operator*(Vector v1, const Vector& v2);

// Проверка ненулевого вектора
bool is_zero(const Vector& v);

// Скалярное произведение
Real dot(const Vector& v1, const Vector& v2);

// Норма
Real norm(const Vector& v);

// Покомпонентное деление вектора на вектор
Vector operator/(Vector v1, const Vector& v2);

// Покомпонентный квадратный корень
Vector sqrt(Vector v1);

// Функция, сглаживающая нулевой вектор
Vector notNull(Vector v1);
