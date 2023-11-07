
#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int row, col;
    double valor;
    struct Node *next;
} Node;


typedef struct {
    int rows, cols;
    Node **rows_array;
} Matriz;


Node* createNode(int row, int col, double valor) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->row = row;
    newNode->col = col;
    newNode->valor = valor;
    newNode->next = NULL;
    return newNode;
}

void insere(Matriz *mat, int row, int col, double valor) {
    Node *newNode = createNode(row, col, valor);
    Node *temp = mat->rows_array[row];

    if (temp == NULL) {
        mat->rows_array[row] = newNode;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void imprimeMatriz(Matriz mat) {
    for (int i = 0; i < mat.rows; i++) {
        Node *temp = mat.rows_array[i];
        for (int j = 0; j < mat.cols; j++) {
            if (temp != NULL && temp->col == j) {
                printf("%.2f ", temp->valor);
                temp = temp->next;
            } else {
                printf("0.00 ");
            }
        }
        printf("\n");
    }
}

Matriz leMatriz() {
    int rows, cols;
    FILE *file = fopen("matrizes.txt", "r"); //alterar nome do arquivo

    fscanf(file, "%d %d", &rows, &cols);

    Matriz mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.rows_array = (Node **)calloc(rows, sizeof(Node *));

    int row, col;
    double valor;
    while (fscanf(file, "%d %d %lf", &row, &col, &valor) != EOF) {
        insere(&mat, row, col, valor);
    }

    fclose(file);
    return mat;
}

Matriz somaMatrizes(Matriz A, Matriz B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        printf("matrizes nao possuem o mesmo tamanho logo a soma nao pode ser realizada.\n");
        exit(1);
    }

    Matriz C;
    C.rows = A.rows;
    C.cols = A.cols;
    C.rows_array = (Node **)calloc(C.rows, sizeof(Node *));

    for (int i = 0; i < A.rows; i++) {
        Node *tempA = A.rows_array[i];
        Node *tempB = B.rows_array[i];
        while (tempA != NULL || tempB != NULL) {
            int col = (tempA != NULL) ? tempA->col : tempB->col;
            double val = (tempA != NULL) ? tempA->valor : 0;

            if (tempA != NULL && tempA->col == col) {
                val += tempB->valor;
                tempA = tempA->next;
                tempB = (tempB != NULL) ? tempB->next : NULL;
            } else {
                val += tempB->valor;
                tempB = tempB->next;
            }

            insere(&C, i, col, val);
        }
    }
    return C;
}

int main(void) {
    Matriz A, B, C;
    A = leMatriz();
    printf("matriz A:\n");
    imprimeMatriz(A);

    B = leMatriz();
    printf("matriz B:\n");
    imprimeMatriz(B);

    C = somaMatrizes(A, B);
    printf("soma:\n");
    imprimeMatriz(C);

    return 0;
}

