#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main () {
    int M, K, N; 
    
    // Lê as dimensões das matrizes
    scanf("%d %d %d", &M, &K, &N);
    
    // Aloca a Matriz A como um vetor 1D
    int *matrizA = malloc(M * K * sizeof(int));

    // Aloca a Matriz A como um vetor 1D
    int *matrizB = malloc(K * N * sizeof(int));

    // Lendo a Matriz A
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            scanf("%d", &matrizA[(i * K) + j]); 
        }
    }

    // Lendo a Matriz B
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &matrizB[(i * N) + j]); 
        }
    }

    // Alocando a Matriz C com CALLOC (já nasce com tudo zero)
    // O tamanho dela é M (linhas de A) x N (colunas de B)
    int *matrizC = calloc(M * N, sizeof(int));

    // A mágica Paralela do OpenMP vai aqui!
    // As iterações do laço 'i' (linhas) são totalmente independentes,
    // a Thread 0 calcula a linha 0 de C, a Thread 1 calcula a linha 1 de C.
    // Nenhum conflito de memória!
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                // A Lógica em 1D:
                // Posição em C: (i * N) + j
                // Posição em A: (i * K) + k
                // Posição em B: (k * N) + j
                matrizC[(i * N) + j] += matrizA[(i * K) + k] * matrizB[(k * N) + j];
            }
        }
    }

    printf("Matriz C:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrizC[(i * N) + j]); // Imprime o número e um espaço
        }
        printf("\n"); // Pula de linha toda vez que o 'j' termina (fim da linha)
    }
    free(matrizA);
    free(matrizB);
    free(matrizC);
    return 0;
}