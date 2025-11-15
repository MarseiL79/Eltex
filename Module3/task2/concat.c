#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s строка1 строка2 ...\n", argv[0]);
        return 1;
    }
    
    char result[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(result, argv[i]);
        if (i < argc - 1) strcat(result, " ");
    }
    printf("Результат: %s\n", result);
    return 0;
}