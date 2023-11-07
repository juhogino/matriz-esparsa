#include <stdio.h>
#include "matriz.h"

int main() {
    Matriz A, B, C;
    
    A = leMatriz("matrizA.txt"); //arquivo com os valores da matriz A
    printf("Matriz A:\n");
    imprimeMatriz(A);

    B = leMatriz("matrizB.txt"); //arquivo com os valores da matriz B
    printf("Matriz B:\n");
    imprimeMatriz(B);

    C = somaMatrizes(A, B);
    printf("soma de A e B:\n");
    imprimeMatriz(C);

    liberaMatriz(&A);
    liberaMatriz(&B);
    liberaMatriz(&C);

    return 0;
}

