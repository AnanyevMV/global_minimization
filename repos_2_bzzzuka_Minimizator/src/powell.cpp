#include "powell.hpp"
#include <cmath>  
#include <math.h>
#include <limits>
#include <tuple>



std::pair<ld, ld> SWAP(ld &a, ld &b)
{
	ld dum = a; a = b; b = dum;
	return { a, b };
}

std::pair<Vector, Vector>  linmin(Vector &p, Vector &xi, Function f)
{
	const ld TOL = 1.0e-8;
	int n = p.size();
	int ncom = n;
	Vector *pcom_p = new Vector(n);
	Vector *xicom_p = new Vector(n);
	Function nrfunc = f;
	Vector &pcom = *pcom_p, &xicom = *xicom_p;
	for (int j = 0; j<n; j++) {
		pcom[j] = p[j];
		xicom[j] = xi[j];
	}
	ld ax = 0.0;
	ld xx;
	ld bx = 1.0;
	ld  xmin, fx, fb, fa;
	//mnbrak
	//_______________________________________________________________
	const ld GOLD = 1.618034, GLIMIT = 100.0, TINY = 1.0e-20;
	ld ulim, u, r, q, fu;
	ld te; // ïîä ñâàï
	fa = f1dim(ax, ncom, pcom_p, xicom_p, nrfunc);
	fb = f1dim(bx, ncom, pcom_p, xicom_p, nrfunc);
	if (fb > fa) {
		std::pair<ld,ld> te = SWAP(ax, bx);
		//te = ax; ax = bx; bx = te; 
		te.first = bx;
		te.second = ax;
		std::pair<ld, ld> te1 = SWAP(fb, fa);
		te1.first = fa;
		te1.second = fb;
		//te = fb; fa = fb; fb = te;
	}
	ld cx = bx + GOLD * (bx - ax);
	ld fc = f1dim(cx, ncom, pcom_p, xicom_p, nrfunc);
	while (fb > fc) {
		r = (bx - ax)*(fb - fc);
		q = (bx - cx)*(fb - fa);
		u = bx - ((bx - cx)*q - (bx - ax)*r) /
			(2.0*SIGN(MAX(fabs(q - r), TINY), q - r));
		ulim = bx + GLIMIT * (cx - bx);
		if ((bx - u)*(u - cx) > 0.0) {
			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
			if (fu < fc) {
				ax = bx;
				bx = u;
				fa = fb;
				fb = fu;
			//	return;
			}
			else if (fu > fb) {
				cx = u;
				fc = fu;
			//	return;
			}
			u = cx + GOLD * (cx - bx);
			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
		}
		else if ((cx - u)*(u - ulim) > 0.0) {
			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
			if (fu < fc) {
				bx = cx; cx = u; u = u + GOLD * (u - cx);
				fb = fc; fc = fu; fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
			}
		}
		else if ((u - ulim)*(ulim - cx) >= 0.0) {
			u = ulim;
			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
		}
		else {
			u = cx + GOLD * (cx - bx);
			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
		}
		ax = bx; bx = cx; cx = u;
		fa = fb; fb = fc; fc = fu;
	}

	xx = bx;
	const int ITMAX = 100;
	const ld CGOLD = 0.3819660;
	const ld ZEPS = exp(-6);
	int iter;
	ld d = 0.0, etemp, fu1, fv, fx1;
	ld p1, q1, r1, tol1, tol2, u1, v, w, xm;     // íó òóò ðèàë õç êàê óïðîñòèòü 
	ld e = 0.0;

	ld a = (ax < cx ? ax : cx);
	ld b = (ax > cx ? ax : cx);
	ld x = w = v = bx;
	ld fw = fv = fx1 = f1dim(x, ncom, pcom_p, xicom_p, nrfunc);
	for (iter = 0; iter<ITMAX; iter++) {
		xm = 0.5*(a + b);
		tol2 = 2.0*(tol1 = TOL * fabs(x) + ZEPS);
		if (fabs(x - xm) <= (tol2 - 0.5*(b - a))) {
			xmin = x;
			//fret = fx1;
			break;
		}
		if (fabs(e) > tol1) {
			r1 = (x - w)*(fx1 - fv);
			q1 = (x - v)*(fx1 - fw);
			p1 = (x - v)*q1 - (x - w)*r1;
			q1 = 2.0*(q1 - r1);
			if (q1 > 0.0) p1 = -p1;
			q1 = fabs(q1);
			etemp = e;
			e = d;
			if (fabs(p1) >= fabs(0.5*q1*etemp) || p1 <= q1 * (a - x) || p1 >= q1 * (b - x))
				d = CGOLD * (e = (x >= xm ? a - x : b - x));
			else {
				d = p1 / q1;
				u1 = x + d;
				if (u1 - a < tol2 || b - u1 < tol2)
					d = SIGN(tol1, xm - x);
			}
		}
		else {
			d = CGOLD * (e = (x >= xm ? a - x : b - x));
		}
		u1 = (fabs(d) >= tol1 ? x + d : x + SIGN(tol1, d));
		fu1 = f1dim(u1, ncom, pcom_p, xicom_p, nrfunc);
		if (fu1 <= fx1) {
			if (u1 >= x) a = x; else b = x;

			v = w; w = x; x = u1;

			fv = fw; fw = fx1; fx1 = fu1;
		}
		else {
			if (u1 < x) a = u1; else b = u1;
			if (fu1 <= fw || w == x) {
				v = w;
				w = u1;
				fv = fw;
				fw = fu1;
			}
			else if (fu1 <= fv || v == x || v == w) {
				v = u1;
				fv = fu1;
			}
		}
	}
	//fret = fx1;
	//xmin = x;
	//return fx;
	//_____________________________________________________________
	for (int j = 0; j<n; j++) {
		xi[j] *= xmin;
		p[j] += xi[j];
	}
	return { p, xi };
	delete xicom_p;
	delete pcom_p;
}

