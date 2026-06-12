#include <stdio.h>
#include <mpi.h> // A biblioteca do MPI

int main(int argc, char** argv) {
    // 1. Inicializa o ambiente MPI (Obrigatório no início)
    MPI_Init(&argc, &argv);

    int rank, size;
    
    // 2. Descobre quem sou eu (rank) e quantos somos no total (size)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        // Eu sou o Processo 0. O que eu fizer aqui, SÓ EU faço.
        int numero_secreto = 42;
        printf("[Processo 0] Inventei o numero %d e vou enviar para o Processo 1.\n", numero_secreto);
        
        // Enviando a mensagem:
        // (endereço_do_dado, quantidade, tipo, rank_destino, etiqueta(tag), comunicador)
        MPI_Send(&numero_secreto, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } 

    MPI_Finalize();
    return 0;
}