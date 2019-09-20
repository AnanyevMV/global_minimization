#include "Adadelta.h"

#include "Parameters.h"

IterationData Adadelta(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim) {

	IterationData data;
	data.x_curr = startPoint;
	data.f_curr = f(startPoint);
	data.iter_counter = 0;
	
	// Возможная точка мимнимума
	Vector x_next;	
	
	// Градиент, квадрат градиента
	Vector g, gd;	
	
	//  Параметр сохранения второго момента
	Vector v;

	// Значение функции на следующем шаге
	Real f_next;
	
	// Параметр метода 
	Real beta;	

	// Для нужд метода =)
	Vector k;
	Vector s;

	
	beta = parameters[0];  	

	for (int i = 0;i < startPoint.size();i++) {
		s.push_back(0.0);
		k.push_back(0.0);
	}

	g = grad(f, data.x_curr, grad_accuracy); // Вычисляем градиент целеловой функции
	gd = g * g;
	v = (1 - beta) * gd;
	
	k = -(sqrt(notNull(s)) * g) / (sqrt(notNull(v)));
	s = beta * s + (1 - beta)*k*k;
	
	// Итеративная формула
	x_next = data.x_curr + k;
	f_next = f(x_next);

	while (data.iter_counter < iter_lim) {
		data.next(x_next, f_next);
		
		g = grad(f, data.x_curr, grad_accuracy);
		gd = g * g;
		v = beta * v + (1 - beta) * gd;
		
		k = -(sqrt(notNull(s)) *g) / (sqrt(notNull(v)));
		s = beta * s + (1 - beta)*k*k;

		// Итеративная формула
		x_next = data.x_curr + k;
		f_next = f(x_next);
	}

	return data;
}
