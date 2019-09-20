#pragma once

#include "Functions.h"
#include "Data.h"

#include "Methods/Adam/Adam.h"
#include "Methods/RmsProp/RmsProp.h"
#include "Methods/Adagrad/Adagrad.h"
#include "Methods/AdaMax/AdaMax.h"
#include "Methods/Adadelta/Adadelta.h"
#include "Methods/Nadam/Nadam.h"
#include "Methods/AMSGrad/AMSGrad.h"

#include <ctime>

/**
 *
 * @struct Flist
 * @brief Структура узла списка функций
 * @var Flist::f
 * Функция для тестирования
 * @var Flist::x_real_min
 * Реальный минимум функции
 * @var Flist::point_on_optimize_params
 * Стартовая точка, используемая для подбора параметров
 * @var Flist::startPoints
 * Множество стартовых точек для теста
 * @var Flist::next
 * @var Flist::prev
 * @var Flist::name
 * Название функции
 *
*/

typedef struct Flist {

  Function f;
  Vector x_real_min;
  Vector point_on_optimize_params;
  vector <Vector> startPoints;
  Flist *next;
  Flist *prev;
  const char *name;

  Flist(Function f, Vector x_real_min, Vector point_on_optimize_params, Flist *prev, Flist *next, const char *name = NULL);

} FLIST;

/**
 *
 * @struct Mlist
 * @brief Структура узла списка методов
 * @var Mlist::m
 * Метод для тестирования
 * @var Mlist::next
 * @var Mlist::prev
 * @var Mlist::name
 * Название метода
*/

typedef struct Mlist {
  Method m;
  Mlist *next;
  Mlist *prev;
  const char *name;

  Mlist(Method m, Mlist *prev, Mlist *next, const char *name = NULL);

} MLIST;

typedef MLIST *MLISTPTR;
typedef FLIST *FLISTPTR;

/**
 *
 *
 * @brief fl_push
 * @usage Для работы с FLIST
 * @param[in] head
 * @param[in] f
 * @param[in] x_real_min
 * @param[in] start_point
 * @param[in] name
 *
 *
*/

void fl_push(FLISTPTR &head,
  Function f,
  Vector x_real_min,
  Vector start_point,
  const char *name = NULL
);

void ml_push(MLISTPTR &head,
  Method m,
  const char *name = NULL
);

FLISTPTR createFullFlist();

MLISTPTR createOurMlist();

void genDots(FLISTPTR testFunctionList);
