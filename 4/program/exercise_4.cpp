#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

static const bool is_debug = true;
static const double a = 1.0;
static const double b = 3.0;

//TODO: Change a and b to 0 and 1

double f(const double x) {
    return pow(x, 2) - 5;
}

double df(const double x) {
    return 2 * x;
}

double newton(const double kX, const double kEpsilon, std::ostringstream &result_stream) {
    double x = kX;
    double y = f(x);

    while ((std::abs(y) > kEpsilon) && (x >= a) && (x <= b)) {
        if (is_debug) {
            std::cout << x << " " << y << std::endl;
        }

        result_stream << x << "," << y << std::endl;

        x = x - y / df(x);
        y = f(x);
    }

    return x;
}

void writeStringToFile(const std::string &file_name, const std::string &data) {
    std::ofstream f;
    f.open(file_name.c_str());
    f << data;
    f.close();
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
        std::cout << "x0: " << x0 << std::endl;
    }

    std::ostringstream result_stream;
    newton(x0, epsilon, result_stream);
    std::string result = result_stream.str();

    if (is_debug) {
        std::cout << result << std::endl;
    }

    writeStringToFile("result.csv", result);

    return 0;
}
