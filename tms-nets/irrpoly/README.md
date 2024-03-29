# Проверка многочленов на неприводимость и примитивность над заданным полем Галуа

## Требования
Для работы программы необходимо наличие:
- компилятора с поддержкой C++ 17
- Cmake 3.8 для сборки проекта
- 64-битного процессора с как минимум двумя ядрами

При сборке данного проекта проверяется наличие библиотеки POSIX threads, в случае наличия
программа с ней линкуется, в противном случае для многопоточности используются потоки `std::thread`.

## Состав
Данная программа состоит из нескольких классов:
- `gf<uint32_t P>` - класс чисел над полем Галуа заданной характеристики P
- `polynomial<typename T>` - класс арифметики многочленов
- `polynomialgf<uint32_t P>` - многочлены над полем Галуа, методы для их проверки на
неприводимость и примитивность
- `checker<typename value_type, typename result_type>` - реализует многопоточность,
принимает на вход три функции:
  - `input_func` - генерирует входные данные для `check_func`
  - `check_func` - функция, работу которой и требуется распараллелить
  - `callback_func` - функция, обрабатывающая результат работы `check_func`
  
Всё необходимое для проверки многочленов на неприводимость и примитивность в несколько потоков
погружено в `namespace multithread`, кроме того, весь код заголовочных файлов помещён в
`namespace irrpoly`.

В файле [polynomialgf.hpp](include/irrpoly/polynomialgf.hpp) реализованы следующие алгоритмы:
- `gcd` - поиск наибольшего общего делителя двух многочленов
- `is_irreducible_berlekamp` - проверка многочлена на неприводимость методом Берлекампа
- `is_irreducible_rabin` - проверка многочлена на неприводимость методом Рабина
- `is_primitive_definition` - проверка многочлена на примитивность по определению

## Документация
Описание кода программы может быть найдено в папке [docs](docs) или на соответствующей странице
[GitHub Pages](https://irreducible-polynoms.github.io/irrpoly/html/).

Сборка проекта и обновление документации может выполняться с использованием [Makefile](tools/Makefile).
Доступные команды:
- `make build` - компиляция всех исполняемых файлов, они будут помещены в папки
с исходными файлами в поддирректории `bin/debug/` и `bin/release/`
- `make docs` - перегенерация документации (требуется установленный Doxygen)

## Использование
Для использования программы требуется определиться с тем, какие именно многочлены нужно получить:
- характеристика поля Галуа
- степень многочленов
- их количество
- неприводимость или примитивность требуется проверять
- порядок перебора многочленов

После чего требуется необходимым образом модифицировать функцию-шаблон в примере
[general](examples/general/main.cpp). Скорее всего потребуется реализовать собственный
неслучайный способ подбора многочленов на проверку.

Для использования кода в других проектах рекомендуется использовать Git Submodules
([документация тут](https://git-scm.com/book/en/v2/Git-Tools-Submodules)).

Единственный заголовочный файл, который потребуется подключить для использования данного кода - 
`#include "irrpoly/polynomialgf.hpp"` (предполагается, что ваш код расположен в корневой папке
репозитория, данный проект помещён в виде Git Submodule в папку `irrpoly` и добавлена
команда Cmake `include_directories("${CMAKE_SOURCE_DIR}/irrpoly/include")`).

## Тестирование
Читайте [здесь](tests/wolfram/README.md).
