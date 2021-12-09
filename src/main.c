#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main(void) {

    // Инициализация
    matrix_t matrix = {0};
    
    // Считывание файла
    read_file(&matrix); 
    check_emp(&matrix);

    // Создание и вывод значений для графического изображение графа
    FILE* ft = fopen ("out.gv", "w+"); 
    if (ft == NULL) {
        printf("Не удалось открыть файл для записи значений");
    } else {
        print_file( ft, &matrix, 1);
        fclose(ft);

        // Создание и вывод графического изображение графа
        system("dot -Tpng out.gv -o out.jpg");
        system("wslview out.jpg");
    }

    // Считывание вершины, которую нужно удалить
    printf("Введите номер вершины, которую хотите удалить: ");
    size_t vertex_del;
    if (!scanf("%zu", &vertex_del)) {
        printf("Не удалось считать вершину");
    } else {
        // Удаление вершины
        delet_node(&matrix, vertex_del);

        // Вывод на экран модифицированного графа
        ft = fopen ("out.gv", "w+"); 
        if (ft == NULL) {
            printf("Не удалось открыть файл для записи значений");
            // exit(-1);
        } else {
            print_file( ft, &matrix, 0);
            fclose(ft);

            // Создание и вывод графического изображение модифицированного графа
            system("dot -Tpng out.gv -o out.jpg");
            system("wslview out.jpg");
        }
    }

    // Сортировка вершин
    //   Подсчет степени вершин
    top_t *top = calculate_degree_vertex(&matrix);
    //   Сортировка массива вершин 
    buble_sort(top, matrix.count_line);
    //   Вывод в терминал отсортированного массива вершин
    print_vertex(top, vertex_del, matrix.count_line);

    // Освобождение памяти
    freeing_matrix(&matrix);
    free(top);

    return 0;
}
