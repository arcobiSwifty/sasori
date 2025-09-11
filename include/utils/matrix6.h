#ifndef MATRIX6
#define MATRIX6

const int STATE_SIZE = 6;

void matrix_multiply(float A[][STATE_SIZE], float B[][STATE_SIZE], float C[][STATE_SIZE], int dim);
void matrix_transpose(float A[][STATE_SIZE], float AT[][STATE_SIZE], int rows, int cols);
void matrix_multiply_vec(float A[][STATE_SIZE], float B[], float C[], int rows, int cols);
void matrix_identity(float A[][STATE_SIZE], int dim);
void matrix_add(float A[][STATE_SIZE], float B[][STATE_SIZE], float C[][STATE_SIZE], int dim);
void matrix_subtract(float A[][STATE_SIZE], float B[][STATE_SIZE], float C[][STATE_SIZE], int dim);
#endif