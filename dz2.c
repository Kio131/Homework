#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int* read_file(FILE* fp, int* bond, unsigned* count_line, unsigned* count_column);
int check_value(int a);
void check_emp(int* bond, unsigned count_line, unsigned count_column);
void print_file(FILE* ft, int* bond, unsigned count_line, unsigned count_column);
void placing_flags(int* bond, unsigned count_line, unsigned count_column, unsigned i, int* flag);
void print_result(int* flag, unsigned count_line);

int main(void) {

    // Инициализация
    unsigned count_line;
    unsigned count_column;
    unsigned i;
    int* bond;
    
    // Считывание файла
    FILE* fp; 
    if ((fp = fopen ("enter.txt", "r+")) == NULL) {
        printf("Ошибка открытия файла для чтения\n");
        exit(-1);
    }
    bond = read_file(fp, bond, &count_line, &count_column);
    fclose(fp);
    
    check_emp(bond, count_line, count_column);

    // Сщздание и вывод значений для графического изображение графа
    FILE* ft; 
    if ((ft = fopen ("out.gv", "w+")) == NULL) {
        printf("Не удалось открыть файл для записи значений");
        exit(-1);
    }
    print_file( ft, bond, count_line, count_column);
    fclose(ft);

    // Создание и вывод графического изображение графа
    system("dot -Tpng out.gv -o out.jpg");
    system("wslview out.jpg");

    // Проверка графа на связность
    int* flag = (int*) malloc(count_line * sizeof(int));
    for (i = 0; i < count_line; i++) {
        flag[i] = 0;
    }
    i = 0;
    flag[0] = 1; 
    placing_flags(bond, count_line, count_column, i, flag);
    print_result(flag, count_line);

    // Освобождение памяти
    free(bond);
    free(flag);

    return 0;
}

int* read_file(FILE* fp, int* bond, unsigned* count_line, unsigned* count_column) {       // Считывание информации из файла enter.txt
    unsigned i;
    unsigned j;
    
    int c;
    if ((c = (int) fgetc(fp)) == EOF) {
        printf("Файл пуст\n");
        exit(-2);
    }

    for((*count_column) = 0; (c != '\n') && (c != EOF); (*count_column)++) {

        if ((*count_column) == UINT_MAX) {
            printf("Превышен размер вводимых данных\n");
            exit(-3);
        }

        bond = realloc(bond, ((*count_column) + 1) * sizeof(int));

        if (bond == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(-4);
        }
        bond[(*count_column)] = 0;

        if (check_value(c)) {
            printf("Неверный формат ввода\nМатрица инцидентности должна содержать только \"0\" и \"1\" без пробелов\n");
            exit(-5);
        }

        bond[(*count_column)] = c - '0';
        c = (int) fgetc(fp);
    }

    for ((*count_line) = 1; ((c = (int) fgetc(fp)) != EOF); (*count_line)++) {

        if ((*count_line) == UINT_MAX) {
            printf("Превышен размер вводимых данных\n");
            exit(-3);
        }

        bond = realloc(bond, ((*count_column) + 1) * ((*count_line) + 1) * sizeof(int));

        if (bond == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(-4);
        }

        for (j = 0; j < (*count_column); j++){
            bond[(*count_line) * (*count_column) + j] = 0;
        }

        if (bond == NULL) {
            printf("Ошибка выделением памяти");
            exit(-4);
        }

        for(i = 0; (c != '\n') && (c != EOF); i++) {

            if (i > (*count_column)) {
                printf("Неверный формат ввода\n");
                exit(-5);
            }

            if (check_value(c)) {
                printf("Неверный формат ввода\n Матрица инцидентности должна содержать только \"0\" и \"1\"\n");
                exit(-5);
            }

            bond[(*count_line) * (*count_column) + i] = c - '0';
            c = (int) fgetc(fp);
        }

        if (i < (*count_column)) {
                printf("Неверный формат ввода\n");
                exit(-5);
            }
    }

    return bond;
}

int check_value(int a) {                                  // Проверка правильности ввода значений
    if ((a == '1') || (a == '0')) {
        return 0;
    } else {
        return 1;
    }
}

void check_emp(int* bond, unsigned count_line, unsigned count_column) {        // Проверка матрицы на пустоту
    unsigned i;
    unsigned j;

    for (i = 0; i < count_line * count_column; i++) {
        if (!bond[i]){
            ++j;
        }
    }
    if (j == count_line * count_column) {
        printf("Матрица пуста\n");
        exit(-6);
    }
}

void print_file(FILE* ft, int* bond, unsigned count_line, unsigned count_column) {       // Запись значений для вывода графа в файл out.gv  
    unsigned i;
    unsigned j;
    unsigned count_link;
    unsigned loop;

    fprintf(ft, "graph NEW {\n");

    for (j = 0; j < count_column; j++) {
        loop = 0;
        count_link = 0;
        for (i = 0; i < count_line; i++) {
            if (bond[i * count_column + j]) {
                ++count_link;
                if (!loop) {
                    loop = i + 1;
                };
                if (count_link == 2) {
                    fprintf(ft, "\t%d -- %d\n", loop, i + 1);
                } else if (count_link > 2) {                            
                    printf("Неверный формат ввода\nВ каждом столбце матрицы инцидентности не может быть единиц больше двух\n");
                    exit(-5);
                }
            }
        }
        if (count_link == 1) {
            fprintf(ft, "\t%d -- %d\n", loop, loop);
        }
        if (!count_link && !loop) {
            printf("Неверный формат ввода\nВ матрице инцидентности не должно быть пустых столбцов\n");
            exit(-5);
        }
    }
    for (i = 0; i < count_line; i++) {
        count_link = 0;
        for (j = 0; j < count_column; j++) {
            if (bond[i * count_column + j]) {
                ++count_link;
            }
        }
        if (!count_link) {
            fprintf(ft, "\t%d\n", i + 1);
        }
    }

    fprintf(ft, "}");
}

void placing_flags(int* bond, unsigned count_line, unsigned count_column, unsigned i, int* flag) {     // Проход в глубину с расставлением флагов о посещении
    unsigned k;
    unsigned c;
    unsigned save;

    for (k = 0; k < count_column; k++) {
        if (bond[i * count_column + k]) {
            for (c = 0; c < count_line; c++) {
                if (bond[c * count_column + k]) {
                    if (!flag[c]) {
                        flag[c] = 1;
                        save = i;
                        i = c;
                        placing_flags(bond, count_line, count_column, i, flag);
                        i = save;
                    }
                }
            }
        }
    }
}

void print_result(int* flag, unsigned count_line) {        // Вывод результата проверки на связность
    unsigned i;
    unsigned j;
    
    j = 1;
    for (i = 0; i < count_line; i++) {
        j *= flag[i];
    }

    if(j) {
        printf("Граф связный\n");
    } else {
        printf("Граф не связаный\n");
    }
}
