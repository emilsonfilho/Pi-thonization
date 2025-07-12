#include <stdio.h>
#include <sys/time.h>

#define NUM_THERMS 2000000000

int main() {
    double pi_approximation = 0;
    double signal = 1.0;

    struct timeval start;
    gettimeofday(&start, NULL);

    for (unsigned int k = 0; k < NUM_THERMS; k++) {
        pi_approximation += signal/(2*k + 1);
        signal *= -1.0;
    }
    pi_approximation *= 4;

    struct timeval end;
    gettimeofday(&end, NULL);

    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;

    printf("Total Processo (Sequencial): %.2fs\nValor de PI: %.9f\n", time, pi_approximation);
}