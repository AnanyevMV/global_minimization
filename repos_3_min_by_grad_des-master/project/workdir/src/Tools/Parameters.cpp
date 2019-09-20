#include <fstream>

#include "Parameters.h"

// Обновление статических параметров
void Parameters::updParams(Method method, Vector params) {

  if (method == Adam) {
    Adam_beta1 = params[0];
    Adam_beta2 = params[1];
    Adam_gamma = params[2];
  }

  if (method == RmsProp) {
    RmsProp_beta = params[0];
    RmsProp_alpha = params[1];

  }

  if (method == Adagrad) {
    Adagrad_beta = params[0];
  }

  if (method == AdaMax) {
    AdaMax_beta1 = params[0];
    AdaMax_beta2 = params[1];
    AdaMax_gamma = params[2];
  }

  if (method == Adadelta) {
	  Adadelta_beta = params[0];
  }

  if (method == Nadam) {
    Nadam_beta1 = params[0];
    Nadam_beta2 = params[1];
    Nadam_gamma = params[2];
  }

  if (method == AMSGrad) {
    AMSGrad_beta1 = params[0];
    AMSGrad_beta2 = params[1];
    AMSGrad_gamma = params[2];
  }

}

// Получение статических параметров
Vector Parameters::getParams(Method method) {

  if (method == Adam) {
    return { Adam_beta1, Adam_beta2, Adam_gamma };
  }

  if (method == RmsProp) {
    return { RmsProp_beta, RmsProp_alpha };
  }

  if (method == Adagrad) {
    return { Adagrad_beta };
  }

  if (method == AdaMax) {
    return { AdaMax_beta1, AdaMax_beta2, AdaMax_gamma };
  }

	if (method == Adadelta) {
		return{ Adadelta_beta };
	}

  if (method == Nadam) {
    return { Nadam_beta1, Nadam_beta2, Nadam_gamma };
  }

  if (method == AMSGrad) {
    return { AMSGrad_beta1, AMSGrad_beta2, AMSGrad_gamma };
  }

  //Выход при несовпадении имен
  std::cout << "WARNING: unidentified name in Parameters::getParams(methodName)" << endl;
  return { NULL };

}

// Расчет спец. градиента
Vector Parameters::nGrad(Method method, Vector &params, FLISTPTR head, Real nGrad_accuracy, Real grad_accuracy, int iter_lim) {
  int n = (int)params.size();
  Vector grad(n);

  for (int i = 0; i < n; i++) {
    params[i] = params[i] + nGrad_accuracy;
    updParams(method, params);
    grad[i] = sNorm(method, head, grad_accuracy, iter_lim);
    params[i] = params[i] - 2 * nGrad_accuracy;
    updParams(method, params);
    grad[i] = (grad[i] - sNorm(method, head, grad_accuracy, iter_lim))/2*nGrad_accuracy;
    params[i] = params[i] + nGrad_accuracy;
    updParams(method, params);
  }

  return grad;
}

// Расчет спец. нормы отклонений Х по всем функциям из списка хэд
Real Parameters::sNorm(Method method, FLISTPTR head, Real grad_accuracy, int iter_lim) {
  FLISTPTR ptr = head;
  Real snorm = 0;

  while(ptr){
    Vector xs = method(ptr->f, ptr->point_on_optimize_params, Parameters::getParams(method), grad_accuracy, iter_lim).x_curr;
    snorm = snorm + norm(xs - ptr->x_real_min);
    ptr = ptr->next;
  }

  return snorm;
}

void Parameters::checkParams(Vector &params) {
  int n = (int)params.size();

  for (int i = 0; i < n; i++)
    if (params[i] >= 1)
      params[i] = 0.99;
    else if (params[i] <= 0)
     params[i] = 0.01;
}

/*
 * Оптимизация параметров методов в списке мхэд по функциям
 * в списке фхэд градиентным спуском
 *
 */
void Parameters::calcParams(FLISTPTR fhead, MLISTPTR mhead, Real nGrad_accuracy, Real grad_accuracy, int iter_limit, int method_iter_limit, Real stop_condition) {

  MLISTPTR mptr = mhead;
  Vector params;
  Vector prev_params;
  Vector min_params;
  Real prevNorm, curNorm, minNorm;
  int count;

  ofstream fout;
  fout.open("optimized_parameters.txt");


  while (mptr) {
    params = getParams(mptr->m);
    prev_params = params;


    fout << mptr->name << endl;
    prevNorm = sNorm(mptr->m, fhead, grad_accuracy, method_iter_limit);
    minNorm = prevNorm;
    min_params = params;
    // Градиентный спуск по параметрам метода
    params = params - nGrad(mptr->m, params, fhead, nGrad_accuracy, grad_accuracy, method_iter_limit);
    //checkParams(adam_params);
    updParams(mptr->m, params);
    curNorm = sNorm(mptr->m, fhead, grad_accuracy, method_iter_limit);

    if (curNorm < minNorm) {
      minNorm = curNorm;
      min_params = params;
    }

    fout << curNorm << endl;
    fout << params << endl;

    count = 1;

    while (abs(curNorm - prevNorm) > stop_condition) {
      prevNorm = curNorm;
      prev_params = params;
      // Градиентный спуск по параметрам метода
      params = params - nGrad(mptr->m, params, fhead, nGrad_accuracy, grad_accuracy, method_iter_limit);
      //checkParams(adam_params);
      updParams(mptr->m, params);
      curNorm = sNorm(mptr->m, fhead, grad_accuracy, method_iter_limit);

      if (curNorm < minNorm) {
        minNorm = curNorm;
        min_params = params;
      }

      fout << "Norm= " << curNorm << endl;
      fout << "Params= " << params << endl;
      count++;

      if (count > iter_limit) //Итерации расчета градиента
        break;
    }

    fout << "min_Norm= " << minNorm << endl;
    fout << "opt_params= " << min_params << endl << endl;

    updParams(mptr->m, min_params);
    mptr = mptr->next;
  }


  fout.close();
}

/* Рассчитанные прежним вызовом параметры
Real Parameters::Adam_beta1 = 0.86296943;
Real Parameters::Adam_beta2 = 0.88791641;
Real Parameters::Adam_gamma = 0.07670128;
Real Parameters::RmsProp_beta = 0.93005474;
Real Parameters::RmsProp_alpha = 0.06376147;
Real Parameters::Adagrad_beta = 0.9;
Real Parameters::AdaMax_beta1 = 0.92096770;
Real Parameters::AdaMax_beta2 = 0.95563197;
Real Parameters::AdaMax_gamma = 0.18211471;
Real Parameters::Adadelta_beta = 0.9;
Real Parameters::Nadam_beta1 = 0.88441989;
Real Parameters::Nadam_beta2 = 0.88441989;
Real Parameters::Nadam_gamma = 0.08381000;
*/
