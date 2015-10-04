#include <cstdio>
#include <cstdlib>
#include "gmp.h"

static const bool is_debug = false;

int main(int argc, char** argv) {
    int d = 0;
    scanf("%d", &d);

    if (is_debug) {
        printf("%d\n", d);
    }

    // TODO: Set precision to *d*

    mpf_t tmp;
    mpf_init_set_d(tmp, 0.0);

    mpf_t sum;
    mpf_init_set_d(sum, 0.0);

    size_t n = 0;
    mpf_t *xn = NULL;
    mpf_t x;
    mpf_init_set_d(x, 0.0);

    while (gmp_scanf("%Ff", x) != EOF) {
        n++;
        xn = (mpf_t*) realloc(xn, n * sizeof(mpf_t));
        mpf_init_set(xn[n-1], x);

        mpf_add(tmp, sum, x);
        mpf_swap(sum, tmp);
    }

    if (is_debug) {
        printf("%d", n);
    }

    mpf_t average;
    mpf_init_set_d(average, 0.0);

    mpf_div_ui(average, sum, n);
    gmp_printf("%Ff\n", average);

    mpf_t first;
    mpf_init_set_d(first, 0.0);

    mpf_t second;
    mpf_init_set_d(second, 0.0);

    for (int i = 0; i < n; ++i) {
        mpf_t power;
        mpf_init_set_d(power, 0.0);

        mpf_mul(power, xn[i], xn[i]);
        mpf_add(tmp, first, power);
        mpf_swap(tmp, first);

        mpf_add(tmp, second, xn[i]);
        mpf_swap(tmp, second);

        mpf_clear(power);
    }

    mpf_div_ui(tmp, first, n);
    mpf_swap(tmp, first);

    mpf_div_ui(tmp, second, n);
    mpf_swap(tmp, second);

    mpf_mul(tmp, second, second);
    mpf_swap(tmp, second);

    mpf_t variance;
    mpf_sub(variance, first, second);

    gmp_printf("%Ff\n", variance);

    int p = n;
    int p_min = n;

    while (--p) {
        bool is_valid = true;
        for (int i = 0; i < n - p; ++i) {
            if (mpf_cmp(xn[i], xn[i+p])) {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            p_min = p;
        }
    }

    printf("%d\n", p_min);

    for (int i = 0; i < n; ++i) {
        mpf_clear(xn[i]);
    }

    free(xn);
    mpf_clear(variance);
    mpf_clear(second);
    mpf_clear(first);
    mpf_clear(average);
    mpf_clear(x);
    mpf_clear(sum);
    mpf_clear(tmp);

    return 0;
}
