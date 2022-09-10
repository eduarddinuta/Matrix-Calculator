// Copyright Dinuta Eduard-Stefan 311CA
#ifndef _FUNCTIONS_
#define _FUNCTIONS

#define MOD 10007

// Memoria
typedef struct {
	int **a;
	int lin, col;
} memory_t;

int verify_ind(int ind1, int ind2, int size);
int *alloc_vector(int number);
int **alloc_matrix(int rows, int columns);
void free_memory(memory_t *mem, int size);
void read_matrix(int *rows, int *columns, int ***a);
void resize(memory_t **mem, int *capacity, double sz, int size);
void add(memory_t *mem, int *size, int **A);
void print_matrix_dimension(memory_t *mem, int size);
void print_matrix(memory_t *mem, int size);
void crop_matrix(memory_t *mem, int size);
int **product(memory_t *mem, int size);
int sum(int ind, memory_t *mem);
void swap(int *a, int *b);
void sort(memory_t *mem, int size);
void transpose(memory_t *mem, int size);
void erase(memory_t *mem, int *size);
void partition(memory_t *AP, int **A, int dim, int *crash);
int **sum_matrix(int **A, int **B, int dim, int sign);
void clear(int **A, int **B, int dim);
void free_matrix(int **A, int dim);
void build_CP(memory_t *CP, memory_t *M, int dim, int *crash);
int **strassen(int **A, int **B, int dim);
void build_M(memory_t *M, memory_t *AP, memory_t *BP, int dim, int *crash);
#endif
