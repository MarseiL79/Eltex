#include "calculator.h"

int main() {
    int op_count = 0;
    OperationInfo* operations = load_operations("./libs", &op_count);
    
    if (!operations || op_count == 0) {
        printf("No operations loaded. Creating default operations.\n");
        
        operations = malloc(5 * sizeof(OperationInfo));
        if (!operations) {
            printf("Memory allocation failed\n");
            return 1;
        }
        
        operations[0] = (OperationInfo){'+', strdup("Addition"), NULL, 2, NULL};
        operations[1] = (OperationInfo){'-', strdup("Subtraction"), NULL, 2, NULL};
        operations[2] = (OperationInfo){'*', strdup("Multiplication"), NULL, 2, NULL};
        operations[3] = (OperationInfo){'/', strdup("Division"), NULL, 2, NULL};
        operations[4] = (OperationInfo){'^', strdup("Power"), NULL, 2, NULL};
        op_count = 5;
        
        printf("Please create .so libraries in ./libs directory\n");
    }
    
    printf("Welcome to Calculator! Loaded %d operations.\n", op_count);

    while (1) {
        printMenu(operations, op_count);
        performCalculationGUI(operations, op_count);

        char continue_calc;
        printf("Continue calculations? (y/n): ");
        scanf(" %c", &continue_calc);

        if (continue_calc != 'y' && continue_calc != 'Y') break;
    }

    printf("Calculator closed.\n");
    unload_operations(operations, op_count);
    
    return 0;
}