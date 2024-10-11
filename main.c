#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tamanho do vetor de valores RGB
#define rgb 3

// Estrutura do cabeçalho do arquivo
typedef struct arquivo {
    char codificacao[2]; // Ex: P3
    int n[2];            // Ordem da matriz NxN
    int maxRGB;          // Valor máximo de RGB
}arquivo;

// Função que lê o cabeçalho do arquivo
arquivo leCabecalho(FILE *file) {
    arquivo arq;
    
    fscanf(file, "%s", arq.codificacao);
    fscanf(file, "%d[^ ]", &arq.n[0]);
    fscanf(file, "%d[^\n]", &arq.n[1]);
    fscanf(file, "%d", &arq.maxRGB);

    return arq;
}

/*
Entrada: Ordem n da matriz.
Descrição: Aloca a matriz preenchida inicida com zeros na Heap.
Saída: Matriz alocada ou NULL (caso de erro).
*/
int* alocarMatrizComZeros(int n) {
    int *mat;

    mat = (int*)calloc((n * n * rgb), sizeof(int));
    
    return mat;
}

/*
Entrada: Ordem n da matriz.
Descrição: Aloca a matriz na Heap.
Saída: Matriz alocada ou NULL (caso de erro).
*/
int* alocarMatriz(int n) {
    int *mat;

    mat = (int*)malloc((n * n * rgb) * sizeof(int));
    
    return mat;
}

/*
Entrada: Arquivo, matriz alocada, ordem n da matriz.
Descrição: Leitura de uma matriz vinda de um arquivo, ordem n da matriz.
Saída: void.
*/
void lerMatrizArquivo(FILE *file, int* mat, int n) {
    int indice;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
        indice = (i * n + j) * rgb;
        fscanf(file, "%d %d %d ", &mat[indice], &mat[indice + 1], &mat[indice + 2]);
        }
    }
    return;
}

/*
Entrada: Cabeçalho do arquivo, matriz alocada, ordem n da matriz.
Descrição: Salva uma dada matriz em um arquivo de resultado
Saída: void.
*/
void salvarResultado(arquivo arq, int *mat, int n) {
    char path[] = "Resultados/resultado.out";
    int indice;
    FILE *file = fopen(path, "w");

    fprintf(file, "%s\n%d %d\n%d\n", arq.codificacao, arq.n[0], arq.n[1], arq.maxRGB);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
        indice = (i * n + j) * rgb;
        fprintf(file, "%d %d %d ", mat[indice], mat[indice + 1], mat[indice + 2]);
        }
        fprintf(file, "\n");
    }

    return;
}


/*
Entrada: Matriz A e B que serão somadas (mesma matriz mas com coordenadas diferentes), ordem n da matriz original, 
         ordem k da submatriz, as coordenadas x (linhas) e y (colunas) dos dois quadrantes e a operação a ser realizada.
Descrição: Função que realiza a soma ou subtração de dois quadrantes de uma matriz.
Saída: Matriz R resultante da operação.
*/
int* operacaoMatriz(int* A, int *B, int n, int k, int xA, int yA, int xB, int yB, char op) {
    int* R = alocarMatriz(k); // Aloca a submatriz resultante
    int indiceA, indiceB, indiceR; // Índices para acessar os valores das matrizes

    if (op == '+') {
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                indiceA = ((xA + i) * n + (yA + j)) * rgb;
                indiceB = ((xB + i) * n + (yB + j)) * rgb;
                indiceR = (i * k + j) * rgb;

                R[indiceR] = A[indiceA] + B[indiceB];
                R[indiceR + 1] = A[indiceA + 1] + B[indiceB + 1];
                R[indiceR + 2] = A[indiceA + 2] + B[indiceB + 2];
            }
        }
    } else {
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                indiceA = ((xA + i) * n + (yA + j)) * rgb;
                indiceB = ((xB + i) * n + (yB + j)) * rgb;
                indiceR = (i * k + j) * rgb;

                R[indiceR] = A[indiceA] - B[indiceB];
                R[indiceR + 1] = A[indiceA + 1] - B[indiceB + 1];
                R[indiceR + 2] = A[indiceA + 2] - B[indiceB + 2];
            }
        }
    }

    return R;
}


