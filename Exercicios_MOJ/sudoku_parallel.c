#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n = 0;
    scanf("%d", &n);
    int *sudoku = malloc(n * n * sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
        scanf("%d", &sudoku[(i * n) + j]);
        }
    }

    // --- BLOCO DE TESTE: Imprimindo para ver na prática ---
    printf("\nTabuleiro lido da memoria:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Imprime o número e um espaço
            printf("%d ", sudoku[(i * n) + j]); 
        }
        // Quando o 'j' termina de rodar, a linha acabou. 
        // Então damos um Enter antes de ir para a próxima linha 'i'.
        printf("\n"); 
    }
    // Boa prática de C: Sempre liberar a memória que você pediu com malloc!
    free(sudoku);
    return 0;
}