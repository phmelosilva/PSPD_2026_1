#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int is_prime(unsigned long long int n) {
    if (n <= 1) return 0;
    
    if (n == 2) return 1;
    
    if (n % 2 == 0) return 0;
    
    for (unsigned long long int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    
    return 1;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N;
    if (rank == 0) {
        scanf("%d", &N);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    unsigned long long int *J = malloc(N * sizeof(unsigned long long int));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            scanf("%llu", &J[i]);
        }
    }

    MPI_Bcast(J, N, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    int tamanho_bloco = N / size;
    int resto = N % size;

    int inicio, fim;

    if (rank < resto) {
        inicio = rank * (tamanho_bloco + 1);
        fim = inicio + (tamanho_bloco + 1);
    } else {
        inicio = rank * tamanho_bloco + resto;
        fim = inicio + tamanho_bloco;
    }

    unsigned long long int soma_local = 0;
    for (int i = inicio; i < fim; i++) {
        if (is_prime(J[i])) {
            soma_local += J[i];
        }
    }

    unsigned long long int soma_total = 0;

    MPI_Reduce(&soma_local, &soma_total, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("%lld\n", soma_total);
    }


    MPI_Finalize();
    return 0;
}