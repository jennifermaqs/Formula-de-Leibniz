// q2_1.c - implementar a série sequencialmente.
#include <stdio.h>
#include <time.h>

#define NUM_TERMS 2000000000LL

int main() {
    clock_t start = clock();

    double pi = 0.0;
    double sinal = 1.0;

    for (long long k = 0; k < NUM_TERMS; k++) {
        pi += sinal / (2 * k + 1);
        sinal *= -1.0;
    }

    pi *= 4.0;
    clock_t end = clock();

    double tempo = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Pi (sequencial) = %.9lf\n", pi);
    printf("Total Processo (Sequencial): %.2lf s\n", tempo);

    return 0;
}
