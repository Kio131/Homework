
# Домашнее задание № 4
###
###
## Задание
###
### Удалить заданную вершину и в модифицированном графе отсортировать вершины по возрастанию степени вершины
###
###
## Описание программы
###
### Программа посимвольно считывает из файла в массив матрицу инцидентности, попутно проверяя правильность формата её ввода. После этого программа выводит изображение графа. После этого она считывает вершину, которую нужно удалить. Удаляет вершину (делая строку в матрице инцидентности невалидной). Затем выводит изображение модифицироваанного графа. После этого программа считает стпень вершины для всех оставшихся вершин, затем отсортировывая массив вершин по степеням вершин. Выводит результат сортировки на экран. 
##
### Программа завершается с кодами:
### 0, если все выполнено успешно;
### -1, если не открылся файл для чтения или записи;
### -2, если файл, куда пользователь ввёл значения, пуст;
### -3, если происходит переполнение; 
### -4, если произошла ошибка с выделением памяти;
### -5, если формат вводимых данных неверный;
### -6, если матрица инцидентности состоит только из 0;
###
### Значение кода завершения можно посмотреть командой:
>  echo $?
###
### Команда для компиляции:
> gcc -I include/ src/main.c src/resources.c -o main.out
### или
> make build
###
### Для выполнении программы необходимо её скомпилировать, затем выполнить файл «a.out» командой:
> ./main.out
### или
> make run
### Команда для компиляции и запуска:
> make
