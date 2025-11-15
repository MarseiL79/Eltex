#include <stdio.h>
#include "permissions.h"

void printMenu(int* flag) {
    int choice;

    printf("\nCurrent file: \"%s\"\n", filename);
    printf("0. Exit\n");
    printf("1. Select file\n");
    printf("2. Modify file permissions\n");
    printf("3. Display current permissions for file\n");
    printf("Choose option: ");

    scanf("%d", &choice);
    printf("\n");

    switch (choice) {
        case 1:
            select_file();
            if(file_exists) {
                current_file_permissions();
            } 
            break;
        case 2: 
            if(file_exists) {
                modify_file_permissions();
                current_file_permissions();
            } else {
                printf("File with name \"%s\" doesn't exist\n", filename);
            }
            break;
        case 3:
            if(file_exists) {
                current_file_permissions();
            } else {
                printf("File with name \"%s\" doesn't exist\n", filename);
            }
            break;
        default:
            *flag = 0;
            break;
    }
}

int main() {
    int flag = 1;
    do {
        printMenu(&flag);
    } while(flag != 0);

    printf("Terminating program...\n");
    return 0;
}