#include "Methods/Adadelta/Adadelta.h"

IterationData Adadelta(Function f, Vector startPoint, const StopCondition& stop_condition) {

    IterationData data;
    data.x_initial = startPoint;
    data.f_initial = f(startPoint);
    data.x_curr = startPoint;
    data.f_curr = f(startPoint);
    data.iter_counter = 0;
    data.method_title = "Adadelta";
	Params par;
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
	Vector k;   //delta x_t
	Vector s;


	beta = par.Adadelta_beta;

	for (int i = 0;i < (int)startPoint.size();i++) {
		s.push_back(0.0);
		k.push_back(0.0);
	}

	g = grad(f, data.x_curr, 0.00000001); // Вычисляем градиент целеловой функции
	gd = g * g;
	v = (1 - beta) * gd;

    k = -(sqrt(notNull(s))) / (sqrt(notNull(v))) * g;
	s = (1 - beta)*k*k;

	// Итеративная формула
	x_next = data.x_curr + k;
	f_next = f(x_next);
    data.next(x_next, f_next);

	//while (data.iter_counter < 300) {
	//while (!stop_condition(data)) {
    do{
		g = grad(f, data.x_curr, 0.00000001);
		gd = g * g;
        v = beta * v + (1 - beta) * gd;

		k = -(sqrt(notNull(s))) / (sqrt(notNull(v))) * g;
		s = beta * s + (1 - beta)*k*k;

		// Итеративная формула
		x_next = data.x_curr + k;
		f_next = f(x_next);
        data.next(x_next, f_next);
	}while (!stop_condition(data));

	return data;
}
