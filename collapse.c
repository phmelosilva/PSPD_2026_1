#include <stdio.h>
#include <omp.h>

int main () {
    int n = 100;

    // 1. ABRIMOS A REGIÃO PARALELA (Aqui nascem as threads)
    // Como tem várias coisas lá dentro, SOMOS OBRIGADOS a usar {}
    #pragma omp parallel 
    {
        // 2. Várias coisas acontecendo em paralelo ANTES do for
        int tid = omp_get_thread_num();
        printf("Thread %d acordou e está pronta para trabalhar!\n", tid);

        // 3. Agora dividimos o trabalho do for entre as threads que já estão vivas
        // Note que aqui é só "omp for", e não "omp parallel for"
        #pragma omp for collapse(2)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Cálculo aqui...
            }
        }

        // 4. Coisas acontecendo em paralelo DEPOIS do for
        printf("Thread %d terminou sua parte da matriz!\n", tid);

    } // FIM DO BLOCO PARALELO (Aqui as threads morrem e voltamos a ser sequencial)

    printf("Fim do programa principal.\n");
    return 0;
}