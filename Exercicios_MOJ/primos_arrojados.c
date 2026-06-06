#include <stdio.h>
#include <stdbool.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    
    if (n == 2) return true;
    
    if (n % 2 == 0) return false;
    
    // Testa os ímpares de 3 até a raiz quadrada de n
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false; // Achou um divisor, não é primo
        }
    }
    
    return true;
}

int main() {
    int t;

    if (scanf("%d", &t) != 1) return 1;

    while (t > 0) {
        int n;
        scanf("%d", &n);

        bool arrojado = true;

        while (n > 0) {
            if (!is_prime(n)) {
                arrojado = false;
                break;
            }
            n = n / 10; 
        }

        if (arrojado) {
            printf("S\n");
        } else {
            printf("N\n");
        }
        
        t--;
    }

    return 0;
}