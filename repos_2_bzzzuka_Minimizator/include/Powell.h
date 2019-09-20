#include <fstream>
#include <complex>
#include "Math.h"

using namespace std;

	ld brent(const ld ax, const ld bx, const ld cx, ld f(const ld),
             const ld tol, ld &xmin);
 
	ld f1dim(const ld x);
 
  
    void linmin(Vector &p, Vector &xi, ld &fret, ld func(Vector &));
  
    void mnbrak(ld &ax, ld &bx, ld &cx, ld &fa, ld &fb, ld &fc,
		ld func(const ld));
  
    
    void powell(Vector &p, ld **xi, const ld ftol, int &iter,
                ld &fret, ld func(Vector &));