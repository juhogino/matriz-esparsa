#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"


void imprimeMatriz(Matriz A) {
    int i, j;
    Celula *atual;

    for (i = 0; i < A.nlin; i++) {
        atual = A.linhas[i].cabeca -> direita; 
        for (j = 0; j < A.ncol; j++) {
            if (atual != A.linhas[i].cabeca && atual->coluna == j) {
                printf("%.2f\t", atual->valor);
                atual = atual->direita;
            } else {
                printf("0.00\t");
            }
        }
        printf("\n");
    }
}

Matriz leMatriz(const char *matrizesesparsas) {
    FILE *arquivo = fopen(matrizesesparsas, "r");
    if (arquivo == NULL) {
        printf("Erro.\n");
        exit(EXIT_FAILURE);
    }

    Matriz M;
    int m, n, i, j;
    float valor;

    fscanf(arquivo, "%d, %d", &m, &n);
    M.nlin = m;
    M.ncol = n;
    M.linhas = (Celula*)malloc(m * sizeof(Celula));

    for (i = 0; i < m; i++) {
        M.linhas[i].cabeca = (Celula*)malloc(sizeof(Celula));
        M.linhas[i].cabeca->direita = M.linhas[i].cabeca;
        M.linhas[i].cabeca->abaixo = M.linhas[i].cabeca;
        M.linhas[i].cabeca->linha = -1;
        M.linhas[i].cabeca->coluna = -1;
        M.linhas[i].cabeca->valor = 0;

        for (j = 0; j < n; j++) {
            fscanf(arquivo, "%d, %d, %f", &i, &j, &valor);
            insere(&M, i, j, valor);
        }
    }

    fclose(arquivo);
    return M;
}

Matriz somaMatrizes(Matriz A, Matriz B) {
    Matriz C;
    if (A.nlin != B.nlin || A.ncol != B.ncol) {
        printf("dimensoes das matrizes nao compativeis para a soma.\n");
        exit(EXIT_FAILURE);
    }
    
    C.nlin = A.nlin;
    C.ncol = A.ncol;
    C.linhas = (Celula*)malloc(A.nlin * sizeof(Celula));
	int i;
    for ( i = 0; i < A.nlin; i++) {
        C.linhas[i].cabeca = (Celula*)malloc(sizeof(Celula));
        C.linhas[i].cabeca->direita = C.linhas[i].cabeca;
        C.linhas[i].cabeca->abaixo = C.linhas[i].cabeca;
        C.linhas[i].cabeca->linha = -1;
        C.linhas[i].cabeca->coluna = -1;
        C.linhas[i].cabeca->valor = 0;

        Celula *atualA = A.linhas[i].cabeca->direita;
        Celula *atualB = B.linhas[i].cabeca->direita;
        Celula *atualC = C.linhas[i].cabeca;

        while (atualA != A.linhas[i].cabeca && atualB != B.linhas[i].cabeca) {
            Celula *novo = (Celula*)malloc(sizeof(Celula));
            if (atualA->coluna < atualB->coluna) {
                *novo = *atualA;
                atualA = atualA->direita;
            } else if (atualB->coluna < atualA->coluna) {
                *novo = *atualB;
                atualB = atualB->direita;
            } else {
                novo->valor = atualA->valor + atualB->valor;
                *novo = *atualA;
                atualA = atualA->direita;
                atualB = atualB->direita;
            }
            novo->direita = atualC->direita;
            atualC->direita = novo;
            atualC = novo;
        }
        while (atualA != A.linhas[i].cabeca) {
            Celula *novo = (Celula*)malloc(sizeof(Celula));
            *novo = *atualA;
            novo->direita = atualC->direita;
            atualC->direita = novo;
            atualA = atualA->direita;
            atualC = novo;
        }
        while (atualB != B.linhas[i].cabeca) {
            Celula *novo = (Celula*)malloc(sizeof(Celula));
            *novo = *atualB;
            novo->direita = atualC->direita;
            atualC->direita = novo;
            atualB = atualB->direita;
            atualC = novo;
        }
    }

    return C;
}

Matriz multiplicaMatrizes(Matriz A, Matriz B) {
    Matriz C;
    
    if (A.ncol != B.nlin) {
        printf("dimensoes das matrizes nao compativeis para a multiplicacao.\n");
        exit(EXIT_FAILURE);
    }
    
    C.nlin = A.nlin;
    C.ncol = B.ncol;
    C.linhas = (Celula*)malloc(C.nlin * sizeof(Celula));
	int i;
    for ( i = 0; i < C.nlin; i++) {
        C.linhas[i].cabeca = (Celula*)malloc(sizeof(Celula));
        C.linhas[i].cabeca->direita = C.linhas[i].cabeca;
        C.linhas[i].cabeca->abaixo = C.linhas[i].cabeca;
        C.linhas[i].cabeca->linha = -1;
        C.linhas[i].cabeca->coluna = -1;
        C.linhas[i].cabeca->valor = 0;
    }

    for (i = 0; i < A.nlin; i++) {
       int k;
	    for ( k = 0; k < A.ncol; k++) {
            if (A.linhas[i].cabeca->direita != A.linhas[i].cabeca) {
                Celula *atualA = A.linhas[i].cabeca->direita;
                while (atualA != A.linhas[i].cabeca) {
                    int j = atualA->coluna;
                    if (B.linhas[k].cabeca->abaixo != B.linhas[k].cabeca) {
                        Celula *atualB = B.linhas[k].cabeca->abaixo;
                        while (atualB != B.linhas[k].cabeca) {
                            if (j == atualB->linha) {
                                int coluna_resultante = atualB->coluna;
                                float valor_resultante = atualA->valor * atualB->valor;

                                insere(&C, i, coluna_resultante, valor_resultante);
                            }
                            atualB = atualB->abaixo;
                        }
                    }
                    atualA = atualA->direita;
                }
            }
        }
    }


    return C;
}

void insere(Matriz *M, int i, int j, float v) {
    if (i >= M->nlin || j >= M->ncol) {
        printf("posicao invalida.\n");
        return;
    }

    Celula *novo = (Celula*)malloc(sizeof(Celula));
    if (novo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(EXIT_FAILURE); }
    novo->linha = i;
    novo->coluna = j;
    novo->valor = v;

    Celula *atual = M->linhas[i].cabeca;
    while (atual->direita != M->linhas[i].cabeca && atual->direita->coluna < j) {
        atual = atual->direita;
    }

    novo->direita = atual->direita;
    atual->direita = novo;
}

void retira(Matriz *M, int i, int j) {

     if (i >= M->nlin || j >= M->ncol) {
        printf("posicao invalida.\n");
        return; }
    Celula *anterior = M->linhas[i].cabeca;
    Celula *atual = M->linhas[i].cabeca->direita;

    while (atual != M->linhas[i].cabeca && atual->coluna != j) {
        anterior = atual;
        atual = atual->direita;
    }

    if (atual != M->linhas[i].cabeca) {
        anterior->direita = atual->direita;
        free(atual);
    }
}

void liberaMatriz(Matriz *M) {
    
    int i;
    for (i = 0; i < M->nlin; i++) {
        Celula *atual = M->linhas[i].cabeca->direita;
        while (atual != M->linhas[i].cabeca) {
            Celula *temp = atual;
            atual = atual->direita;
            free(temp);
        }
        free(M->linhas[i].cabeca);
    }
    free(M->linhas);
}
