// q2_1.c - Jennifer Marques de Brito
#include <stdio.h>
#include <time.h>

// LL para garantir que o número seja tratado como um long long literal,
// prevenindo possíveis problemas de overflow 
#define NUM_TERMS 2000000000LL

int main() {
    // registra o tempo de início da execução sequencial
    clock_t start = clock();

    double pi = 0.0;
    double sinal = 1.0;

    // loop principal para calcular a soma da série
    // a série é: 1/1 - 1/3 + 1/5 - 1/7 
    // Para k=0, +1/(2*0+1) = +1/1
    // Para k=1, -1/(2*1+1) = -1/3 ... e assim por diante
    for (long long k = 0; k < NUM_TERMS; k++) {
        // adiciona o termo atual a soma
        pi += sinal / (2.0 * k + 1.0);
        // inverte o sinal para o próximo termo
        sinal *= -1.0;
    }

    // após a convergencia para pi/4, multiplica por 4
    // para obter a aproximação de pi
    pi *= 4.0;

    // registra o tempo de término da execução sequencial
    clock_t end = clock();

    // calcula o tempo total gasto em segundos
    double tempo = (double)(end - start) / CLOCKS_PER_SEC;
    //imprime valor aproximado de Pi
    printf("Pi (sequencial) = %.9lf\n", pi);
    //imprime o tempo total de processamento sequencial formatado
    printf("Total Processo (Sequencial): %.2lf s\n", tempo);

    return 0;
}