/*
Entrada: Matrizes A, B e C e seus respectivos tamanhos, variável 'novo' para guardar o tamanho das novas submatrizes
         os valores x (linha) e y (coluna) de cada matriz;
Descrição: Função que realiza a multiplicação de duas matrizes quadradas A e B e preenche a matriz resultante C.
Saída: void.
*/
void multiplicarMatrizes(int* A, int* B, int* C, int nA, int nB, int nC, int novo, int xA, int yA, int xB, int yB, int xC, int yC) {

    // Dividimos a matriz em 4 quadrantes
    novo = novo / 2;

    if(novo == 1) {
        for(int k = 0; k < rgb; k++) {

            // Calculo dos índices separando em quadrantes 2x2
            int indiceC00 = (xC * nC + yC) * rgb + k;
            int indiceC01 = (xC * nC + (yC + novo)) * rgb + k;
            int indiceC10 = ((xC + novo) * nC + yC) * rgb + k;
            int indiceC11 = ((xC + novo) * nC + (yC + novo)) * rgb + k;

            int indiceA00 = (xA * nA + yA) * rgb + k;
            int indiceA01 = (xA * nA + (yA + novo)) * rgb + k;
            int indiceA10 = ((xA + novo) * nA + yA) * rgb + k;
            int indiceA11 = ((xA + novo) * nA + (yA + novo)) * rgb + k;

            int indiceB00 = (xB * nB + yB) * rgb + k;
            int indiceB01 = (xB * nB + (yB + novo)) * rgb + k;
            int indiceB10 = ((xB + novo) * nB + yB) * rgb + k;
            int indiceB11 = ((xB + novo) * nB + (yB + novo)) * rgb + k;

            // AE + BG
            C[indiceC00] += A[indiceA00] * B[indiceB00] + A[indiceA01] * B[indiceB10];
            // AF + BH
            C[indiceC01] += A[indiceA00] * B[indiceB01] + A[indiceA01] * B[indiceB11];
            // CE + DG
            C[indiceC10] += A[indiceA10] * B[indiceB00] + A[indiceA11] * B[indiceB10];
            // CF + DH
            C[indiceC11] += A[indiceA10] * B[indiceB01] + A[indiceA11] * B[indiceB11];
        }
        return;
    }

    // 1º Quadrante
    // Aqui calculamos AE + BG
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA, yA, xB, yB, xC, yC);
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA, yA + novo, xB + novo, yB, xC, yC);

    // 2º Quadrante
    // Aqui calculamos AF + BH
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA, yA, xB, yB + novo, xC, yC + novo);
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA, yA + novo, xB + novo, yB + novo, xC, yC + novo);

    // 3º Quadrante
    // Aqui calculamos CE + DG
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA + novo, yA, xB, yB, xC + novo, yC);
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA + novo, yA + novo, xB + novo, yB, xC + novo, yC);

    // 4º Quadrante
    // Aqui calculamos CF + DH
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA + novo, yA, xB, yB + novo, xC + novo, yC + novo);
    multiplicarMatrizes(A, B, C, nA, nB, nC, novo, xA + novo, yA + novo, xB + novo, yB + novo, xC + novo, yC + novo);
}


