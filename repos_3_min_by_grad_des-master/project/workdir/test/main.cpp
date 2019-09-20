/* Standart libs */
#include <fstream>
#include <stdlib.h>
#include <search.h>

/* Methods */
#include "Adagrad.h"
#include "Adam.h"
#include "RmsProp.h"
#include "AdaMax.h"
#include "Adadelta.h"
#include "Nadam.h"
#include "AMSGrad.h"

/* Tools */
#include "Data.h"
#include "Math.h"
#include "Functions.h"
#include "Lists.h"
#include "Parameters.h"

/* Config */
#include "ConfigMain.h"


// Функция сравнения двух элементов, необходимая для библиотечной qsort
int compvar(const void *one, const void *two) {
  Real a = *((Real*)one);
  Real b = *((Real*)two);

  if (a < b)
    return -1;

  if (a == b)
    return 0;

  return 1;

}

/*
 * Основной тест:
 * на передаваемом списке1
 * функций для каждой из
 * множества стартовой точки
 * вызываются все методы из списка2
*/
void basicTest(FLISTPTR testFunctionList, MLISTPTR testMethodList, Real grad_accuracy, int iter_lim) {
  FLISTPTR fptr = testFunctionList;
  MLISTPTR mptr = testMethodList;

  if (!mptr || !fptr) {
    cout << "basicTest Warning: BAD LIST" << endl;
    return;
  }

  Vector x;
  Real curnorm;

  /*
   * Описание и начальная инициализация анализируемых значений:
   * худшие функции,
   * стартовые точки,
   * суммарные и медианные отклонения от реальных минимумов
  */
  vector<const char *> mname;
  vector<const char *> worst_fname;
  vector<Vector> worst_start_point;
  vector<Vector> worst_real_min;
  vector<Vector> worst_calc_min;
  vector<Vector> all_norms;

  Vector worst_function_value;
  Vector worst_function_start_value;
  Vector worst_function_true_value;

  Vector worst_norm;
  Vector sum_norm;
  Vector median_norm;

  int test_id;
  int method_id;
  int i;

  ofstream fout;
  fout.open("basic_test_raw_result.txt");

  while (mptr) {
    Vector norms;
    mname.push_back(mptr->name);
    worst_fname.push_back(fptr->name);
    worst_start_point.push_back(fptr->startPoints[0]);
    worst_real_min.push_back(fptr->x_real_min);
    worst_calc_min.push_back(fptr->x_real_min);

    worst_function_value.push_back(fptr->f(fptr->x_real_min));
    worst_function_start_value.push_back(fptr->f(fptr->x_real_min));
    worst_function_true_value.push_back(fptr->f(fptr->x_real_min));


    while (fptr) {
      for (i = 0; i < (int)fptr->startPoints.size(); i++) {
        norms.push_back(0);
      }

      fptr = fptr->next;
    }

    fptr = testFunctionList;
    all_norms.push_back(norms);
    worst_norm.push_back(0);
    sum_norm.push_back(0);
    median_norm.push_back(0);
    mptr = mptr->next;
  }

  fptr = testFunctionList;
  mptr = testMethodList;
  test_id = 0;

  // Начало прохода по списку функций:
  while (fptr) {
    fout << fptr->name << ':' << endl;
    fout << "true_min: " << fptr->x_real_min << endl << endl;

    // По множеству стартовых точек:
    for (i = 0; i < (int)fptr->startPoints.size(); i++) {
      fout << '\t' << "start_point: " << fptr->startPoints[i] << endl;
      mptr = testMethodList;
      method_id = 0;

      // По списку методов:
      while (mptr) {
        fout << '\t' << '\t' << mptr->name << ": ";
        // Рассчитанный минимум
        x = mptr->m(fptr->f, fptr->startPoints[i], Parameters::getParams(mptr->m), grad_accuracy, iter_lim).x_curr;

        // Его отклонение от реального минимума текущей функции
        curnorm = norm(x - fptr->x_real_min);

        fout << x << '\t' << "||finish_point - truemin|| = " << curnorm << endl;
        mptr = mptr->next;
        //cout << endl;

        // Заносим в массив всех отклонений для данного метода
        all_norms[method_id][test_id] = curnorm;
        // Сумма отклонений для данного метода
        sum_norm[method_id] = sum_norm[method_id] + curnorm;

        // Для худшего случая:
        if (curnorm > worst_norm[method_id]) {
          worst_fname[method_id] = fptr->name;
          worst_start_point[method_id] = fptr->startPoints[i];
          worst_real_min[method_id] = fptr->x_real_min;
          worst_calc_min[method_id] = x;
          worst_function_value[method_id] = fptr->f(x);
          worst_function_start_value[method_id] = fptr->f(worst_start_point[method_id]);
          worst_function_true_value[method_id] = fptr->f(worst_real_min[method_id]);
          worst_norm[method_id] = curnorm;
        }

        method_id++;
      }

      test_id++;
      fout << endl;
    }

    cout << "function " << fptr->name << " DONE" << endl;
    fptr = fptr->next;
    fout << endl << endl << endl << endl;
  }

  fout.close();

  // Вывод проанализированных данных:
  fout.open("basic_test_analysed_result.txt");

  for (method_id = 0; method_id < (int)mname.size(); method_id++) {
    int size;
    Vector norms;
    fout << mname[method_id] << ": " << endl;
    fout << "Worst function: " << worst_fname[method_id] << endl;
    fout << "True minimum: " << worst_real_min[method_id] << endl;
    fout << "Worst start point: " << worst_start_point[method_id] << endl;
    fout << "Calculated minimum: " << worst_calc_min[method_id] << endl;
    fout << "Function value in true minimum: " << worst_function_true_value[method_id] << endl;
    fout << "Function value in start point: " << worst_function_start_value[method_id] << endl;
    fout << "Function value in finish point: " << worst_function_value[method_id] << endl;
    fout << "||finish_point - truemin||: " << worst_norm[method_id] << endl;
    fout << "Total norm of difference " << sum_norm[method_id] << endl;

    // Расчет медианного отклонения:
    size = all_norms[method_id].size();
    qsort(&all_norms[method_id][0], size, sizeof(Real), compvar);

    if (size % 2)
      median_norm[method_id] = all_norms[method_id][(int)size / 2];
    else
      median_norm[method_id] = (all_norms[method_id][size / 2] + all_norms[method_id][size / 2 - 1]) / 2;
      fout << "Median norm of difference: " << median_norm[method_id] << endl;
      fout << endl << endl << endl;
  }

  fout.close();
  cout << "test_DONE" << endl;
}


int main() {
  FLISTPTR fhead = NULL;
  MLISTPTR mhead = NULL;
  fhead = createFullFlist();
  mhead = createOurMlist();


  Real grad_accuracy = 0.00000001;
  int iter_limit = 300;

  CreateConfig();
  cout << "You can place your data in config.txt... Continue?" << endl;
  getchar();
  ReadConfig();

  genDots(fhead);
  basicTest(fhead, mhead, grad_accuracy, iter_limit);
  //Parameters::calcParams(fhead, mhead, 0.00000001, 0.00000001, 300, 300, 0.00000001);
  return 0;
}
