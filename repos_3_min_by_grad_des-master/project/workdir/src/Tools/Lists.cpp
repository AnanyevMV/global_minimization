#include "Lists.h"


// Конструктор элемента списка методов
Mlist::Mlist(Method m, Mlist *prev, Mlist *next, const char *name) {
  this->m = m;
  this->name = name;
  this->prev = prev;
  this->next = next;
}

// Конструктор элемента списка функций
Flist::Flist(Function f, Vector x_real_min, Vector point_on_optimize_params, Flist *prev, Flist *next, const char *name) {
  this->f = f;
  this->x_real_min = x_real_min;
  this->point_on_optimize_params = point_on_optimize_params;
  this->name = name;
  this->prev = prev;
  this->next = next;
  this->startPoints = { point_on_optimize_params };
}

// Пуш элемент в список методов хэд
void ml_push(MLISTPTR &head, Method m, const char *name) {
  MLISTPTR ptr = head;

  if (!ptr) {
    head = new MLIST(m, NULL, NULL, name);
  } else {

    while (ptr->next)
      ptr = ptr->next;

    ptr->next = new MLIST(m, ptr, NULL, name);
  }
}

// Пуш элемент в список функций хэд
void fl_push(FLISTPTR &head, Function f, Vector x_real_min, Vector point_on_optimize_params, const char *name) {
  FLISTPTR ptr = head;

  if (!ptr) {
    head = new FLIST(f, x_real_min, point_on_optimize_params, NULL, NULL, name);
  } else {

    while (ptr->next)
      ptr = ptr->next;

    ptr->next = new FLIST(f, x_real_min, point_on_optimize_params, ptr, NULL, name);
  }
}

// Заполнение списка методов
MLISTPTR createOurMlist() {

  MLISTPTR head = NULL;

  ml_push(head, Adam, "Adam");
  ml_push(head, RmsProp, "RMSProp");
  ml_push(head, Adagrad, "AdaGrad");

  ml_push(head, AdaMax, "AdaMax");
  ml_push(head, Adadelta, "Adadelta");

  ml_push(head, Nadam, "Nadam");
  ml_push(head, AMSGrad, "AMSGrad");

  return head;

}

void genDots(FLISTPTR testFunctionList) {
  // Список функций для внешней прогонки
  FLISTPTR f = testFunctionList;
  // Список функций для внешней прогонки
  FLISTPTR fptr = testFunctionList;
  // Список функций для внутренней прогонки
  FLISTPTR fptr1 = testFunctionList;

  // Вектор векторов новой стартовой точки
  vector<Vector> X;

  // Значение текущего аргумента для минимума
  Real swapMin;
  // Значение текущего аргумента для максимума
  Real swapMax;
  // Минимальное значение для аргумента
  Real minS;
  // Максимальное значение для аргумента
  Real maxS;
  // Расстояние между точками
  Real distance;
  // Сгенерированное число
  Real randomer;
  // Инициализация генератора
  srand(time(0));
  // Выполняем, пока есть функция
  while (fptr) {
    // Объявление первого элемента в векторе
    X = { {} };
    // Генерируем 100 точек
    for (int j = 0; j < 100; j++) {
      // Покомпонентно ищем минимальный и макимальное значение
      for (int i = 0; i < (int)fptr->x_real_min.size(); i++) {
        swapMin = fptr->x_real_min[i];
        swapMax = fptr->x_real_min[i];
        // Для текущего минима и максимума ищем реальные мин. и макс. значения
        while (fptr1) {
          if (swapMin > fptr1->x_real_min[i])
            swapMin = fptr1->x_real_min[i];
          if (swapMax < fptr1->x_real_min[i])
            swapMax = fptr1->x_real_min[i];
          // Сохроняем найденные значения
          minS = swapMin;
          maxS = swapMax;
          fptr1 = fptr1->next;
        }
        // Вычисляем расстояние
        distance = maxS - minS;
        // Правый конец отрезка генерации
        maxS = swapMax + distance;
        // Левый отрезок генерации
        minS = swapMin - distance;
        // Генерируем число
        randomer = minS + rand() % int((maxS - minS + 1));
        // Записываем полученный результат
        X[j].push_back(randomer);
      }
      // Передаем внутреннему циклу список функции для последующей проверки
      fptr1 = testFunctionList;
      X.push_back({});
    }
    X.pop_back();
    // Смотрим полученные стартовые точки
    //for (int i = 0; i < X.size(); i++)
    //  cout << X[i] << "  ";
    //cout << endl;
    //cout << X.size() << endl;
    // Передаем вектор векторов стартовых точек структуре с функциями
    fptr->startPoints = X;
    // Очищаем вектор векторов стартовых точек для последующего использования
    X.clear();
    // Выводим на экран результат о том, что мы завершили работать с текущей функцией
    //cout << "\t\t\t-----------------------------" << endl;
    //cout << "\t\t\tWe are finish for function: " << fptr->name << endl;
    //cout << "\t\t\t-----------------------------" << endl;
    // Переходим к следующей функции
    fptr = fptr->next;
  }
}