int* strassen(int* A, int* B, int nA, int nB, int novo, int x1, int y1, int x2, int y2) {
    int *P1, *P2, *P3, *P4, *P5, *P6, *P7; // Matrizes Ps que guardam os 7 produtos
    int k; // Guarda o tamanho das novas submatrizes

    // Define o limite para a multiplicação de matrizes pelo método convencional
    if(novo <= 32) {
        int *C = alocarMatrizComZeros(novo);
        multiplicarMatrizes(A, B, C, nA, nB, novo, novo, x1, y1, x2, y2, 0, 0);

        return C;
    }

    k = novo / 2;


    // Aqui começa as somas de matrizes e cálculo dos produtos 
    // P1 = A(F − H)
    P1 = strassen(A, operacaoMatriz(B, B, nB, k, x2, y2 + k, x2 + k, y2 + k, '-'), nA, k, k, x1, y1, 0, 0);

    // P2 = (A + B)H
    P2 = strassen(operacaoMatriz(A, A, nA, k, x1, y1, x1, y1 + k, '+'), B, k, nB, k, 0, 0, x2 + k, y2 + k);

    // P3 = (C + D)E
    P3 = strassen(operacaoMatriz(A, A, nA, k, x1 + k, y1, x1 + k, y1 + k, '+'), B, k, nB, k, 0, 0, x2, y2);

    // P4 = D(G − E)
    P4 = strassen(A, operacaoMatriz(B, B, nB, k, x2 + k, y2, x2, y2, '-'), nA, k, k, x1 + k, y1 + k, 0, 0);

    // P5 = (A + D)(E + H)
    P5 = strassen(operacaoMatriz(A, A, nA, k, x1, y1, x1 + k, y1 + k, '+'), operacaoMatriz(B, B, nB, k, x2, y2, x2 + k, y2 + k, '+'), k, k, k, 0, 0, 0, 0);

    // P6 = (B − D)(G + H)
    P6 = strassen(operacaoMatriz(A, A, nA, k, x1, y1 + k, x1 + k, y1 + k, '-'), operacaoMatriz(B, B, nB, k, x2 + k, y2, x2 + k, y2 + k, '+'), k, k, k, 0, 0, 0, 0);

    // P7 = (A − C)(E + F)
    P7 = strassen(operacaoMatriz(A, A, nA, k, x1, y1, x1 + k, y1, '-'), operacaoMatriz(B, B, nB, k, x2, y2, x2, y2 + k, '+'), k, k, k, 0, 0, 0, 0);

    // Juntando os 7 produtos para formar a matriz resultante
    int *C = alocarMatriz(novo);
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            int indiceC00 = (i * novo + j) * rgb;
            int indiceC01 = (i * novo + (j + k)) * rgb;
            int indiceC10 = ((i + k) * novo + j) * rgb;
            int indiceC11 = ((i + k) * novo + (j + k)) * rgb;

            int indiceP = (i * k + j) * rgb;

            for (int cor = 0; cor < rgb; cor++) {
                C[indiceC00 + cor] = P5[indiceP + cor] + P4[indiceP + cor] - P2[indiceP + cor] + P6[indiceP + cor];
                C[indiceC01 + cor] = P1[indiceP + cor] + P2[indiceP + cor];
                C[indiceC10 + cor] = P3[indiceP + cor] + P4[indiceP + cor];
                C[indiceC11 + cor] = P1[indiceP + cor] + P5[indiceP + cor] - P3[indiceP + cor] - P7[indiceP + cor];
            }
        }
    }

    free(P1);
    free(P2);
    free(P3);
    free(P4);
    free(P5);
    free(P6);
    free(P7);

    return C;
}

int main() {
    int *A, *B, *C; // Matriz A, B e C
    struct arquivo arq; // Cabeçalho do arquivo
    char nomeArquivo[5];
    char caminho[20] = {"Casos-de-Teste/"};
    FILE *file;


    // Caminho dos arquivos
    printf("Nome do arquivo: ");
    scanf(" %[^\n]", nomeArquivo);

    strcat(caminho, nomeArquivo);
    file = fopen(caminho, "r");
    
    arq = leCabecalho(file);

    // Alocação de memória para as matrizes
    A = alocarMatriz(arq.n[0]);
    if (!A) {
        fclose(file);
        return -1;
    }

    B = alocarMatriz(arq.n[0]);
    if (!B) {
        fclose(file);
        free(A);
        return -1;
    }

    // Lendo o arquivo de imagem
    lerMatrizArquivo(file, A, arq.n[0]);
    lerMatrizArquivo(file, B, arq.n[0]);
    fclose(file);


    // Multiplicação das matrizes usando Strassen
    C = strassen(A, B, arq.n[0], arq.n[0], arq.n[0], 0, 0, 0, 0);

    // Salavando o resultado em arquivo   
    salvarResultado(arq, C, arq.n[0]);

    // Liberação de memória das matrizes
    free(A);
    free(B);
    free(C);

    return 0;
}