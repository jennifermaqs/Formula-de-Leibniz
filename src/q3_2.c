// q3_2.c - 
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define NUM_TERMS 2000000000LL
#define NUM_THREADS 16
#define PARTIAL_NUM_TERMS ((NUM_TERMS) / (NUM_THREADS))

CRITICAL_SECTION cs; // proteção da variável global
double soma_global = 0.0;

typedef struct {
    int id;
    long long start_index;
    double tempo_gasto;
} ThreadArgs;

double partialFormula(long long first_term) {
    long long last_term = first_term + PARTIAL_NUM_TERMS;
    double soma = 0.0;
    double sinal = (first_term % 2 == 0) ? 1.0 : -1.0;

    for (long long k = first_term; k < last_term; k++) {
        soma += sinal / (2 * k + 1);
        sinal *= -1.0;
    }

    return soma;
}

DWORD WINAPI partialProcessing(LPVOID param) {
    ThreadArgs* args = (ThreadArgs*)param;

    DWORD inicio = GetTickCount();
    double parcial = partialFormula(args->start_index);

    // Protegendo a soma global
    EnterCriticalSection(&cs);
    soma_global += parcial;
    LeaveCriticalSection(&cs);

    DWORD fim = GetTickCount();
    args->tempo_gasto = (fim - inicio) / 1000.0;

    printf("TID: %d: %.2lf s\n", args->id, args->tempo_gasto);
    return 0;
}

int main() {
    HANDLE threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    InitializeCriticalSection(&cs);

    DWORD inicio_total = GetTickCount();

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].start_index = i * PARTIAL_NUM_TERMS;
        args[i].tempo_gasto = 0.0;

        threads[i] = CreateThread(
            NULL, 0,
            partialProcessing,
            &args[i],
            0, NULL
        );
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    double tempo_total = (GetTickCount() - inicio_total) / 1000.0;
    double soma_tempos = 0.0;

    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
        soma_tempos += args[i].tempo_gasto;
    }

    double pi = soma_global * 4.0;
    printf("Pi (paralelo) = %.9lf\n", pi);
    printf("Total Processo (Paralelo): %.2lf s\n", tempo_total);
    printf("Total Threads: %.2lf s\n", soma_tempos);

    DeleteCriticalSection(&cs);
    return 0;
}
