#include <iostream>
#include <cmath>
#include "Powell.h"
#include "Math.h"
using namespace std;

void powell(Vector &p, ld **xi, const ld ftol, int &iter,
	ld &fret, ld func(Vector &))
{
	const int ITMAX=200;
	const ld TINY=1.0e-25;
	int i,j,ibig;
	ld del,fp,fptt,t;

	int n=p.size();
	Vector pt(n),ptt(n),xit(n);
	fret=func(p);
	for (j=0;j<n;j++) pt[j]=p[j];
	for (iter=0;;++iter) {
		fp=fret;
		ibig=0;
		del=0.0;
		for (i=0;i<n;i++) {
			for (j=0;j<n;j++) xit[j]=xi[j][i];
			fptt=fret;
			linmin(p,xit,fret,func);
			if (fptt-fret > del) {
				del=fptt-fret;
				ibig=i+1;
			}
		}
		if (2.0*(fp-fret) <= ftol*(fabs(fp)+fabs(fret))+TINY) {
			return;
		}
		for (j=0;j<n;j++) {
			ptt[j]=2.0*p[j]-pt[j];
			xit[j]=p[j]-pt[j];
			pt[j]=p[j];
		}
		fptt=func(ptt);
		if (fptt < fp) {
			t=2.0*(fp-2.0*fret+fptt)*SQR(fp-fret-del)-del*SQR(fp-fptt);
			if (t < 0.0) {
				linmin(p,xit,fret,func);
				for (j=0;j<n;j++) {
					xi[j][ibig-1]=xi[j][n-1];
					xi[j][n-1]=xit[j];
				}
			}
		}
	}
}


int ncom;
ld(*nrfunc)(Vector &);
Vector *pcom_p, *xicom_p;

ld f1dim(const ld x)
{
	int j;

	Vector xt(ncom);
	Vector &pcom = *pcom_p, &xicom = *xicom_p;
	for (j = 0; j<ncom; j++)
		xt[j] = pcom[j] + x * xicom[j];
	return nrfunc(xt);
}

void linmin(Vector &p, Vector &xi, ld &fret, ld func(Vector &))
{
	int j;
	const ld TOL = 1.0e-8;
	ld xx, xmin, fx, fb, fa, bx, ax;

	int n = p.size();
	ncom = n;
	pcom_p = new Vector(n);
	xicom_p = new Vector(n);
	nrfunc = func;
	Vector &pcom = *pcom_p, &xicom = *xicom_p;
	for (j = 0; j<n; j++) {
		pcom[j] = p[j];
		xicom[j] = xi[j];
	}
	ax = 0.0;
	xx = 1.0;
	mnbrak(ax, xx, bx, fa, fx, fb, f1dim);
	fret = brent(ax, xx, bx, f1dim, TOL, xmin);
	for (j = 0; j<n; j++) {
		xi[j] *= xmin;
		p[j] += xi[j];
	}
	delete xicom_p;
	delete pcom_p;
}


namespace {
	inline void shft3(ld &a, ld &b, ld &c, const ld d)
	{
		a = b;
		b = c;
		c = d;
	}
}

ld brent(const ld ax, const ld bx, const ld cx, ld f(const ld),
	const ld tol, ld &xmin)
{
	const int ITMAX = 100;
	const ld CGOLD = 0.3819660;
	const ld ZEPS = numeric_limits<ld>::epsilon()*1.0e-3;
	int iter;
	ld a, b, d = 0.0, etemp, fu, fv, fw, fx;
	ld p, q, r, tol1, tol2, u, v, w, x, xm;
	ld e = 0.0;

	a = (ax < cx ? ax : cx);
	b = (ax > cx ? ax : cx);
	x = w = v = bx;
	fw = fv = fx = f(x);
	for (iter = 0; iter<ITMAX; iter++) {
		xm = 0.5*(a + b);
		tol2 = 2.0*(tol1 = tol * fabs(x) + ZEPS);
		if (fabs(x - xm) <= (tol2 - 0.5*(b - a))) {
			xmin = x;
			return fx;
		}
		if (fabs(e) > tol1) {
			r = (x - w)*(fx - fv);
			q = (x - v)*(fx - fw);
			p = (x - v)*q - (x - w)*r;
			q = 2.0*(q - r);
			if (q > 0.0) p = -p;
			q = fabs(q);
			etemp = e;
			e = d;
			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q * (a - x) || p >= q * (b - x))
				d = CGOLD * (e = (x >= xm ? a - x : b - x));
			else {
				d = p / q;
				u = x + d;
				if (u - a < tol2 || b - u < tol2)
					d = SIGN(tol1, xm - x);
			}
		}
		else {
			d = CGOLD * (e = (x >= xm ? a - x : b - x));
		}
		u = (fabs(d) >= tol1 ? x + d : x + SIGN(tol1, d));
		fu = f(u);
		if (fu <= fx) {
			if (u >= x) a = x; else b = x;
			shft3(v, w, x, u);
			shft3(fv, fw, fx, fu);
		}
		else {
			if (u < x) a = u; else b = u;
			if (fu <= fw || w == x) {
				v = w;
				w = u;
				fv = fw;
				fw = fu;
			}
			else if (fu <= fv || v == x || v == w) {
				v = u;
				fv = fu;
			}
		}
	}
	xmin = x;
	return fx;
}


void mnbrak(ld &ax, ld &bx, ld &cx, ld &fa, ld &fb, ld &fc,
	ld func(const ld))
{
	const ld GOLD = 1.618034, GLIMIT = 100.0, TINY = 1.0e-20;
	ld ulim, u, r, q, fu;

	fa = func(ax);
	fb = func(bx);
	if (fb > fa) {
		SWAP(ax, bx);
		SWAP(fb, fa);
	}
	cx = bx + GOLD * (bx - ax);
	fc = func(cx);
	while (fb > fc) {
		r = (bx - ax)*(fb - fc);
		q = (bx - cx)*(fb - fa);
		u = bx - ((bx - cx)*q - (bx - ax)*r) /
			(2.0*SIGN(MAX(fabs(q - r), TINY), q - r));
		ulim = bx + GLIMIT * (cx - bx);
		if ((bx - u)*(u - cx) > 0.0) {
			fu = func(u);
			if (fu < fc) {
				ax = bx;
				bx = u;
				fa = fb;
				fb = fu;
				return;
			}
			else if (fu > fb) {
				cx = u;
				fc = fu;
				return;
			}
			u = cx + GOLD * (cx - bx);
			fu = func(u);
		}
		else if ((cx - u)*(u - ulim) > 0.0) {
			fu = func(u);
			if (fu < fc) {
				shft3(bx, cx, u, u + GOLD * (u - cx));
				shft3(fb, fc, fu, func(u));
			}
		}
		else if ((u - ulim)*(ulim - cx) >= 0.0) {
			u = ulim;
			fu = func(u);
		}
		else {
			u = cx + GOLD * (cx - bx);
			fu = func(u);
		}
		shft3(ax, bx, cx, u);
		shft3(fa, fb, fc, fu);
	}
}
