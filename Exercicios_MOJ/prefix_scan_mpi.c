#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int S;
    long long N;

    if (rank == 0) {
       scanf("%d", &S);
       scanf("%lld", &N);
    } 

    MPI_Bcast(&S, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    long long tamanho_bloco = N / size;
    long long resto = N % size;

    int inicio, fim;

    if (rank < resto) {
        inicio = rank * (tamanho_bloco + 1);
        fim = inicio + (tamanho_bloco + 1);
    } else {
        inicio = rank * tamanho_bloco + resto;
        fim = inicio + tamanho_bloco;
    }

    int *vetor = malloc(N * sizeof(int));
    srand(S);

    for (int i = 0; i < N; i++) {
        vetor[i] = rand() % 100;
    }

    int soma_local = 0;

    for (int i = inicio; i < fim; i++) {
        soma_local += vetor[i];
    }

    int soma_total = 0;
    MPI_Reduce(&soma_local, &soma_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        printf("%d\n", soma_total);
    }

    free(vetor);
    MPI_Finalize();
    return 0;
}