ld f1dim(const ld x, int ncom, Vector *pcom_p, Vector *xicom_p, Function nrfunc)
{
	Vector xt(ncom);
	Vector &pcom = *pcom_p, &xicom = *xicom_p;
	for (int j = 0; j<ncom; j++)
		xt[j] = pcom[j] + x * xicom[j];
	return nrfunc(xt);
}


std::tuple<Vector, int, ld> powell1(Vector &p, ld **xi, const ld ftol, Function func)
{
	const int ITMAX = 200;
	int iter;
	const ld TINY = 1.0e-25;
	ld  fp, fptt, t, fret;
	int n = p.size();
	Vector pt(n), ptt(n), xit(n);
	fret = func(p);
	for (int j = 0; j<n; j++) pt[j] = p[j];
	for (iter = 0; iter<200; iter++) {
		fp = fret;
		int ibig = 0;
		ld del = 0.0;
		for (int i = 0; i<n; i++) {
			for (int j = 0; j<n; j++) xit[j] = xi[j][i];
			fptt = fret;
			std::pair<Vector, Vector> qwe = linmin(p, xit, func);
			p = qwe.first; 
			xit = qwe.second; 
			if (fptt - fret > del) {
				del = fptt - fret;
				ibig = i + 1;
			}
		}
		if (2.0*(fp - fret) <= ftol * (fabs(fp) + fabs(fret)) + TINY) {
			return {p, iter, fret};
		}
		if (iter == ITMAX) return {p, iter, fret};
		for (int j = 0; j<n; j++) {
			ptt[j] = 2.0*p[j] - pt[j];
			xit[j] = p[j] - pt[j];
			pt[j] = p[j];
		}
		fptt = func(ptt);
		if (fptt < fp) {
			t = 2.0*(fp - 2.0*fret + fptt)*SQR(fp - fret - del) - del * SQR(fp - fptt);
			if (t < 0.0) {
				std::pair<Vector, Vector> qwe = linmin(p, xit, func);
				p = qwe.first;
				xit = qwe.second;
				for (int j = 0; j<n; j++) {
					xi[j][ibig - 1] = xi[j][n - 1];
					xi[j][n - 1] = xit[j];
				}
			}
		}

	}
}

std::pair<Vector, int> powell(Function f, Vector start_point, int n) {
	
	ld MIN_VAL = 100000; // ìèíèìóì ôóíêöèè
	int NDIM = (int)start_point.size(); // îïðåëÿåì ðàçìåðíîñòü 
	Vector x_cur = start_point;
	const ld FTOL = 1.0e-6;
	Vector p_d = start_point;
	Vector p = p_d;
	ld **xi = new ld*[NDIM]; 
	for (int count = 0; count < NDIM; count++)
		xi[count] = new ld[NDIM];
	
	for (int i = 0; i < NDIM; i++)
		for (int j = 0; j < NDIM; j++)
			xi[i][j] = (i == j ? 1.0 : 0.0);
	
	ld fret;
	std::tuple<Vector, int, ld> ans = powell1(p, xi, FTOL, f);
	x_cur = std::get<0>(ans);
	//x_cur = ans.first;
	//ld iter = ans.second;
	int iter = std::get<1>(ans) = 1;
	//fret = ans.third; 
	fret = std::get<2>(ans);
	//x_cur = p;
	if (fret < MIN_VAL) { MIN_VAL = fret; }



	return { x_cur ,iter };
}




