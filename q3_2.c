#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THERMS 2000000000
#define NUM_THREADS 16
#define PARTIAL_NUM_THERMS ((NUM_THERMS)/(NUM_THREADS))

double result = 0;
double time_threads = 0;
pthread_mutex_t mutex;

double partialFormula(int first_therm) {
    const int num_terms = first_therm + PARTIAL_NUM_THERMS;

    double pi_approximation = 0;
    double signal = 1.0;
    
    for (unsigned int k = first_therm; k < (first_therm + PARTIAL_NUM_THERMS); k++) {
        pi_approximation += signal/(2*k + 1);
        signal *= -1.0;
    }

    return pi_approximation;
}

void* partialProcessing(void* args) {
    int first_therm = *((int*) args);

    struct timeval start;
    gettimeofday(&start, NULL);

    double sum = partialFormula(first_therm);

    pthread_mutex_lock(&mutex);
    result += sum;
    pthread_mutex_unlock(&mutex);

    struct timeval end;
    gettimeofday(&end, NULL);

    pthread_t tid = pthread_self();
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
    printf("TID: %lu: %.2fs\n", tid, time);

    pthread_mutex_lock(&mutex);
    time_threads += time;
    pthread_mutex_unlock(&mutex);
}

int main() {   
    struct timeval start_total;
    gettimeofday(&start_total, NULL);

    pthread_t thread[NUM_THREADS];
    unsigned int first[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        first[i] = i * PARTIAL_NUM_THERMS;
        pthread_create(&thread[i], NULL, &partialProcessing, &first[i]);
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    result *= 4;

    struct timeval end_total;
    gettimeofday(&end_total, NULL);

    double time_total = (end_total.tv_sec - start_total.tv_sec) + (end_total.tv_usec - start_total.tv_usec) / 1000000.0;

    printf("Total Processo (Paralelo): %.2fs\nTotal Threads: %.2fs\nValor de PI: %.9f\n", time_total, time_threads, result);

    return EXIT_SUCCESS;
}