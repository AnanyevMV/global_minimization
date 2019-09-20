#include "RmsProp.h"

#include "Tools/Parameters.h"


IterationData RmsProp(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim) {

  IterationData data;
  data.x_curr = startPoint;
  data.f_curr = f(startPoint);
  data.iter_counter = 0;

  Vector x_next;

  // Градиент и его покомпонентный квадрат
  Vector g, gd;
  // Mомент
  Vector v;

  Real f_next;
  // Сглаживающий коэффициент
  Real e;
  // Параметр сохранения момента
  Real beta;
  // Параметр метода
  Real alpha;

  beta = parameters[0];
  alpha = parameters[1];

  g = grad(f, data.x_curr, grad_accuracy); 
  gd = g * g;
  v = (1 - beta) * gd;
  // Итеративная формула
  x_next = data.x_curr - alpha * (g / (sqrt(notNull(v))));
  f_next = f(x_next);

  while (data.iter_counter < iter_lim) {
    data.next(x_next, f_next);
    g = grad(f, data.x_curr, grad_accuracy);
    gd = g * g;
    v = beta * v + (1 - beta) * gd;
    // Итеративная формула
    x_next = data.x_curr - alpha * (g / (sqrt(notNull(v))));
    f_next = f(x_next);
  }

  return data;
}
