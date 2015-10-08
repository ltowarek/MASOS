#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <gmpxx.h>

static const bool is_debug = false;

void print(mpq_class &number) {
    std::cout << std::fixed;
    std::cout << number << std::endl;
}

int main(int argc, char** argv) {
	std::cin.sync_with_stdio(false);

    if (argc != 2) {
        std::cout << "Wrong number of parameters!" << std::endl;
        return -1;
    }

    int d = atoi(argv[1]);

    if (is_debug) {
        std::cout << "d: " << d << std::endl;
    }

    // TODO: Set precision to *d*

    // 1 <= n <= 2^24 (16777216)
    int n = 0;
    std::vector<mpq_class> xn;
    mpq_class tmp;
	std::string str;
    while (std::cin >> str) {
        try {
			xn.push_back(mpq_class(str));
		} catch (...) {
			std::cout << "Failed with string: " << str << std::endl;
		}
		n++;
    }

    if (n < 1) {
        std::cout << "No arguments have been read!" << std::endl;
        return -1;
    }

    if (is_debug) {
        std::cout << "n: " << n << std::endl;
    }

    // Mean value
    mpq_class sum;
    for (int i = 0; i < n; ++i) {
        sum += xn[i];
    }
    mpq_class mean = sum / n;
    print(mean);

    // Variance
    mpq_class first;
    mpq_class second;
    for (int i = 0; i < n; ++i) {
        first += (xn[i] * xn[i]);
        second += xn[i];
    }
    first /= n;
    second /= n;
    second *= second;
    mpq_class variance = first - second;
    print(variance);

    // Period
    int p = 0;
    int p_min = n;

    while (++p) {
        bool is_valid = true;
        for (int i = 0; i < n - p; ++i) {
            if (xn[i] != xn[i+p]) {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            p_min = p;
            break;
        }
    }

    std::cout << p_min << std::endl;

    return 0;
}
