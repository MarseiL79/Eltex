#include <stdio.h>
#include "calculator.h"

// Консольные функции
void printMenu(OperationInfo operations[], int op_count) {
    printf("\n=== CALCULATOR ===\n");
    for (int i = 0; i < op_count; i++) {
        printf("%c - %s\n", operations[i].symbol, operations[i].description);
    }
    printf("q - Quit\n");
}

void getArguments(double args[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Enter argument %d: ", i + 1);
        if (scanf("%lf", &args[i]) != 1) {
            printf("Invalid input. Using 0.0\n");
            args[i] = 0.0;
        }
    }
}

void performCalculationGUI(OperationInfo operations[], int op_count) {
    char choice;
    double arguments[100]; 
    int arg_count;

    printf("Choose operation: ");
    scanf(" %c", &choice);

    if (choice == 'q') return;

    Operation selected_operation = NULL;
    int min_args = 0;

    for (int i = 0; i < op_count; i++) {
        if (operations[i].symbol == choice) {
            selected_operation = operations[i].function;
            min_args = operations[i].min_args;
            break;
        }
    }

    if (selected_operation == NULL) {
        printf("Unknown operation!\n");
        return;
    }

    printf("Enter number of arguments (minimum %d): ", min_args);
    if (scanf("%d", &arg_count) != 1) {
        printf("Invalid input for argument count!\n");
        return;
    }

    if (arg_count < min_args) {
        printf("Error: not enough arguments! Minimum %d required.\n", min_args);
        return;
    }

    if (arg_count > (int)(sizeof(arguments)/sizeof(arguments[0]))) {
        printf("Error: too many arguments! Max %zu allowed.\n", sizeof(arguments)/sizeof(arguments[0]));
        return;
    }

    getArguments(arguments, arg_count);
    double result = calculate(selected_operation, arg_count, arguments);

    if (isnan(result)) {
        printf("Error: ");
        if (choice == '/') {
            printf("Division by zero!\n");
        } else if (choice == '^') {
            printf("Power operation requires exactly 2 arguments\n");
        } else {
            printf("Invalid operation!\n");
        }
    } else {
        printf("Result: %.10g\n", result);
    }
}

int main() {
    OperationInfo operations[] = {
        {'+', "Addition", add, 2},
        {'-', "Subtraction", subtract, 2},
        {'*', "Multiplication", multiply, 2},
        {'/', "Division", divide, 2},
        {'^', "Power", power, 2}
    };

    int op_count = sizeof(operations) / sizeof(operations[0]);

    printf("Welcome to Calculator!\n");

    while (1) {
        printMenu(operations, op_count);
        performCalculationGUI(operations, op_count);

        char continue_calc;
        printf("Continue calculations? (y/n): ");
        scanf(" %c", &continue_calc);

        if (continue_calc != 'y' && continue_calc != 'Y') break;
    }

    printf("Calculator closed.\n");
    return 0;
}