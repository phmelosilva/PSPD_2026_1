# Guia de Urgência — OpenMP

## O que é OpenMP

Biblioteca para paralelismo em **memória compartilhada**. Todas as threads acessam as mesmas variáveis. Funciona com diretivas `#pragma` — você anota o código sequencial e o compilador cuida de criar threads.

Compilar sempre com: `gcc -fopenmp arquivo.c -o executavel -lm`

---

## 1. O mínimo absoluto

### Paralelizar um for

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    // cada iteração roda em uma thread diferente
    resultado[i] = calcula(i);
}
```

**Regra**: cada iteração deve ser **independente**. Se iteração 5 depende do resultado da 4, não pode paralelizar direto.

### Quantas threads?

OpenMP usa por padrão = número de cores do CPU. Pode mudar:
```c
omp_set_num_threads(8);              // no código
export OMP_NUM_THREADS=8             // variável de ambiente
#pragma omp parallel for num_threads(4) // por pragma
```

---

## 2. Variáveis: privadas vs compartilhadas

Essa é a parte mais importante pra não ter bugs.

### Regra padrão
- Variáveis declaradas **antes** do pragma → **shared** (compartilhada)
- Variáveis declaradas **dentro** do bloco → **private** (cada thread tem a sua)

```c
int x = 10;                          // shared por padrão
#pragma omp parallel for
for (int i = 0; i < N; i++) {        // i é SEMPRE private
    int temp = x + i;                // temp é private (declarada dentro)
    resultado[i] = temp;             // resultado é shared (declarada fora)
}
```

### Quando isso causa problema (race condition)

```c
double soma = 0;
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    soma += array[i];  // BUG! Várias threads leem/escrevem soma ao mesmo tempo
}
```

Duas threads podem ler `soma = 5` ao mesmo tempo, ambas calculam `5 + algo`, ambas escrevem → uma sobrescreve a outra. Resultado errado.

---

## 3. Soluções para variáveis compartilhadas

### reduction — a mais comum

```c
double soma = 0;
#pragma omp parallel for reduction(+:soma)
for (int i = 0; i < N; i++) {
    soma += array[i];
}
// soma tem o valor correto aqui
```

Cada thread ganha cópia privada de `soma`, inicializada em 0. No fim, todas são somadas.

Operadores suportados: `+`, `*`, `-`, `&`, `|`, `^`, `&&`, `||`, `max`, `min`

### critical — uma thread por vez

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    int val = calcula(i);
    #pragma omp critical
    {
        // só uma thread executa esse bloco por vez
        if (val > maximo) maximo = val;
    }
}
```

**Lento** — serializa o trecho. Use só quando reduction não resolve.

### atomic — operação atômica simples

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    #pragma omp atomic
    contador++;  // operação simples, protegida por hardware
}
```

Mais rápido que critical, mas só funciona com operações simples (`++`, `+=`, etc).

### Quando não precisa de nada

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    C[i] = A[i] + B[i];  // cada thread escreve posição diferente → sem conflito
}
```

---

## 4. collapse — fundindo loops

```c
// SEM collapse: só divide o loop de i (M iterações)
#pragma omp parallel for
for (int i = 0; i < M; i++)
    for (int j = 0; j < N; j++)
        C[i][j] = calcula(i, j);

// COM collapse: divide M*N iterações
#pragma omp parallel for collapse(2)
for (int i = 0; i < M; i++)
    for (int j = 0; j < N; j++)
        C[i][j] = calcula(i, j);
```

**Quando usar**: M pequeno e N grande (ou vice-versa). collapse distribui melhor.

**Requisito**: os dois loops devem ser independentes e ter limites fixos (não depender de i).

---

## 5. schedule — como distribuir iterações

```c
#pragma omp parallel for schedule(static)    // divide igual (padrão)
#pragma omp parallel for schedule(dynamic)   // sob demanda
#pragma omp parallel for schedule(dynamic, 4) // blocos de 4
```

| Tipo | Como funciona | Quando usar |
|------|---------------|-------------|
| `static` | Divide igualmente no início | Iterações custam o mesmo |
| `dynamic` | Thread pega próxima iteração livre | Custo variável por iteração |
| `guided` | Blocos decrescentes | Meio-termo |

