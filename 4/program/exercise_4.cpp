#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

static const bool is_debug = true;
static const double a = 1.0;
static const double b = 3.0;
static const std::string result_file_name = "result.csv";
static const std::string plot_script_file_name = "plot.p";

//TODO: Change a and b to 0 and 1

double f(const double x) {
    return pow(x, 2) - 5;
}

double df(const double x) {
    return 2 * x;
}

double newton(const double x0, const double epsilon, std::ostringstream &result_stream) {
    double x = x0;
    double y = f(x);

    while ((std::abs(y) > epsilon) && (x >= a) && (x <= b)) {
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

void createPlotScript(const std::string &result_file, std::string &script_code) {
    std::ostringstream s;
    s << "set datafile separator \",\"" << std::endl;
    s << "set autoscale" << std::endl;
    s << "set xtic auto" << std::endl;
    s << "set ytic auto" << std::endl;
    s << "set title \"TITLE\"" << std::endl;
    s << "set xlabel \"x\"" << std::endl;
    s << "set ylabel \"y\"" << std::endl;
    s << "set xr [" << a << ":" << b <<"]" << std::endl;
    s << "plot \"" << result_file << "\" notitle with linespoints" << std::endl;
    script_code = s.str();
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

    writeStringToFile(result_file_name, result);

    std::string plot_script;
    createPlotScript(result_file_name, plot_script);

    if (is_debug) {
        std::cout << plot_script << std::endl;
    }

    writeStringToFile(plot_script_file_name, plot_script);

    return 0;
}
