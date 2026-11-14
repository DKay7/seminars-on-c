# Примеры семинара 2

Сюда перенесены примеры из `seminar-2` и `seminar-3`.
Комментарии и сообщения в программах написаны на английском.

- `division-example.c` — целочисленное деление и разворот цифр числа.
- `fp/` — числа с плавающей точкой; две CMake-цели для разной точности вывода.
- `controls/` — `if/else`, ошибка `if (cond); { ... }`, `switch` и области видимости.
- `loops/loop-examples.c` — пустые части заголовка и тело, несколько переменных,
  оператор запятая и присваивание в условии; `while` и `do-while`,
  различия проверки условия и подсчёт цифр нуля.
- `loops/do-while-macro.c` — многооператорный макрос с `do { ... } while (0)`,
  безопасный для использования в `if/else` с точкой с запятой после вызова.
- `out-params/`, `gcd/`, `arrays/` — примеры из семинара 3.

Сборка всех C-примеров из корня репозитория:

```sh
cmake -S seminar-2 -B /tmp/seminar-2-build
cmake --build /tmp/seminar-2-build
/tmp/seminar-2-build/examples/controls/if-else-example
/tmp/seminar-2-build/examples/controls/switch-example
/tmp/seminar-2-build/examples/loops/loop-examples
/tmp/seminar-2-build/examples/loops/do-while-macro
```

В `if-else-example.c` точка с запятой после `if` оставлена специально;
предупреждения компилятора ожидаемы. В `fp-example.c` сохранён учебный
закомментированный `assert`; предупреждение о неиспользуемой `EPS` ожидаемо.

Python-пример: `python3 seminar-2/examples/fp/fp-example.py`.
