#include "AMSGrad.h"

#include "Tools/Parameters.h"

IterationData AMSGrad(Function f, Vector startPoint, Vector parameters, Real grad_accuracy, int iter_lim) {

  IterationData data;
  data.x_curr = startPoint;
  data.f_curr = f(startPoint);
  data.iter_counter = 0;

  Vector x_next;
  // Вектор-градиент и его покомпонентный квадрат
  Vector g, gd;

  Real f_next;
  // Сглаживающий коэффициент
  Real e;

  // Параметры сохранения моментов
  Real beta1, beta2;

  // Парaметр метода
  Real gamma;

  // Первый и второй моменты
  Vector m, m_t, v, v_t, v_tt;

  /* Присвоение соответствующих параметров из структуры параметров */

  beta1 = parameters[0];
  beta2 = parameters[1];
  gamma = parameters[2];


  g = grad(f, data.x_curr, grad_accuracy);
  gd = g * g;
  m = (1 - beta1) * g;
  v = (1 - beta2) * gd;
  m_t = m * (1 / (1 - beta1));
  v_t = v * (1 / (1 - beta2));
  v_tt = v_t;

  // Итеративная формула
  x_next = data.x_curr - gamma * (m_t / (sqrt(notNull(v_tt))));
  f_next = f(x_next);

  while (data.iter_counter < iter_lim) {
    data.next(x_next, f_next);
    g = grad(f, data.x_curr, grad_accuracy);
    gd = g * g;
    m = beta1 * m + (1 - beta1) * g;
    v = beta2 * v + (1 - beta2) *gd;

    m_t = m * (1 / (1 - pow(beta1, data.iter_counter)));
    v_t = v * (1 / (1 - pow(beta2, data.iter_counter)));

    v_tt = norm(v_tt) > norm(v_t) ? v_tt : v_t;

    // Итеративная формула
    x_next = data.x_curr - gamma * (m_t / (sqrt(notNull(v_tt))));
    f_next = f(x_next);
  }

  return data;

}
