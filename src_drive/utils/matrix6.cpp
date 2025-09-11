#include "utils/matrix6.h"

void matrix_multiply(float A[][STATE_SIZE], float B[][STATE_SIZE], float C[][STATE_SIZE], int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = 0;
            for (int k = 0; k < dim; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_transpose(float A[][STATE_SIZE], float AT[][STATE_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            AT[j][i] = A[i][j];
        }
    }
}

void matrix_multiply_vec(float A[][STATE_SIZE], float B[], float C[], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        C[i] = 0;
        for (int k = 0; k < cols; k++) {
            C[i] += A[i][k] * B[k];
        }
    }
}

void matrix_identity(float A[][STATE_SIZE], int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            A[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

void matrix_add(float A[][STATE_SIZE], float B[][STATE_SIZE], float C[][STATE_SIZE], int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrix_subtract(float A[][STATE_SIZE], float B[][STATE_SIZE], float C[][STATE_SIZE], int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}