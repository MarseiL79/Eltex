#include <math.h>

double subtract(int count, const double args[]) {
    if (count <= 0) return NAN;
    double result = args[0];
    for (int i = 1; i < count; i++) result -= args[i];
    return result;
}