#include <stdio.h>
#include <mpi.h> // A biblioteca do MPI

int main(int argc, char** argv) {
    // 1. Inicializa o ambiente MPI (Obrigatório no início)
    MPI_Init(&argc, &argv);

    int rank, size;
    
    // 2. Descobre quem sou eu (rank) e quantos somos no total (size)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Finalize();
    return 0;
}