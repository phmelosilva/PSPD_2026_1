#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main()
{
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); //Obtém número da thread
        // Não há ordem definida de qual vai aparecer primeiro
        printf("Oi da thread %d\n", id);
    }
    return 0;
}
