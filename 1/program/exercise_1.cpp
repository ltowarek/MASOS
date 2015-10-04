#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "gmp.h"

static const bool is_debug = false;

void print(char *buffer) {
    int counter = strlen(buffer) - 2;

    while (buffer[counter] == '0') {
        buffer[counter] = '\0';
        --counter;
    }

    if (buffer[counter] == '.') {
        buffer[counter] = '\0';
    }

    printf("%s\n", buffer);
}

int main(int argc, char** argv) {
    int d = 0;
    scanf("%d", &d);

    if (is_debug) {
        printf("%d\n", d);
    }

    // TODO: Set precision to *d*

    mpf_t tmp;
    mpf_init(tmp);

    mpf_t sum;
    mpf_init(sum);

    // 1 <= n <= 2^24 (16777216)
    int n = 0;
    mpf_t *xn = (mpf_t*) malloc(16777216 * sizeof(mpf_t));
    mpf_t x;
    mpf_init(x);

    while (gmp_scanf("%Ff", x) != EOF) {
        mpf_init_set(xn[n], x);
        mpf_add(tmp, sum, x);
        mpf_swap(sum, tmp);
        n++;
    }

    if (is_debug) {
        printf("%d\n", n);
    }

    mpf_t average;
    mpf_init(average);

    mpf_div_ui(average, sum, n);

    char *buffer = NULL;
    gmp_asprintf(&buffer, "%Ff\n", average);

    print(buffer);

    mpf_t first;
    mpf_init(first);

    mpf_t second;
    mpf_init(second);

    for (int i = 0; i < n; ++i) {
        mpf_t power;
        mpf_init(power);

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

    gmp_printf("%ff\n", variance);

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