// void linmin(Vector &p, Vector &xi, ld &fret, Function f)
// {
// 	const ld TOL = 1.0e-8;
// 	int n = p.size();
// 	int ncom = n;
// 	Vector *pcom_p = new Vector(n);
// 	Vector *xicom_p = new Vector(n);
// 	Function nrfunc = f;
// 	Vector &pcom = *pcom_p, &xicom = *xicom_p;
// 	for (int j = 0; j<n; j++) {
// 		pcom[j] = p[j];
// 		xicom[j] = xi[j];
// 	}
// 	ld ax = 0.0;
// 	ld xx;
// 	ld bx = 1.0;
// 	ld  xmin, fx, fb, fa;
// 	//mnbrak
// 	//_______________________________________________________________,nrfunc
// 	const ld GOLD = 1.618034, GLIMIT = 100.0, TINY = 1.0e-20;
// 	ld ulim, u, r, q, fu;
// 	fa = f1dim(ax, ncom, pcom_p, xicom_p, nrfunc);
// 	fb = f1dim(bx, ncom, pcom_p, xicom_p, nrfunc);
// 	if (fb > fa) {
// 		SWAP(ax, bx);
// 		SWAP(fb, fa);
// 	}
// 	ld cx = bx + GOLD * (bx - ax);
// 	ld fc = f1dim(cx, ncom, pcom_p, xicom_p, nrfunc);
// 	while (fb > fc) {
// 		r = (bx - ax)*(fb - fc);
// 		q = (bx - cx)*(fb - fa);
// 		u = bx - ((bx - cx)*q - (bx - ax)*r) /
// 			(2.0*SIGN(MAX(fabs(q - r), TINY), q - r));
// 		ulim = bx + GLIMIT * (cx - bx);
// 		if ((bx - u)*(u - cx) > 0.0) {
// 			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
// 			if (fu < fc) {
// 				ax = bx;
// 				bx = u;
// 				fa = fb;
// 				fb = fu;
// 				return;
// 			}
// 			else if (fu > fb) {
// 				cx = u;
// 				fc = fu;
// 				return;
// 			}
// 			u = cx + GOLD * (cx - bx);
// 			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
// 		}
// 		else if ((cx - u)*(u - ulim) > 0.0) {
// 			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
// 			if (fu < fc) {
// 				bx = cx; cx = u; u = u + GOLD * (u - cx);
// 				fb = fc; fc = fu; fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
// 			}
// 		}
// 		else if ((u - ulim)*(ulim - cx) >= 0.0) {
// 			u = ulim;
// 			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
// 		}
// 		else {
// 			u = cx + GOLD * (cx - bx);
// 			fu = f1dim(u, ncom, pcom_p, xicom_p, nrfunc);
// 		}
// 		ax = bx; bx = cx; cx = u;
// 		fa = fb; fb = fc; fc = fu;
// 	}

// 	xx = bx;
// 	const int ITMAX = 100;
// 	const ld CGOLD = 0.3819660;
// 	const ld ZEPS = exp(-6);
// 	int iter;
// 	ld d = 0.0, etemp, fu1, fv, fx1;
// 	ld p1, q1, r1, tol1, tol2, u1, v, w, xm;     // íó òóò ðèàë õç êàê óïðîñòèòü 
// 	ld e = 0.0;

// 	ld a = (ax < cx ? ax : cx);
// 	ld b = (ax > cx ? ax : cx);
// 	ld x = w = v = bx;
// 	ld fw = fv = fx1 = f1dim(x, ncom, pcom_p, xicom_p, nrfunc);
// 	for (iter = 0; iter<ITMAX; iter++) {
// 		xm = 0.5*(a + b);
// 		tol2 = 2.0*(tol1 = TOL * fabs(x) + ZEPS);
// 		if (fabs(x - xm) <= (tol2 - 0.5*(b - a))) {
// 			xmin = x;
// 			//fret = fx1;
// 			break;
// 		}
// 		if (fabs(e) > tol1) {
// 			r1 = (x - w)*(fx1 - fv);
// 			q1 = (x - v)*(fx1 - fw);
// 			p1 = (x - v)*q - (x - w)*r;
// 			q1 = 2.0*(q - r1);
// 			if (q1 > 0.0) p1 = -p1;
// 			q1 = fabs(q);
// 			etemp = e;
// 			e = d;
// 			if (fabs(p1) >= fabs(0.5*q1*etemp) || p1 <= q1 * (a - x) || p1 >= q1 * (b - x))
// 				d = CGOLD * (e = (x >= xm ? a - x : b - x));
// 			else {
// 				d = p1 / q1;
// 				u1 = x + d;
// 				if (u - a < tol2 || b - u < tol2)
// 					d = SIGN(tol1, xm - x);
// 			}
// 		}
// 		else {
// 			d = CGOLD * (e = (x >= xm ? a - x : b - x));
// 		}
// 		u1 = (fabs(d) >= tol1 ? x + d : x + SIGN(tol1, d));
// 		fu1 = f1dim(u1, ncom, pcom_p, xicom_p, nrfunc);
// 		if (fu1 <= fx1) {
// 			if (u >= x) a = x; else b = x;

