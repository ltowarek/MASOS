#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

static const bool is_debug = false;
static const double a = 0.0;
static const double b = 1.0;
static const std::string result_file_name = "result.csv";
static const std::string plot_script_file_name = "plot.p";

double f(double x, double y)
{
	double r = -2 * x * y;
	return r;
}

double runge(double x, double y, double H)
{
	double K1 = (H * f(x, y));
	double K2 = (H * f((x + 1 / 2 * H), (y + 1 / 2 * K1)));
	double K3 = (H * f((x + 1 / 2 * H), (y + 1 / 2 * K2)));
	double K4 = (H * f((x + H), (y + K3)));
	double r = (y + ((K1 + 2 * K2 + 2 * K3 + K4) / 6));
	return r;
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
	s << "set xr [" << a << ":" << b << "]" << std::endl;
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

	double y = x0;  // y(0) = x0
	double x = a;
	while(x <= b)
	{
		if (is_debug) {
			std::cout << x << " " << y << std::endl;
		}

		result_stream << x << "," << y << std::endl;

		y = runge(x, y, epsilon);
		x += epsilon;
	}

	std::string result = result_stream.str();

	if (is_debug) {
		std::cout << std::endl << result << std::endl;
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
