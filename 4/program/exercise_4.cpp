#include <cmath>
#include <cstdlib>
#include <iostream>

static const bool is_debug = true;
static const double a = 1.0;
static const double b = 3.0;

//TODO: Change a and b to 0 and 1

double f(double x) {
    return pow(x, 2) - 5;
}

double df(double x) {
    return 2 * x;
}

double newton(double x, double epsilon) {
    double y = f(x);
    while ((std::abs(y) > epsilon) && (x >= a) && (x <= b)) {
        x = x - y / df(x);
        y = f(x);

        std::cout << x << " " << y << std::endl;
    }

    return x;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Wrong number of parameters!" << std::endl;
        return -1;
    }

    double x0 = atof(argv[1]);
    double epsilon = atof(argv[2]);

    if (is_debug) {
        std::cout << "epsilon: " << epsilon << std::endl;
        std::cout << x0 << " " << f(x0) << std::endl;
    }

    newton(x0, epsilon);

    return 0;
}
