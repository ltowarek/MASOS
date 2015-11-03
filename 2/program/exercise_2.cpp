#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <gmpxx.h>
#include <iomanip>

static const bool is_debug = false;

mpq_class parseFraction(const std::string &fraction) {
    std::size_t slash_position = fraction.find("/");
    mpz_class numerator = mpz_class(fraction.substr(0, slash_position));
    mpz_class denominator = mpz_class(fraction.substr(slash_position + 1));
    return mpq_class(numerator, denominator);
}

std::string fixTrailingZeros(mpf_class &num, int precision) {
    char *buffer = NULL;
    gmp_asprintf(&buffer, "%.*Ff", precision + 2, num.get_mpf_t());

    int counter = strlen(buffer) - 2;

    buffer[counter] = '\0';
    --counter;

    while (buffer[counter] == '0') {
        buffer[counter] = '\0';
        --counter;
    }

    if (buffer[counter] == '.') {
        buffer[counter] = '\0';
    }

    return std::string(buffer);
}

int getSizeOfScope(int i, std::vector<mpq_class> &xn) {
	mpq_class from((i - 1) * (i - 1), 100);
	mpq_class to((i * i), 100);

	int size = 0;

	for (int id = 0; id < xn.size(); id++) {
		if (xn[id] >= from) {
			if (xn[id] < to) {
				size++;
			}
		}
	}
	return size;
}

int main(int argc, char** argv) {
    std::cin.sync_with_stdio(false);

    //2^16 = 65536 -> from spec
    //TODO: check if this value need to be multiplied [base(10) or base(2)] ??
    mpf_set_default_prec(65536);

    if (argc != 2) {
        std::cout << "Wrong number of parameters!" << std::endl;
        return -1;
    }

    int d = atoi(argv[1]);

    if (is_debug) {
        std::cout << "d: " << d << std::endl;
    }

    // 1 <= n <= 2^24 (16777216)
    int n = 0;
    std::vector<mpq_class> xn;
    std::string str;
    while (std::cin >> str) {
        try {
            xn.push_back(parseFraction(str));
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

	std::sort(xn.begin(), xn.end());

    // Chi
	mpq_class chi;
	int k = 10;
	for (int i = 1; i <= k; i++) {
		mpq_class pi(2 * i - 1, 100);
		mpq_class npi(pi * n);
		mpq_class Yi(getSizeOfScope(i, xn));
		Yi -= npi;
		Yi *= Yi;
		Yi /= npi;
		chi += Yi;
	}

    mpf_class f_chi(chi);
    std::cout << fixTrailingZeros(f_chi, d) << std::endl;

    // Kolmogorov
    mpq_class k_plus;
    mpq_class k_minus;
    for (int i = 1; i < n; ++i) {
        mpq_class tmp_k_plus = mpq_class(i, n) - xn[i - 1];
        mpq_class tmp_k_minus = xn[i - 1] - mpq_class(i - 1, n);
        if (tmp_k_plus > k_plus) {
            k_plus = tmp_k_plus;
        }
        if (tmp_k_minus > k_minus) {
            k_minus = tmp_k_minus;
        }
    }
    mpf_class f_k_plus(k_plus);
    mpf_class f_k_minus(k_minus);
    std::cout << fixTrailingZeros(f_k_plus, d) << std::endl;
    std::cout << fixTrailingZeros(f_k_minus, d) << std::endl;

    return 0;
}