---

## 6. Outras diretivas úteis

### parallel (sem for) — bloco paralelo genérico
```c
#pragma omp parallel
{
    int tid = omp_get_thread_num();
    printf("Thread %d executando\n", tid);
}
```

### sections — tarefas diferentes em paralelo
```c
#pragma omp parallel sections
{
    #pragma omp section
    { tarefa_A(); }
    
    #pragma omp section
    { tarefa_B(); }
}
```

### barrier — sincronização
```c
#pragma omp parallel
{
    fase1();
    #pragma omp barrier    // todas as threads esperam aqui
    fase2();               // só começa quando TODAS terminaram fase1
}
```

---

## 7. Exercícios práticos

### Exercício 1: Soma de vetor (5 min)
Escreva programa que soma 1 milhão de números em paralelo.
```c
#include <stdio.h>
#include <omp.h>

int main() {
    int N = 1000000;
    double array[N]; // ou global se for grande
    for (int i = 0; i < N; i++) array[i] = i * 0.001;

    double soma = 0;
    // ADICIONE O PRAGMA AQUI
    for (int i = 0; i < N; i++)
        soma += array[i];

    printf("Soma: %.2f\n", soma);
    return 0;
}
```
**Resposta**: `#pragma omp parallel for reduction(+:soma)`

### Exercício 2: Encontrar máximo (5 min)
```c
int maximo = 0;
// Qual pragma usar?
for (int i = 0; i < N; i++) {
    if (array[i] > maximo) maximo = array[i];
}
```
**Resposta**: `#pragma omp parallel for reduction(max:maximo)`

### Exercício 3: Preencher matriz identidade (5 min)
```c
// Precisa de reduction? atomic? critical? Nada?
for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
        M[i][j] = (i == j) ? 1.0 : 0.0;
```
**Resposta**: `#pragma omp parallel for collapse(2)` — nada mais. Cada (i,j) é posição diferente.

### Exercício 4: Contar pares (5 min)
```c
int count = 0;
for (int i = 0; i < N; i++) {
    if (array[i] % 2 == 0) count++;
}
```
**Resposta**: `#pragma omp parallel for reduction(+:count)`

### Exercício 5: Multiplicação de matrizes completa
Implemente do zero: ler M, K, N, ler matrizes, multiplicar em paralelo, imprimir resultado.
Restrição: sem reduction, sem atomic, sem critical.
(Referência: `06_matriz_openmp.c`)

### Exercício 6: Validação de Sudoku
Verificar linhas, colunas e blocos em paralelo.
(Referência: `07_sudoku_paralelo.c`)

---

## 8. Checklist mental pra prova

Quando ver um problema OpenMP:

1. [ ] Qual é o loop principal a paralelizar?
2. [ ] Iterações são independentes?
3. [ ] Alguma variável é escrita por múltiplas threads?
   - Soma → `reduction(+:var)`
   - Posições diferentes de array → nada
   - Operação complexa → `critical`
4. [ ] Loop externo tem poucas iterações? → `collapse(2)`
5. [ ] Use `long long` se N > 2 bilhões
6. [ ] Arrays grandes → declarar global (não local)
7. [ ] Compilar com `-fopenmp`

---

## 9. Erros clássicos

```c
// ERRO 1: esquecer que soma é shared
double soma = 0;
#pragma omp parallel for          // faltou reduction(+:soma)
for (...) soma += x;

// ERRO 2: collapse com loop dependente
#pragma omp parallel for collapse(2)
for (int i = 0; i < N; i++)
    for (int j = 0; j < i; j++)   // j depende de i! collapse não funciona bem
        ...

// ERRO 3: variável declarada fora usada como temp
int temp;                          // shared! várias threads sobrescrevem
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    temp = calcula(i);             // race condition
    resultado[i] = temp;
}
// FIX: declarar temp DENTRO do for

// ERRO 4: printf dentro de região paralela
// Funciona, mas saída embaralhada. Não é erro de lógica, só de output.
```
