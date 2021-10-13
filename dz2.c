#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int check_value(int a);

int check_con(int* bond, unsigned count_line, unsigned count_column,int* flag);

int main(void) {
    
    FILE* fp; 
    if ((fp = fopen ("enter.txt", "r+")) == NULL) {
        printf("Ошибка открытия файла\n");
        exit(-10);
    }

    int c;
    if ((c = (int) fgetc(fp)) == EOF) {
        printf("Файл пуст\n");
        exit(-6);
    }

    unsigned count_line;
    count_line = 1;
    unsigned count_column;
    count_column = 1;
    unsigned i;
    unsigned j;
    int* bond;


    for(count_column = 0; (c != '\n') && (c != EOF) && (count_column <= UINT_MAX); count_column++) {

        bond = realloc(bond, (count_column + 1) * sizeof(int));

        if (bond == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(-2);
        }
        bond[count_column] = 0;

        if (check_value(c)) {
            printf("Неверный формат ввода\nМатрица инцидентности должна содержать только \"0\" и \"1\" без пробелов\n");
            exit(-3);
        }

        bond[count_column] = c - '0';
        c = (int) fgetc(fp);
    }

    for (count_line = 1; ((c = (int) fgetc(fp)) != EOF) && (count_column < UINT_MAX); count_line++) {

        bond = realloc(bond, (count_column + 1) * (count_line + 1) * sizeof(int));

        if (bond == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(-1);
        }

        for (j = 0; j < count_column; j++){
            bond[count_line * count_column + j] = 0;
        }

        if (bond == NULL) {
            printf("Ошибка с выделением памяти");
            exit(-3);
        }

        for(i = 0; (c != '\n') && (c != EOF); i++) {

            if (i > count_column) {
                printf("Неверный формат ввода\n");
                exit(-3);
            }

            if (check_value(c)) {
                printf("Неверный формат ввода\n Матрица инцидентности должна содержать только \"0\" и \"1\"\n");
                exit(-3);
            }

            bond[count_line * count_column + i] = c - '0';
            c = (int) fgetc(fp);
        }

        if (i < count_column) {
                printf("Неверный формат ввода\n");
                exit(-3);
            }
    }

    fclose(fp);
    j = 0;


    for (i = 0; i < count_line * count_column; i++) {
        if (!bond[i]){
            ++j;
        }
    }
    if (j == count_line * count_column) {
        printf("Матрица пуста\n");
        exit(-7);
    }


    unsigned count_link;
    unsigned loop;

    FILE* ft; 
    if ((ft = fopen ("out.gv", "w+")) == NULL) {
        exit(-4);
    }

    fprintf(ft, "graph NEW {\n");

    unsigned k;

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
                    exit(-7);
                }
            }
        }
        if (count_link == 1) {
            fprintf(ft, "\t%d -- %d\n", loop, loop);
        }
    }
    for (i = 0; i < count_line; i++) {
        count_link = 0;
        for (j = 1; j < count_column; j++) {
            if (bond[i * count_column + j]) {
                ++count_link;
            }
        }
        if (!count_link) {
            fprintf(ft, "\t%d\n", i + 1);
        }
    }

    fprintf(ft, "}");

    fclose(ft);

    system("dot -Tpng out.gv -o out.jpg");
    system("wslview out.jpg");


    int* flag = (int*) malloc(count_line * sizeof(int));
    for (i = 0; i < count_line; i++) {
        flag[i] = 0;
    }
    

    j = i = 0;
    unsigned count_con;
    count_con = 0;

    if(check_con(bond, count_line, count_column, flag)) {
        printf("Граф не связный\n");
    } else {
        printf("Граф связаный\n");
    }

    free(bond);
    return 0;
}

int check_value(int a) {                                  // проверка правильности ввода значений
    if ((a == '1') || (a == '0')) {
        return 0;
    } else {
        return 1;
    }
}

int check_con(int* bond, unsigned count_line, unsigned count_column, int* flag) {
    unsigned j;
    unsigned i;

    for (j = 1; j < count_column; j++) {
        for (i = 0; i < count_line; i++) {
            if (bond[i * count_column + j]) {
                flag[i] = 1;
            }
        }
    }

    for (i = 0; i < count_line; i++) {
        //printf("%d", (int) flag[0]);
        if (!flag[i]) {
            return 1;
        }
    }
    //printf("\n");
    
    //printf("\t\t%u\n", k);

    return 0;
}