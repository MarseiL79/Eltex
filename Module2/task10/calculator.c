#include "calculator.h"

double calculate(Operation operation, int arg_count, const double args[]) {
    return operation(arg_count, args);
}

OperationInfo* load_operations(const char* dir_path, int* op_count) {
    DIR* dir;
    struct dirent* entry;
    OperationInfo* operations = NULL;
    *op_count = 0;
    
    dir = opendir(dir_path);
    if (!dir) {
        printf("Cannot open directory: %s\n", dir_path);
        return NULL;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        char* dot = strrchr(entry->d_name, '.');
        if (dot && strcmp(dot, ".so") == 0) {
            char lib_path[256];
            snprintf(lib_path, sizeof(lib_path), "%s/%s", dir_path, entry->d_name);
            
            void* lib_handle = dlopen(lib_path, RTLD_LAZY);
            if (!lib_handle) {
                printf("Cannot load library %s: %s\n", entry->d_name, dlerror());
                continue;
            }
            
            char func_name[100];
            strncpy(func_name, entry->d_name, dot - entry->d_name);
            func_name[dot - entry->d_name] = '\0';
            
            Operation func = (Operation)dlsym(lib_handle, func_name);
            if (!func) {
                printf("Cannot find function %s in library %s\n", func_name, entry->d_name);
                dlclose(lib_handle);
                continue;
            }
            
            operations = realloc(operations, (*op_count + 1) * sizeof(OperationInfo));
            if (!operations) {
                printf("Memory allocation failed\n");
                dlclose(lib_handle);
                closedir(dir);
                return NULL;
            }
            
            operations[*op_count].symbol = func_name[0];
            operations[*op_count].description = strdup(func_name);
            operations[*op_count].function = func;
            operations[*op_count].min_args = 2;
            operations[*op_count].lib_handle = lib_handle;
            
            (*op_count)++;
        }
    }
    
    closedir(dir);
    return operations;
}

void unload_operations(OperationInfo* operations, int op_count) {
    for (int i = 0; i < op_count; i++) {
        if (operations[i].lib_handle) {
            dlclose(operations[i].lib_handle);
        }
        if (operations[i].description) {
            free(operations[i].description);
        }
    }
    free(operations);
}

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
        printf("Error: Invalid operation!\n");
    } else {
        printf("Result: %.10g\n", result);
    }
}