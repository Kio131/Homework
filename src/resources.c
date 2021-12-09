#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>

#include "utils.h"

void print(matrix_t matrix) {
    for (size_t i = 0; i < matrix.count_line; i++) {
        for (size_t j = 0; j < matrix.count_column; j++) {
            printf("%3d", matrix.bond[i * matrix.count_column + j]);
        }
        printf("\n");
    }
}

void read_file(matrix_t *matrix) {       // Считывание информации из файла enter.txt
    unsigned i;
    unsigned j;
    int *tmp = NULL;

    FILE* fp = fopen ("enter.txt", "r+"); 
    if (fp == NULL) {
        printf("Ошибка открытия файла для чтения\n");
        exit(-1);
    }
    
    int c;
    if ((c = (int) fgetc(fp)) == EOF) {
        printf("Файл пуст\n");
        exit(-2);
    }

    for(matrix->count_column = 0; (c != '\n') && (c != EOF); (matrix->count_column)++) {

        if (matrix->count_column == UINT_MAX) {
            printf("Превышен размер вводимых данных\n");
            exit(-3);
        }

        tmp = realloc(matrix->bond, (matrix->count_column + 1) * sizeof(int));
        if (tmp == NULL) {
            printf("Ошибка выделения памяти\n");
            free(matrix->bond);
            exit(-4);
        } else {
            matrix->bond = tmp;
        }

        matrix->bond[matrix->count_column] = 0;

        if (check_value(c)) {
            printf("Неверный формат ввода\nМатрица инцидентности должна содержать только \"0\" и \"1\" без пробелов\n");
            exit(-5);
        }

        matrix->bond[matrix->count_column] = c - '0';
        c = (int) fgetc(fp);
    }

    tmp = NULL;

    for (matrix->count_line = 1; ((c = (int) fgetc(fp)) != EOF); (matrix->count_line)++) {

        if (matrix->count_line == UINT_MAX) {
            printf("Превышен размер вводимых данных\n");
            exit(-3);
        }

        tmp = realloc(matrix->bond, (matrix->count_column + 1) * (matrix->count_line + 1) * sizeof(int));
        if (tmp == NULL) {
            printf("Ошибка выделения памяти\n");
            free(matrix->bond);
            exit(-4);
        } else {
            matrix->bond = tmp;
        }

        for (j = 0; j < matrix->count_column; j++){
            matrix->bond[matrix->count_line * matrix->count_column + j] = 0;
        }

        for(i = 0; (c != '\n') && (c != EOF); i++) {
            if (i > matrix->count_column) {
                printf("Неверный формат ввода\n");
                exit(-5);
            }

            if (check_value(c)) {
                printf("Неверный формат ввода\n Матрица инцидентности должна содержать только \"0\" и \"1\"\n");
                exit(-5);
            }

            matrix->bond[matrix->count_line * matrix->count_column + i] = c - '0';
            c = (int) fgetc(fp);
        }
    }

    fclose(fp);
}

int check_value(int a) {                                  // Проверка правильности ввода значений
    if ((a == '1') || (a == '0')) {
        return 0;
    } else {
        return 1;
    }
}

void check_emp(matrix_t *matrix) {        // Проверка матрицы на пустоту
    unsigned i;
    unsigned j;

    for (i = 0; i < matrix->count_line * matrix->count_column; i++) {
        if (!matrix->bond[i]){
            ++j;
        }
    }
    if (j == matrix->count_line * matrix->count_column) {
        printf("Матрица пуста\n");
        exit(-6);
    }
}

