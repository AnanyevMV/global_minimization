# Тестирование с помошью Wolfram

Для проверки правильности работы программы используется Wolfram.
Неприводимость многочлена в Wolfram можно проверить с помошью функции `IrreduciblePolynomialQ`,
примитивность - с помощью функции `PrimitivePolynomialQ`.
Здесь необходимо упомянуть, что проверка на примитивность в Wolfram работает корректно
только для приведённых многочленов (со старшим коэффициентом равным единице),
т.к. например над полем GF[3] проверка многочлена `2 + 2x + x^2` на примитивность
в Wolfram возвращает True, но при это для многочлена `2 * (2 + 2x + x^2) = 1 + x + 2x^2`
аналогичная проверка даёт ответ False, хотя умножение на коэффициент никак не влияет
на примитивность многочлена. Таким образом, тестирование проводилось только для
приведённых многочленов.

В результате работы тестовой программы выводятся следующие данные:
- результат проверки на неприводимость
- результат проверки на примитивность
- основание поля Галуа
- проверяемый многочлен

Все эти данные копируются и вставляются в файл [output](output) (без расширения!).
После чего необходимо запустить Wolfram и выполнить программу проверки.

Бесплатную версию Wolfram можно получить [здесь](https://www.wolfram.com/engine/).
При первом запуске потребуется ввести Wolfram ID, его можно получить
[здесь](https://www.wolfram.com/engine/free-license/).

Программа проверки состоит из следующих строк:
- `file = OpenRead["output"]` - открытие файла с входными данными на чтение,
потребуется указать полный путь к файлу.
- `f[irr_, prim_, m_, p_] := irr == Boole[IrreduciblePolynomialQ[Dot[Power[x, Range[0, Length[p] - 1]], p], Modulus -> m]] && prim == Boole[PrimitivePolynomialQ[Dot[Power[x, Range[0, Length[p] - 1]], p], Modulus -> m]]` - 
собственно функция проверки, эта функция принимает на вход данные в том же порядке, что они
выби выведены, после чего генерируется вектор вида `{1, x, x^2, ...}` для всех степеней, входящих
в многочлен, этот вектор умножается на вектор коэффициентов, подаваемый на вход функции,
после чего они скалярно умножаются и получается многочлен в стандартной записи, после чего
он подаётся на вход функциям проверки на неприводимость и примитивность и результат сравнивается
с ожидаемым, если результаты совпали - выводится True, иначе - False
- `While[Not[(irr = Read[file, Number]) === EndOfFile], prim = Read[file, Number]; m = Read[file, Number]; p = Read[StringToStream[#], Number] & /@ StringSplit[StringTrim[ReadLine[file], ("{" | "}" | " ") ...], ", "]; Print[f[irr, prim, m, p]]]` - 
функция, в цикле считывающая файл построчно, очищающая данные и передающая их в приведённую выше
функцию проверки, а затем печатающая полученный результат
- `Close[file]` - закрытие файла

Все эти команды потребуется вручную скопировать в Wolfram и проверить, что результат
соответствует ожидаемому - выводится колонка True. 
