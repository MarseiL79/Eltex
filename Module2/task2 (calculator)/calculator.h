#include <math.h>

typedef double (*Operation)(int count, const double args[]);

double add(int count, const double args[]);
double subtract(int count, const double args[]);
double multiply(int count, const double args[]);
double divide(int count, const double args[]);
double power(int count, const double args[]);

double calculate(Operation operation, int arg_count, const double args[]);

typedef struct {
    char symbol;
    char* description;
    Operation function;
    int min_args;
} OperationInfo;
