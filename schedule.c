#include <stdio.h>
#include <omp.h>
#include <unistd.h> // Biblioteca para usar a função sleep()

int main() {
    int N = 12;
    
    // Forçamos o uso de apenas 2 threads para o teste ficar claro
    omp_set_num_threads(2); 

    printf("Iniciando processamento de %d iterações...\n", N);
    double tempo_inicio = omp_get_wtime(); // Inicia o cronômetro do OpenMP

    // AQUI ESTÁ A MÁGICA: Mude de 'static' para 'dynamic' para testar
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        int tid = omp_get_thread_num(); // Descobre qual thread eu sou (0 ou 1)
        
        // Simulação de Carga Desbalanceada:
        // Se for a primeira metade do loop (0 a 5), é um trabalho pesado
        if (i < 6) {
            sleep(1); // Fica travado por 1 segundo
        }
        // Se for a segunda metade (6 a 11), passa direto (instantâneo)

        printf("Thread %d terminou a iteração %d\n", tid, i);
    }

    double tempo_fim = omp_get_wtime(); // Para o cronômetro
    printf("\nTempo Total de Execução: %.2f segundos\n", tempo_fim - tempo_inicio);

    return 0;
}