#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    long long N;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        scanf("%lld", &N);
    }

    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    long long tamanho_bloco = N / size;
    long long resto = N % size;

    long long inicio, fim;

    if (rank < resto) {
        inicio = rank * (tamanho_bloco + 1);
        fim = inicio + (tamanho_bloco + 1);
    } else {
        inicio = rank * tamanho_bloco + resto;
        fim = inicio + tamanho_bloco;
    }

    double L = 1.0 / (double) N;
    double soma_local = 0.0;

    for (long long i = inicio; i < fim; i++) {
        double x = (i + 0.5) * L;
        
        soma_local += 4.0 / (1.0 + x * x);
    }

    double soma_total = 0;
    MPI_Reduce(&soma_local, &soma_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = soma_total * L;
        printf("%.10lf\n", pi);
    }

    MPI_Finalize();
    return 0;
}