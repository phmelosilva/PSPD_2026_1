#include <stdio.h>
#include <string.h>

int main() {
    int l = 0;
    int linha, coluna = 1;
    char texto[1005][1005];

    scanf("%d", &l);
    getchar();
    for (int i = 0; i < l; i++) {
        fgets(texto[i], 1005, stdin);
    }
    scanf("%d %d", &linha, &coluna);
    linha--;
    coluna--;
    int coluna_alvo = coluna;
    char comando;
    while (scanf(" %c", &comando) != EOF) {
        if (comando == 'j') {
            if (linha < l - 1) {
                linha++;
                int ultimo_indice = strlen(texto[linha]) - 2;
                if (coluna_alvo > ultimo_indice) {
                    coluna = ultimo_indice;
                } else {
                    coluna = coluna_alvo;
                }
            }
        }

        if (comando == 'k') {
            if (linha > 0) {
                linha--;
                int ultimo_indice = strlen(texto[linha]) - 2;
                if (coluna_alvo > ultimo_indice) {
                    coluna = ultimo_indice;
                } else {
                    coluna = coluna_alvo;
                }
            }
        }

        printf("%d %d %c\n", linha+1, coluna+1, texto[linha][coluna]);
    }


}