void print_file(FILE* ft, matrix_t *matrix, bool validatoin) {       // Запись значений для вывода графа в файл out.gv  
    unsigned i;
    unsigned j;
    unsigned count_link;
    unsigned loop;

    fprintf(ft, "graph NEW {\n");

    for (j = 0; j < matrix->count_column; j++) {
        loop = 0;
        count_link = 0;
        for (i = 0; i < matrix->count_line; i++) {
            if (matrix->bond[i * matrix->count_column + j] == 1) {
                ++count_link;
                if (!loop) {
                    loop = i + 1;
                };
                if (count_link == 2) {
                    fprintf(ft, "\t%d -- %d\n", loop, i + 1);
                } else if ((count_link > 2) && (validatoin)) {                            
                    printf("Неверный формат ввода\nВ каждом столбце матрицы инцидентности не может быть единиц больше двух\n");
                    exit(-5);
                }
            }
        }
        if (count_link == 1) {
            fprintf(ft, "\t%d -- %d\n", loop, loop);
        }
        if (!count_link && !loop && validatoin) {
            printf("Неверный формат ввода\nВ матрице инцидентности не должно быть пустых столбцов\n");
            exit(-5);
        }
    }
    for (i = 0; i < matrix->count_line; i++) {
        count_link = 0;
        for (j = 0; j < matrix->count_column; j++) {
            if (matrix->bond[i * matrix->count_column + j] > 0) {
                ++count_link;
            } else if (matrix->bond[i * matrix->count_column + j] == -1) {
                --count_link;
            }
        }

        if (count_link == 0) {
            fprintf(ft, "\t%d\n", i + 1);
        }
    }

    fprintf(ft, "}");
}

void freeing_matrix(matrix_t *matrix) {         // Освобождение памяти, занимаемой матрицей
    matrix->count_column = 0;
    matrix->count_line = 0;
    free(matrix->bond);
    matrix->bond = NULL;
}

void delet_node(matrix_t *matrix, size_t node_del) {        // Удаление вершины
    if (--node_del > matrix->count_line) {
        printf("Некорректное значение вершины\n");
        return;
    }
    matrix_t new_matrix = {0};
    new_matrix.bond = (int*) calloc(matrix->count_column * matrix->count_line + 1, sizeof(int));
    for (size_t i = 0; i < matrix->count_line; i++) {
        bool empty_line = false;
        if (i == node_del) {
            empty_line = true;
        }
        int count = 0;
        for (size_t j = 0; j < matrix->count_column; j++) {
            bool connection = false;
            for (size_t k = 0; k < matrix->count_line; k++) {
                if (k != i) {
                    if (matrix->bond[k * matrix->count_column + j]) {
                        if (k == node_del) {
                            connection = true;
                        }
                    }
                }
            }
            if (!connection) {
                if (empty_line) {
                    new_matrix.bond[new_matrix.count_line * new_matrix.count_column + count++] = -1;
                } else {
                    new_matrix.bond[new_matrix.count_line * new_matrix.count_column + count++] =
                        matrix->bond[i * matrix->count_column + j];
                }
            }
        }
        if (!new_matrix.count_column) {
            new_matrix.count_column = count;
        }
        ++new_matrix.count_line;
        count = 0;
    }
    freeing_matrix(matrix);
    *matrix = new_matrix;
}

top_t *calculate_degree_vertex(matrix_t *matrix) {      // Расчет степени вершины для каждой из вершин
    top_t *top = (top_t*)calloc(matrix->count_line, sizeof(top_t));
    for (size_t i = 0; i < matrix->count_line; i++) {
        top[i].top = i + 1;
    }
    for (size_t i = 0; i < matrix->count_column; i++) {
        size_t vertex_1 = 0;
        size_t vertex_2 = 0;
        for (size_t j = 0; j < matrix->count_line; j++) {
            if (matrix->bond[j * matrix->count_column + i] == 1) {
                if (vertex_1) {
                    vertex_2 = j + 1;
                } else {
                    vertex_1 = j + 1;
                }
            }
        }
        if (vertex_1 && !vertex_2) {
            top[vertex_1 - 1].rank += 2;
        } else if (vertex_1 && vertex_2) {
            ++(top[vertex_1 - 1].rank);
            ++(top[vertex_2 - 1].rank);
        }
    }

    return top;
}

void buble_sort(top_t *top, size_t count_vertex) {      // Сортировка пузырьком для стпеней вершин
    for (size_t count_iter = count_vertex - 1; count_iter > 0; count_iter--) {
        for (size_t i = 0; i < count_iter; i++) {
            if (top[i].rank > top[i + 1].rank) {
                top_t tmp = top[i + 1];
                top[i + 1] = top[i];
                top[i] = tmp;
            }
        }
    }
}

void print_vertex(top_t *top, size_t vertex_del, size_t count_vertex) {     // Вывод отсортированных вершин
    printf("Отсортированные вершины по возрастанию степени вершины:\n");
    for (size_t i = 0; i < count_vertex; i++) {
        if (top[i].top != vertex_del) {
            printf("%zu - %zu\n", top[i].top, top[i].rank);
        }
    }
}
