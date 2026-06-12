#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Alocando espaço para UM único long int na Heap
    long int *soma = malloc(sizeof(long int));
    
    // Boa prática: verificar se o sistema realmente conseguiu dar a memória
    if (soma == NULL) {
        printf("Erro crítico: Falha na alocação de memória!\n");
        return 1;
    }

    // 2. CRUCIAL: Limpar o lixo de memória antes de usar o +=
    *soma = 0;

    // 3. Laço for acumulando valores de 1 a 10
    for (int i = 1; i <= 10; i++) {
        // Lemos: "O conteúdo apontado por soma recebe o seu próprio conteúdo mais i"
        *soma += i; 
    }

    // 4. Imprimindo o resultado (%ld é o especificador para long int)
    printf("O resultado da somatória de 1 a 10 é: %ld\n", *soma);

    // 5. Sempre liberar a memória alocada dinamicamente
    free(soma);

    return 0;
}