// 			v = w; w = x; x = u1;

// 			fv = fw; fw = fx1; fx1 = fu1;
// 		}
// 		else {
// 			if (u1 < x) a = u1; else b = u1;
// 			if (fu1 <= fw || w == x) {
// 				v = w;
// 				w = u1;
// 				fv = fw;
// 				fw = fu1;
// 			}
// 			else if (fu1 <= fv || v == x || v == w) {
// 				v = u1;
// 				fv = fu1;
// 			}
// 		}
// 	}
// 	//fret = fx1;
// 	//xmin = x;
// 	//return fx;
// 	//_____________________________________________________________
// 	for (int j = 0; j<n; j++) {
// 		xi[j] *= xmin;
// 		p[j] += xi[j];
// 	}
// 	delete xicom_p;
// 	delete pcom_p;
// }


// ld f1dim(const ld x, int ncom, Vector *pcom_p, Vector *xicom_p, Function nrfunc)
// {
// 	Vector xt(ncom);
// 	Vector &pcom = *pcom_p, &xicom = *xicom_p;
// 	for (int j = 0; j<ncom; j++)
// 		xt[j] = pcom[j] + x * xicom[j];
// 	return nrfunc(xt);
// }


// void powell1(Vector &p, ld **xi, const ld ftol, int &iter,
// 	ld &fret, Function func)
// {
// 	const int ITMAX = 200;
// 	const ld TINY = 1.0e-25;
// 	ld  fp, fptt, t; 
// 	int n = p.size();
// 	Vector pt(n), ptt(n), xit(n);
// 	fret = func(p);
// 	for (int j = 0; j<n; j++) pt[j] = p[j];
// 	for (iter = 0; iter<200 ; iter++) {
// 		fp = fret;
// 		int ibig = 0; 
// 		ld del = 0.0;  
// 		for (int i = 0; i<n; i++) {
// 			for (int j = 0; j<n; j++) xit[j] = xi[j][i];
// 			fptt = fret;
// 			linmin(p, xit, fret, func);
// 			if (fptt - fret > del) {
// 				del = fptt - fret;
// 				ibig = i + 1;
// 			}
// 		}
// 		if (2.0*(fp - fret) <= ftol * (fabs(fp) + fabs(fret)) + TINY) {
// 			return;
// 		}
// 		if (iter == ITMAX) return;
// 		for (int j = 0; j<n; j++) {
// 			ptt[j] = 2.0*p[j] - pt[j];
// 			xit[j] = p[j] - pt[j];
// 			pt[j] = p[j];
// 		}
// 		fptt = func(ptt);
// 		if (fptt < fp) {
// 			t = 2.0*(fp - 2.0*fret + fptt)*SQR(fp - fret - del) - del * SQR(fp - fptt);
// 			if (t < 0.0) {
// 				linmin(p, xit, fret, func);
// 				for (int j = 0; j<n; j++) {
// 					xi[j][ibig - 1] = xi[j][n - 1];
// 					xi[j][n - 1] = xit[j];
// 				}
// 			}
// 		}
		
// 	}
// }


// std::pair<Vector, int> powell(Function f, Vector start_point, int n) {
	
// 	ld MIN_VAL = 100000; // ìèíèìóì ôóíêöèè
// 	int NDIM = (int)start_point.size(); // îïðåëÿåì ðàçìåðíîñòü 
// 	Vector x_cur = start_point;
// 	const ld FTOL = 1.0e-6;
// 	Vector p_d = start_point;
// 	Vector p = p_d;
// 	ld **xi = new ld*[NDIM]; 
// 	for (int count = 0; count < NDIM; count++)
// 		xi[count] = new ld[NDIM];
	
// 	for (int i = 0; i < NDIM; i++)
// 		for (int j = 0; j < NDIM; j++)
// 			xi[i][j] = (i == j ? 1.0 : 0.0);
// 	int iter;
// 	ld fret;
// 	powell1(p, xi, FTOL, iter, fret, f);
// 	x_cur = p;
// 	if (fret < MIN_VAL) { MIN_VAL = fret; }



// 	return { x_cur ,iter };
// }