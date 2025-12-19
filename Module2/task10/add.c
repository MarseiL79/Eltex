#include <math.h>

double add(int count, const double args[]) {
    double result = 0.0;
    for (int i = 0; i < count; i++) result += args[i];
    return result;
}