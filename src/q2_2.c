// q2_2.c (paralelo) - Jennifer Marques de Brito
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// LL para garantir que o número seja tratado como um long long literal,
// prevenindo possíveis problemas de overflow 
#define NUM_TERMS 2000000000LL
#define NUM_THREADS 16
#define PARTIAL_NUM_TERMS ((NUM_TERMS) / (NUM_THREADS))

// mecanismo de sincronização equivalente a um mutex.
CRITICAL_SECTION cs; // proteção da variável global
double soma_global = 0.0;

// struct para passar argumentos para a função de thread
typedef struct {
    int id;  // ID sequencial para a thread 
    long long start_index; // indice inicial de intervalo de termos que a thread deve calcular
    double tempo_gasto; // tempo individual que a thread levou pra completar a tarefa
} ThreadArgs;

// função que calcula uma parte da soma da série 
double partialFormula(long long first_term) {
    // calcula o índice do último termo que esta thread deve processar 
    long long last_term = first_term + PARTIAL_NUM_TERMS;
    double soma = 0.0;
    // se first_term é par, o sinal é positivo (+1.0); se ímpar, é negativo (-1.0)
    double sinal = (first_term % 2 == 0) ? 1.0 : -1.0;

    // loop pra somar os termos da série dentro do intervalo 
    for (long long k = first_term; k < last_term; k++) {
        soma += sinal / (2 * k + 1); // adiciona o termo atual a soma parcial
        sinal *= -1.0;               // inverte o sinal para o próximo termo
    }

    return soma;
}

// função que será executada por cada thread
DWORD WINAPI partialProcessing(LPVOID param) {
     // Converção do ponteiro genérico para ThreadArgs*
    ThreadArgs* args = (ThreadArgs*)param;

    // GetTickCount() registra o tempo de início de execução 
    // e retorna o número de milissegundos desde a inicialização do sistema
    DWORD inicio = GetTickCount();
    // calcula a soma parcial designada para esta thread
    double parcial = partialFormula(args->start_index);

    // INÍCIO DA SEÇÃO CRÍTICA:
    // Protegendo a soma global
    EnterCriticalSection(&cs);
    soma_global += parcial;  // adiciona a soma parcial desta thread a soma global
    LeaveCriticalSection(&cs);
    // FIM DA SEÇÃO CRÍTICA:


    // registra o tempo de término da execução 
    DWORD fim = GetTickCount();
    // calcula o tempo gasto e armazena na struct
    args->tempo_gasto = (fim - inicio) / 1000.0;

    printf("TID: %d: %.2lf s\n", args->id, args->tempo_gasto);
    return 0;
}

int main() {
    HANDLE threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    // inicializa a seção critica antes de criar qualquer thread
    InitializeCriticalSection(&cs);

    // registra o tempo de início do processo paralelo total (desde a criação da primeira thread)
    DWORD inicio_total = GetTickCount();

    // loop para criar e iniciar cada thread.
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i; //atribui um id sequencial
        args[i].start_index = i * PARTIAL_NUM_TERMS; //calcula o indice de inicio 
        args[i].tempo_gasto = 0.0; // tempo gasto inicializa como zero

    // cria a thread.
        threads[i] = CreateThread(
            NULL, 0,
            partialProcessing,
            &args[i],
            0, NULL
        );
    }

    // Espera que todas as threads terminem sua execução.
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    // calcula o tempo total de processamento paralelo (desde o início até todas terminarem)
    double tempo_total = (GetTickCount() - inicio_total) / 1000.0;
    double soma_tempos = 0.0; // variável para somar os tempos individuais de cada thread

    // loop para fechar os handles das threads e somar os tempos individuais
    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
        soma_tempos += args[i].tempo_gasto;
    }
    // soma ainda está em pi/4, então multiplica por 4 para obter pi
    double pi = soma_global * 4.0;

    // valor aproximado de Pi obtido paralelamente
    printf("Pi (paralelo) = %.9lf\n", pi);
    // tempo total que o processo paralelo levou
    printf("Total Processo (Paralelo): %.2lf s\n", tempo_total);
    // soma dos tempos individuais de cada thread
    printf("Total Threads: %.2lf s\n", soma_tempos);

    // deleta a Critical Section para liberar seus recursos.
    DeleteCriticalSection(&cs);
    return 0;
}
