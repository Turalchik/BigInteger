# Длинная арифметика
Реализуйте класс для представления длинных целых чисел (знаковых).

## Требования к реализации
* Название класса: `BigInteger`
* Заголовочный файл: `biginteger.h`
* Файл реализации: `biginteger.cpp`

Для класса BigInteger реализовать:
* Конструктор по умолчанию (инициализирует число нулём).
* Конструкторы от числовых типов.
* Явный конструктор от `std::string`.
* Конструктор копирования.
* Конструктор перемещения.
* Операторы:
  * присваивания копированием,
  * присваивания перемещением,
  * сравнения.
* Арифметические операции: сложение, вычитание, умножение, деление, унарный минус, унарный плюс, инкременты и декременты.
* Внешнюю функцию `std::string to_string(const BigInteger &)`.

Требования:
* Разряды числа должны представляться как минимум 32-битными числами, все биты в их представлении должны использоваться.
* Пользоваться сторонними библиотеками при выполнении задания запрещено. Исключение `std::string` (при реализации to_string и конструктора) и `std::numeric_limits`.

## Дополнительная литература
* Кнут Д.Э. Искусство программирования. Том 2. Получисленные алгоритмы. Глава 4. Секция 4.3.
* [Длинная арифметика](https://ru.wikipedia.org/wiki/Длинная_арифметика)
* [Алгоритм деления](https://ru.wikipedia.org/wiki/Алгоритм_деления)
* ["Modern Computer Arithmetic"](https://members.loria.fr/PZimmermann/mca/mca-0.5.pdf)
* ["Multiple-Length Division Revisited: A Tour of the Minefield"](https://surface.syr.edu/cgi/viewcontent.cgi?article=1162&context=eecs_techreports) 
# BigInteger
