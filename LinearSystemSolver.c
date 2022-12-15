/*
    Author:     Zhengmao Zhang
    Date:       2022-10-19
    Version:    1.0
    Description:    This file contains the implementation of the linear system solver.
    compiler:   gcc LinearSystemSolver.c
    run:        ./a.out
    example input: 
    Enter the number of equations: 4
    Enter the coefficients of equation 1: 0.02 0.01 0 0 0.02
    Enter the coefficients of equation 2: 1 2 1 0 1
    Enter the coefficients of equation 3: 0 1 2 1 4
    Enter the coefficients of equation 4: 0 0 100 200 800
    example output:
    The matrix is:
    0.020000 0.010000 0.000000 0.000000  | 0.020000 
    1.000000 2.000000 1.000000 0.000000  | 1.000000 
    0.000000 1.000000 2.000000 1.000000  | 4.000000 
    0.000000 0.000000 100.000000 200.000000  | 800.000000 
    The value of x1 is:1.000000 The value of x2 is:0.000000 The value of x3 is:0.000000 The value of x4 is:4.000000
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
double matrix[1000][10001];
double answer[1000];
void printMatrix(int n, int m);
void getMatrix(int n, int m);
void matrixSolver(int n, int m);
void printAnswer(int n);
int main(){
    int n = 0;
    printf("Enter the number of equations: ");
    scanf("%d", &n);
    getMatrix(n, n + 1);
    printMatrix(n, n + 1);
    matrixSolver(n, n + 1);
    printAnswer(n);
    return 0;
}
void getMatrix(int n, int m){
    int i, j;
    for(i = 0; i < n; i++){
        printf("Enter the coefficients of equation %d: ", i + 1);
        for(j = 0; j < m; j++){
            scanf("%lf", &matrix[i][j]);
        }
    }
}
void printMatrix(int n, int m){
    printf("The matrix is:\n");
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(j == m - 1) printf(" | ");
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}
void matrixSolver(int n, int m){
    int i, j, k;
    for(i = 0; i < n; i++){
        for(j = i + 1; j < n; j++){
            double ratio = matrix[j][i] / matrix[i][i];
            for(k = i; k < m; k++){
                matrix[j][k] -= ratio * matrix[i][k];
            }
        }
    }
    for(i = n - 1; i >= 0; i--){
        answer[i] = matrix[i][m - 1];
        for(j = i + 1; j < n; j++){
            answer[i] -= matrix[i][j] * answer[j];
        }
        answer[i] /= matrix[i][i];
    }
}
void printAnswer(int n){
    int i;
    for(i = 0; i < n; i++){
        printf("The value of x%d is:", i + 1);
        printf("%lf\n", answer[i]);
    }
}