#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s число1 число2 ...\n", argv[0]);
        return 1;
    }
    
    int max = INT_MIN;
    for (int i = 1; i < argc; i++) {
        int num = atoi(argv[i]);
        if (num > max) max = num;
    }
    printf("Максимальное число: %d\n", max);
    return 0;
}