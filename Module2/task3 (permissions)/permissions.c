#include <stdio.h>
#include <string.h>
#include <math.h>
#include "permissions.h"

int initial_mask[9] = {0};
int rights_mask[9] = {0};
int who_mask[9] = {0};
int file_exists = 0;
char filename[100] = ""; 
struct stat file_stat;
unsigned int permissions = 0;

void octal_to_bit() {
    for (int i = 8; i >= 0; i--) {
        printf("%d", (permissions >> i) & 1);
        initial_mask[8-i] = (permissions >> i) & 1;
        if (i % 3 == 0 && i > 0) {
            printf(" ");  
        }
    }
    printf("\n");
}

void octal_to_symbol() {
    char bits[] = "rwxrwxrwx";
    for (int i = 8; i >= 0; i--) {
        if ((permissions >> i) & 1) {
            printf("%c", bits[8-i]);
        } else {
            printf("-");
        }
        if (i % 3 == 0 && i > 0) {
            printf(" ");  
        }
    }
    printf("\n");
}

void current_file_permissions() {
    printf("Permissions for file \"%s\"\n", filename);
    printf("Octal: %03o\n", permissions);
    octal_to_bit();
    octal_to_symbol();
    printf("\n");
}

void select_file() {
    printf("Enter filename: ");
    scanf("%99s", filename);

    if (stat(filename, &file_stat) == -1) {
        perror("Error getting file info");
        strcpy(filename, "");
        permissions = 0;
        file_exists = 0;
        return;
    }

    for (int i = 0; i < 9; i++) {
        initial_mask[i] = 0;
        rights_mask[i] = 0;
        who_mask[i] = 0;
    }
    permissions = file_stat.st_mode & 0777;
    file_exists = 1;
}

void from_bit_to_octal() {
    permissions = 0;
    for (int i = 0; i < 9; i++) {
        if (initial_mask[i]) {
            permissions |= (1 << (8 - i));
        }
    }
}

void modify_file_permissions() {
    char symbols[7];
    char operation;

    // Обнуление масок
    for (int i = 0; i < 9; i++) {
        rights_mask[i] = 0;
        who_mask[i] = 0;
    }

    int choice = 0;
    printf("1. Symbol format\n");
    printf("2. Octal format\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    printf("\n");

    if(choice == 1) {
        printf("Format: \"ugo+rwx\" or \"a+rwx\"\n");
        printf("a - all \nu - user, g - group, o - other \nr - read, w - write, x - execute\n");
        printf("Enter what modifications you need: ");
        scanf("%7s", symbols);

        for (int i = 0; symbols[i] != '\0'; i++) {
            switch (symbols[i]) {
                case 'a':
                    for (int j = 0; j < 9; j++) who_mask[j] = 1;
                    break;
                case 'u':
                    for (int j = 0; j < 3; j++) who_mask[j] = 1;
                    break;
                case 'g':
                    for (int j = 3; j < 6; j++) who_mask[j] = 1;
                    break;
                case 'o':
                    for (int j = 6; j < 9; j++) who_mask[j] = 1;
                    break;
                case '+':
                    operation = '+';
                    break;
                case '-':
                    operation = '-';
                    break;
                case '=':
                    operation = '=';
                    break;
                case 'r':
                    for (int j = 0; j < 9; j+=3) rights_mask[j] = 1;
                    break;
                case 'w':
                    for (int j = 1; j < 9; j+=3) rights_mask[j] = 1;
                    break;
                case 'x':
                    for (int j = 2; j < 9; j+=3) rights_mask[j] = 1;
                    break;
                default:
                    printf("Invalid format for permissions modification\n");
                    break;
            }
        }

        for (int i = 0; i < 9; i++) {
            rights_mask[i] = rights_mask[i] & who_mask[i];
        }
        
        printf("\nNew permissions: \n");
        if (operation == '+') {
            for (int i = 0; i < 9; i++) {
                if (who_mask[i]) {
                    initial_mask[i] = initial_mask[i] | rights_mask[i];
                }
            }
        }
        else if (operation == '-') {
            for (int i = 0; i < 9; i++) {
                if (who_mask[i] && rights_mask[i]) {
                    initial_mask[i] = 0;
                }
            }
        }
        else if (operation == '=') {
            for (int i = 0; i < 9; i++) {
                if (who_mask[i] == 1) {
                    initial_mask[i] = rights_mask[i];  
                }
            }
        }
        from_bit_to_octal();
    }

    if(choice == 2) {
        int change = 0;
        printf("Format: \"777\" (octal)\n");
        printf("Enter what modifications you need: ");
        scanf("%o", &change);
        permissions = change;
        printf("New permissions set to: %03o\n\n", permissions);
    }
}