// Заполнение списка функций (полного)
FLISTPTR createFullFlist() {

  FLISTPTR head = NULL;
  fl_push(head, f1, { -1, -1 }, { -1, -2 }, "f1");
  fl_push(head, f2, { Real(-46) / 47, Real(-106) / 47 }, { -1, 2 }, "f2");
  fl_push(head, f3, { Real(-19) / 2, -5 }, { -2, 1 }, "f3");
  //fl_push(head, f4, { Real(-77900) / 39, Real(-14220) / 13 }, { -1, -1 }, "f4"));
  fl_push(head, f5, { -0.25, -0.25, -0.25 }, { -3, -3, -3 }, "f5");
  fl_push(head, f6, { -0.582109, 0.525995 }, { -2, -0 }, "f6");
  fl_push(head, f7, { -1.08789, -1.00318 }, { -1, -2 }, "f7");
  fl_push(head, f8, { -1, -1 }, { -3, -3 }, "f8");
  fl_push(head, f9, { -1.34777, -1.34777 }, { -1, 3 }, "f9");
  fl_push(head, f10, { -1, -1 }, { -3, 3 }, "f10");
  fl_push(head, f11, { -3.77931, -3.28319 }, { -2, -1 }, "f11");
  fl_push(head, f12, { 1,2,2,3 }, { -1, -1, -3, -2 }, "f12");
  fl_push(head, f13, { -M_PI, 12.275 }, { -1, -3 }, "f13");
  fl_push(head, f14, { -0.25988392,-0.20213194 }, { -1, -0 }, "f14");
  fl_push(head, f15, { 0, 0 }, { 2, -0 }, "f15");
  fl_push(head, f16, { -1.39325,  0.00000 }, { -0, 0 }, "f16");
  fl_push(head, f17, { 0, 0 }, { -2, -2 }, "f17");
  fl_push(head, f18, { -0.029896, 0 }, { -0, 1 }, "f18");
  fl_push(head, f19, { 3, 0.5 }, { 0, -1 }, "f19");
  fl_push(head, f20, Vector(4, 1), { 1, -0, 0, -1 }, "f20");
  fl_push(head, f21, { 1, 1 }, { -2, -1 }, "f21");
  fl_push(head, f22, { 1, 1 }, { -1, -2 }, "f22");
  fl_push(head, f23, { 1, 1 }, { 3, -3 }, "f23");
  fl_push(head, f24, { 0, 0, 0, 0 }, { -3, 1, -2, -1 }, "f24");
  fl_push(head, f25, { 0, 1, 1, 1 }, { -1, -2, -0, -3 }, "f25");
  fl_push(head, f26, { 0, 0 }, { -2, -0 }, "f26");
  fl_push(head, f27, { 1, 1 }, { -1, -2 }, "f27");
  fl_push(head, f28, { 1, 1 }, { -3, -3 }, "f28");
  fl_push(head, f29, Vector(2, 0), { -3, 2 }, "f29");
  fl_push(head, f30, { 0,0 }, { -1, -1 }, "f30");
  fl_push(head, f31, { 0,-1 }, { -3, -3 }, "f31");
  fl_push(head, f32, { -10, 1 }, { -3, -2 }, "f32");
  fl_push(head, f33, { 0, 0 }, { -1, -3 }, "f33");
  fl_push(head, f34, { -10, 0 }, { 0, -1 }, "f34");
  fl_push(head, f35, { 0, 0 }, { 1, -2 }, "f35");
  fl_push(head, f36, { 1.464, -2.506 }, { -1, -3 }, "f36");
  fl_push(head, f37, { 0, 0 }, { -0, 3 }, "f37");
  fl_push(head, f38, { -0.0898, 0.7126 }, { -0, -0 }, "f38");
  fl_push(head, f39, { -M_PI, 12.275 }, { -3, -3 }, "f39");
  fl_push(head, f40, { -3.2, 12.53 }, { -0, -1 }, "f40");
  fl_push(head, f41, { 0, 0 }, { -1, -3 }, "f41");
  fl_push(head, f42, { 0, 0 }, { -1, -1 }, "f42");
  fl_push(head, f43, { 0, 0 }, { 1, 3 }, "f43");
  fl_push(head, f44, { -1.582142172055011, -3.130246799635430 }, { -1, -0 }, "f44");
  fl_push(head, f45, { 4,-1 }, { -0, -2 }, "f45");
  fl_push(head, f46, { 3.40919, -2.17143 }, { -3, -3 }, "f46");
  fl_push(head, f47, { -1.23729,0 }, { 2, 3 }, "f47");
  fl_push(head, f48, { 0,0 }, { -0, -1 }, "f48");
  fl_push(head, f49, { 1,1 }, { -2, -2 }, "f49");
  fl_push(head, f50, { 2, -3 }, { -1, -0 }, "f50");

  return head;
}
