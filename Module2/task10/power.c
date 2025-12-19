#include <math.h>

double power(int count, const double args[]) {
    if (count != 2) return NAN;
    return pow(args[0], args[1]);
}