# RectanglePacker

## Description
RectanglePacker is a module for solving the NP-hard packing problem: place the different rectangles in the containers.

![Example 1](https://github.com/sci-t/space/blob/master/1.jpg?raw=true)

## Details
In the present algorithm the classical ideas of packing algorithms optimized for the special case with all possible rotations.

The characteristic results obtained by the external rendering module illustrated by the present pictures.

***

![Example 2](https://github.com/sci-t/space/blob/master/2.jpg?raw=true)

— all specified rectangles placed to one container (another similar solution present at the start)

***

![Example 0](https://github.com/sci-t/space/blob/master/0.jpg?raw=true)

— specified rectangles need three containers to be placed inside

***
***

## Основная идея
Используем предварительную обработку данных для повышения плотности упаковки. В первую очередь, сортируем прямоугольники со стороной, большей, чем стороны контейнера, т.к. в противном случае в таких контейнерах остаётся много свободного места. Такие прямоугольники требуют поворота. При установке первого прямоугольника выбирается "горизонт", по которому устанавливаются и остальные.

Далее используется несколько преобразованный от классического способ расстановки по полкам: вместо уровней используются углы для позиционирования, т.к. это позволяет корректно размещать прямоугольники в случаях, когда угол для размещения острый. Отсюда может быть реализован ряд эвристических алгоритмов с разной степенью детализации в зависимости от набора данных для заполнения свободных участков наиболее эффективным образом.

## Ограничения
Программа реализована в виде приложения командной строки (отрисовка осуществляется внешним модулем).
* Входные данные - массив 2*n чисел:
    * первая строка - размеры контейнеров: ширина, высота;
    * остальные строки - размеры прямоугольников: ширина, высота.
* Выходные данные:
    * первая строка - количество контейнеров и их размеры;
    * остальные строки - положение прямоугольников: номер контейнера и координаты углов.