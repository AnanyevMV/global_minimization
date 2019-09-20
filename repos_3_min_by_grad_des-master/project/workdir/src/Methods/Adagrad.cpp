/* Adaptive Gradient */
#include "Adagrad.h"

#include "Tools/Parameters.h"

IterationData Adagrad(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim) {

  IterationData data;
  data.x_curr = startPoint;
  data.f_curr = f(startPoint);
  data.iter_counter = 0;

  Vector x_next;
  // Вектор-градиент и его покомпонентный квадрат
  Vector g, gd;
  // Момент
  Vector v;

  Real f_next;
  // Сглаживающий коэффициент
  Real e;
  // Параметр метода
  Real beta;
  
  beta = parameters[0];

  g = grad(f, data.x_curr, grad_accuracy);
  gd = g * g;
  v = gd;
  // Итеративная формула
  x_next = data.x_curr - beta * (g / (sqrt(notNull(v))));
  f_next = f(x_next);

  while (data.iter_counter < iter_lim) {
    data.next(x_next, f_next);
    g = grad(f, data.x_curr, grad_accuracy);
    gd = g * g;
    v = v + gd;
    // Итеративная формула
    x_next = data.x_curr - beta * (g / sqrt(notNull(v)));
    f_next = f(x_next);
  }

  return data;

}
