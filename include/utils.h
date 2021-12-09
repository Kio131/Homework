#include <stdio.h>
#include "stdbool.h"

#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

typedef struct {
    int *bond;
    size_t count_line;
    size_t count_column;
} matrix_t;

typedef struct {
    size_t top;
    size_t rank;
} top_t;

void read_file(matrix_t *matrix);
int check_value(int a);
void check_emp(matrix_t *matrix);
void print_file(FILE* ft, matrix_t *matrix, bool validatoin);
void delet_node(matrix_t *matrix, size_t node_del);
void freeing_matrix(matrix_t *matrix);
void print(matrix_t matrix);
top_t *calculate_degree_vertex(matrix_t *matrix);
void buble_sort(top_t *top, size_t count_vertex);
void print_vertex(top_t *top, size_t vertex_del, size_t count_vertex);

#endif  // INCLUDE_UTILS_H