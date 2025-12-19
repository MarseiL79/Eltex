#include <math.h>

double multiply(int count, const double args[]) {
    if (count <= 0) return NAN;
    double result = 1.0;
    for (int i = 0; i < count; i++) result *= args[i];
    return result;
}