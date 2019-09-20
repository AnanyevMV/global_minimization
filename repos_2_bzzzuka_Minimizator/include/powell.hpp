#pragma once

#include "math.hpp"
#include <cmath>  
#include <math.h>
#include <limits>


std::pair<Vector, Vector> linmin(Vector &p, Vector &xi, Function f);

ld f1dim(const ld x, int ncom, Vector *pcom_p, Vector *xicom_p, Function f);

//void powell1(Vector &p, Matrix &xi, const double ftol, int &iter,
//	double &fret, double func(Vector &));

std::tuple<Vector, int, ld> powell1(Vector &p, ld **xi, const ld ftol, 
	ld &fret, Function f);

std::pair<ld, ld> SWAP(ld &a, ld &b);

std::pair<Vector, int> powell(Function f, Vector start_point, int iter_limit = 100);