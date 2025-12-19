#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

typedef double (*Operation)(int count, const double args[]);

typedef struct {
    char symbol;
    char* description;
    Operation function;
    int min_args;
    void* lib_handle;
} OperationInfo;

OperationInfo* load_operations(const char* dir_path, int* op_count);
void unload_operations(OperationInfo* operations, int op_count);
double calculate(Operation operation, int arg_count, const double args[]);

void printMenu(OperationInfo operations[], int op_count);
void getArguments(double args[], int count);
void performCalculationGUI(OperationInfo operations[], int op_count);
