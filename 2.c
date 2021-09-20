#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char c, name[20] = {};
    int i = 0;

    printf("Hello, enter your name!\n");

    while((c = getchar()) != EOF && i < 10){
        name[i] = c;
        i++;
    }

    printf("Hello,");
    i = 0;
    while(name[i] != '\0'){
        printf("%c", name[i]);
        i++;
    }
    printf("!\n");

    return 0;
}
