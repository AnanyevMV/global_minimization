#include <vector>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <cmath>

using namespace std;

typedef long double Real;
typedef std::vector<Real> Vector;
typedef Real(*Function)(const Vector & x);

const Real COMPARE_EPS = 0.0000000000000001L;					// Точность сравнения вещественных чисел

struct IterationData {											//структура данных, возвращаемая методом
																
	Vector x_prev;            
	Real   f_prev;            
	Vector x_curr;                
	Real   f_curr;            
	int    iter_counter;

	void next(const Vector& x_next, const Real f_next) {			//переход к следующей итерации
		
		f_prev = f_curr;
		x_prev = x_curr;
		x_curr = x_next;
		f_curr = f_next;
		iter_counter++;

	}

	IterationData() {											//конструктор

		f_prev = f_curr = iter_counter = 0;

	}
};

Vector grad(Function f, const Vector& point, const Real h) {	//градиент, вычисляемый по формуле центральных разностей

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
																//Перегрузка операции вывода в поток
std::ostream& operator<<(std::ostream& os, const Vector& v) {
	for (auto& it : v) {
		os << std::setprecision(8) << std::fixed << std::setw(16) << it;
	}
	return os;
}
																//Перегрузка операций для векторов
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

Vector operator*(Vector v1, const Vector& v2) {
	assert(v1.size() == v2.size());
	for (int i = 0; i < (int)v1.size(); ++i) {
		v1[i] = v1[i]*v2[i];
	}
	return v1;
}
																//
																//проверка равенства вектора нулю
bool is_zero(const Vector& v) {
	for (const auto & it : v) {
		if (std::abs(it) > COMPARE_EPS) {
			return false;
		}
	}
	return true;
}
																//покомпонентное деление
Vector operator/(Vector v1, const Vector& v2) {
	assert(v1.size() == v2.size());
	for (int i = 0; i < (int)v1.size(); ++i) {
		v1[i] = v1[i] / v2[i];
	}
	return v1;
}
																//покомпонетный квадратный корень
Vector sqrt(Vector v1) {
	for (int i = 0; i < (int)v1.size(); ++i) {
		v1[i] = sqrt(v1[i]);
	}
	return v1;
}
																//сглаживание вектора нецентрированной дисперсии во избежание деления на ноль
Vector notNull(Vector v1, Real e) {
	for (int i = 0; i < (int)v1.size(); ++i) {
		if (v1[i] == 0)
			v1[i] = v1[i] + e;
	}
	return v1;
}
																//тестовая функция
Real f25(const Vector &v) {
	// VIII f_min(0,1,1,1+-pi*n)=0
	return std::pow(v[0] * v[0] - v[1] + 1, 4) + 100 * std::pow(v[1] - v[2], 6) + std::pow(std::tan(v[2] - v[3]), 4) + std::pow(v[0], 8) + std::pow(v[3] - 1, 2);
}
																//
IterationData adam(Function f, Vector startPoint, Real accuracy) {

	IterationData data;
	data.x_curr = startPoint;
	data.f_curr = f(startPoint);
	data.iter_counter = 0;

	Vector x_next;
	Vector g, gd;												//градиет и средняя нецентрированная дисперсия
	Real f_next;
	Real e;														//коэффициент сглаживания, чтобы избежать деления на 0
	Real beta1, beta2;											//коэффициенты сохранения
	Real gamma;													//скорость обучения 
	Vector m, m_t, v, v_t;								//накопление градиента и средней нецентрированной дисперсии


	e = 0.00000001;
	beta1 = 0.9;
	beta2 = 0.999;
	gamma = (1 - beta1);

	g = grad(f, data.x_curr, 0.00000001);
	gd = g * g; 
	m = (1 - beta1) * g;
	v = (1 - beta2) * gd;
	m_t = m * (1 / (1 - beta1));
	v_t = v * (1 / (1 - beta2));
	x_next = data.x_curr - gamma * (m_t / (sqrt(notNull(v_t, e)))) ; 
	f_next = f(x_next);
	cout << "grad=" << g << endl;
	cout << "x_next=" << x_next << endl;
	while ((abs(data.f_curr - f_next) > accuracy) || is_zero(g)) {
		data.next(x_next, f_next);
		g = grad(f, data.x_curr, 0.00000001);
		gd = g * g; 
		m = beta1 * m + (1 - beta1) * g;
		v = beta2 * v + (1 - beta2) * gd;
		if (data.iter_counter < 1000) {
			m_t = m * (1 / (1 - beta1));
			v_t = v * (1 / (1 - beta2));
		}
		else {
			m_t = m;
			v_t = v;
		}
		x_next = data.x_curr - gamma * (m_t / (sqrt(notNull(v_t, e)))); 
		f_next = f(x_next);
		//cout << "grad=" << g << endl;
		//cout << "x_next=" << x_next << endl;
		//cout << "f_next=" << f_next << endl;
	}

	return data;
}

int main()
{
	Vector x = { 0,0,0,0 };
	cout << adam(f25, x, 0.000001).iter_counter << endl;
	char c;
	cin >> c;
    return 0;
}

