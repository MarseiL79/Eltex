#include "calculator.h"

double add(int count, const double args[]) {
    double result = 0.0;
    for (int i = 0; i < count; i++) result += args[i];
    return result;
}

double subtract(int count, const double args[]) {
    if (count <= 0) return NAN;
    double result = args[0];
    for (int i = 1; i < count; i++) result -= args[i];
    return result;
}

double multiply(int count, const double args[]) {
    if (count <= 0) return NAN;
    double result = 1.0;
    for (int i = 0; i < count; i++) result *= args[i];
    return result;
}

double divide(int count, const double args[]) {
    if (count <= 0) return NAN;
    double result = args[0];
    for (int i = 1; i < count; i++) {
        if (args[i] == 0.0) return NAN;
        result /= args[i];
    }
    return result;
}

double power(int count, const double args[]) {
    if (count != 2) return NAN;
    return pow(args[0], args[1]);
}

double calculate(Operation operation, int arg_count, const double args[]) {
    return operation(arg_count, args);
}