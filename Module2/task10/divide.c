#include <math.h>

double divide(int count, const double args[]) {
    if (count <= 0) return NAN;
    double result = args[0];
    for (int i = 1; i < count; i++) {
        if (args[i] == 0.0) return NAN;
        result /= args[i];
    }
    return result;
}