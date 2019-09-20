#pragma once
#include <cstdint>
#include <vector>
#include <cmath>
#include <iostream>

typedef long double Real;

template<typename T> struct Point
{
    uint32_t N;
    std::vector<T> coordinate;

    Point();

    Point(uint32_t N_);

    Point(uint32_t N_, const std::vector<T>& coordinate_);
};

template <typename T>
Point<T>::Point()
{
    N = 1;
	coordinate.resize(N);
}

template <typename T>
Point<T>::Point(uint32_t N_)
{
    N = N_;
	coordinate.resize(N_);
}

template <typename T>
Point<T>::Point(uint32_t N_, const std::vector<T>& coordinate_)
{
    N = N_;
    coordinate = coordinate_;
}


typedef Point<Real> PointReal;

struct NetPoint {
    uint32_t point;
    double value;
};

class TMSNet
{
public:
    // Генерация одной точки
    PointReal GeneratePoint()
    {
        return PointReal();
    };
};
