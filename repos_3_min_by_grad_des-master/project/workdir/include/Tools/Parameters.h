#pragma once

/* Tools */
#include "Functions.h"
#include "Lists.h"

/* Methods */
#include "Methods/Adagrad/Adagrad.h"
#include "Methods/Adam/Adam.h"
#include "Methods/RmsProp/RmsProp.h"
#include "Methods/AdaMax/AdaMax.h"
#include "Methods/Adadelta/Adadelta.h"
#include "Methods/Nadam/Nadam.h"
#include "Methods/AMSGrad/AMSGrad.h"

/**
 *
 * @struct Parameters
 * @brief Структура, содержащая статические параметры для методов
 *
 * @details https://3dbabove.com/2017/11/14/optimizationalgorithms/
 *
 * @var Parameters::Adam_beta1
 * Параметр сохранения первого момента в выражении m = (1 - beta1) * g, где g - градиент; m - момент
 * @var Parameters::Adam_beta2
 * Параметр сохранения второго момента в выражении v = (1 - beta2) * gd, где gd - покомпонентный квадрат градиента; v - момент второго порядка
 * @var Parameters::Adam_gamma
 * Шаг обучения
 *
 * @var Parameters::RmsProp_beta
 * Параметр сохранения второго момента в выражении v = (1 - beta2) * gd, где gd - покомпонентный квадрат градиента; v - момент второго порядка
 * @var Parameters::RmsProp_alpha
 * Шаг обучения
 *
 * @var Parameters::Adagrad_beta
 * Шаг обучения
 *
 * @var Parameters::AdaMax_beta1
 * Параметр сохранения первого момента в выражении m = (1 - beta1) * g, где g - градиент; m - момент
 * @var Parameters::AdaMax_beta2
 * Параметр сохранения нормы Lp градиента при p стремящимся к бесконечности; v - норма градиента
 * @var Parameters::AdaMax_gamma
 * Шаг обучения
 *
 * @var Parameters::Adadelta_beta
 * Параметр сохранения второго момента в выражении v = (1 - beta) * gd, где gd - покомпонентный квадрат градиента; v - момент второго порядка
 *
 * @var Parameters::Nadam_beta1
 * Параметр сохранения первого момента в выражении m = (1 - beta1) * g, где g - градиент; m - момент
 * @var Parameters::Nadam_beta2
 * Параметр сохранения второго момента в выражении v = (1 - beta2) * gd, где gd - покомпонентный квадрат градиента; v - момент второго порядка
 * @var Parameters::Nadam_gamma
 * Шаг обучения
 *
 * @var Parameters::AMSGrad_beta1
 * Параметр сохранения первого момента в выражении m = (1 - beta1) * g, где g - градиент; m - момент
 * @var Parameters::AMSGrad_beta2
 * Параметр сохранения второго момента в выражении v = (1 - beta2) * gd, где gd - покомпонентный квадрат градиента; v - момент второго порядка
 * @var Parameters::AMSGrad_gamma
 * Шаг обучения
 *
*/

struct Parameters {
	static Real Adam_beta1, Adam_beta2, Adam_gamma;
	static Real RmsProp_beta, RmsProp_alpha;
	static Real Adagrad_beta;
	static Real AdaMax_beta1, AdaMax_beta2, AdaMax_gamma;
	static Real Adadelta_beta;
  static Real Nadam_beta1, Nadam_beta2, Nadam_gamma;
	static Real AMSGrad_beta1, AMSGrad_beta2, AMSGrad_gamma;

	static void updParams(Method method, Vector params);

	static Vector getParams(Method method);

	static Vector nGrad(Method method, Vector &params, FLISTPTR head, Real nGrad_accuracy, Real grad_accuracy, int iter_lim);

	static Real sNorm(Method method, FLISTPTR head, Real grad_accuracy, int iter_lim);

	static void checkParams(Vector &params);

	static void calcParams(FLISTPTR fhead, MLISTPTR mhead, Real nGrad_accuracy, Real grad_accuracy, int iter_limit, int method_iter_limit, Real stop_condition);
};
