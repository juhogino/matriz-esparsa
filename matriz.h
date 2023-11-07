#ifndef MATRIZ_H
#define MATRIZ_H

typedef struct Celula {
    struct Celula *direita, *abaixo, *cabeca;
    int linha, coluna;
    float valor;
} Celula;

typedef struct {
    Celula *linhas;
    int nlin, ncol;
} Matriz;

void imprimeMatriz(Matriz A);
Matriz leMatriz(const char *matrizesesparsas);
Matriz somaMatrizes(Matriz A, Matriz B);
Matriz multiplicaMatrizes(Matriz A, Matriz B);
void insere(Matriz *M, int i, int j, float v);
void retira(Matriz *M, int i, int j);
void liberaMatriz(Matriz *M);

#endif

