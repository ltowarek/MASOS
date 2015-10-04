#include <vector>
#include <stdio.h>
#include <gmp.h>

using namespace std;

int main() {
	
	mpf_t tmp;
    int d;
	scanf("%d", &d);

	mpf_t sum;
	mpf_init_set_d(sum, 0);

	/***Input***/
    vector<mpf_t> xn;
    mpf_t x;
    while (gmp_scanf ("%Zd", x)) {
        xn.push_back(x);
		mpf_add(tmp, sum, x);
		mpf_swap(sum, tmp);
    }

	int n = xn.size();

#ifdef _DEBUG
	printf("N:%d\n", n);
#endif 

	/***Exercise 1***/
	mpf_t average;
	mpf_div_ui(average, sum, n);

#ifdef _DEBUG
	printf("AVG: ");
#endif 
	gmp_printf("%Zd\n", average);

	/***Exercise 2***/
	mpf_t first, second;
	mpf_init_set_d(first, 0);
	mpf_init_set_d(second, 0);

	for (int i = 0; i < n; i++) {
		mpf_t pow;
		mpf_mul(pow, xn[i], xn[i]);
		mpf_add(tmp, first, pow);
		mpf_swap(tmp, first);

		mpf_add(tmp, second, xn[i]);
		mpf_swap(tmp, second);

		mpf_clear(pow);
	}
	
	mpf_div_ui(tmp, first, n);
	mpf_swap(tmp, first);
	
	mpf_div_ui(tmp, second, n);
	mpf_swap(tmp, second);
	mpf_mul(tmp, second, second);
	mpf_swap(tmp, second);

	mpf_t variance;
	mpf_sub(variance, first, second);


#ifdef _DEBUG
	printf("VARIANCE: ");
#endif 
	gmp_printf("%Zd\n", variance);

	/***Exercise 3***/
	int p, p_min = p = n;

	while (--p) {
		bool valid = true;
		for (int k = 0; k < n - p; k++) {
			if (mpf_cmp(xn[k], xn[k+p])) {
				valid = false;
				break;
			}
		}
		if (valid) {
			p_min = p;
		}
	}

#ifdef _DEBUG
	printf("Perdiod: ");
#endif 
	printf("%d\n", p_min);

	mpf_clear(sum);
	mpf_clear(average);
	mpf_clear(first);
	mpf_clear(second);
	mpf_clear(tmp);
	mpf_clear(variance);

    return 0;
}
