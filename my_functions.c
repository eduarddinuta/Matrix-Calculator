// Copyright Dinuta Eduard-Stefan 311CA
#include <stdio.h>
#include <stdlib.h>

#define MOD 10007

// Memoria
typedef struct {
	int **a;
	int lin, col;
} memory_t;

// verifica conditia de indici pentru produs
int verify_ind(int ind1, int ind2, int size)
{
	if (ind1 >= size || ind1 < 0 || ind2 >= size || ind2 < 0) {
		printf("No matrix with given ind\n");
		return 0;
	}
	return 1;
}

// alocare dinamica de vector
int *alloc_vector(int number)
{
	int *v = (int *)malloc(number * sizeof(int));
	if (!v) {
		fprintf(stderr, "malloc() failed");
		return NULL;
	}
	return v;
}

// alocare dinamica de matrice
int **alloc_matrix(int rows, int columns)
{
	int **a = (int **)malloc(rows * sizeof(int *));
	if (!a) {
		fprintf(stderr, "malloc() failed");
		return NULL;
	}
	for (int i = 0; i < rows; i++) {
		a[i] = alloc_vector(columns);
		if (!a[i]) {
			fprintf(stderr, "malloc() failed");
			for (int j = 0; j < i; j++)
				free(a[j]);
			free(a);
			return NULL;
		}
	}
	return a;
}

// eliberarea memoriei (operatia 'Q')
void free_memory(memory_t *mem, int size)
{
	for (int i = 0; i < size; i++) {
		int rows = mem[i].lin;
		for (int j = 0; j < rows; j++) {
			free(mem[i].a[j]);
			mem[i].a[j] = NULL;
		}
		free(mem[i].a);
		mem[i].a = NULL;
	}
	free(mem);
	mem  = NULL;
}

// citirea unei matrici
void read_matrix(int *rows, int *columns, int ***a)
{
	scanf(" %d%d", rows, columns);
	*a = alloc_matrix(*rows, *columns);
	if (!a)
		return;

	for (int i = 0; i < *rows; i++)
		for (int j = 0; j < *columns; j++)
			scanf("%d", (*(*a + i) + j));
}

// redimensionarea memoriei; variabila sz permite redimensionarea
// flexibila in functie de nevoie: marirea pentru adaugarea de noi
// matrici sau micsorarea dupa mai multe stergeri pentru a nu folosi
// inutil memorie
void resize(memory_t **mem, int *capacity, double sz, int size)
{
	*capacity = *capacity * sz;
	memory_t *tmp = (memory_t *)realloc(*mem, *capacity * sizeof(memory_t));
	if (!tmp) {
		fprintf(stderr, "realloc() failed");
		free_memory(*mem, size);
		exit(-1);
	}
	*mem = tmp;
}

// adaugarea in memorie (operatia 'L')
void add(memory_t *mem, int *size, int **A)
{
	if (!A)
		return;
	mem[*size].a = A;
	*size = *size + 1;
}

// afisarea dimensiunilor unei matrici (operatia 'D')
void print_matrix_dimension(memory_t *mem, int size)
{
	int ind;
	scanf(" %d", &ind);
	if (ind >= size || ind < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	int rows = mem[ind].lin;
	int columns  = mem[ind].col;
	printf("%d %d\n", rows, columns);
}

// afisarea unei matrici (operatia 'Pl)
void print_matrix(memory_t *mem, int size)
{
	int ind;
	scanf(" %d", &ind);
	if (ind >= size || ind < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	int rows = mem[ind].lin;
	int columns  = mem[ind].col;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++)
			printf("%d ", mem[ind].a[i][j]);
		printf("\n");
	}
}

// redimensionarea unei matrici (operatia 'C')
void crop_matrix(memory_t *mem, int size)
{
	int ind;
	scanf(" %d", &ind);

	int number_rows, number_columns;
	int *rows, *columns;

	scanf(" %d", &number_rows);
	rows = alloc_vector(number_rows);
	for (int i = 0 ; i < number_rows; i++)
		scanf(" %d", &rows[i]);

	scanf(" %d", &number_columns);
	columns = alloc_vector(number_columns);
	for (int i = 0 ; i < number_columns; i++)
		scanf(" %d", &columns[i]);

	if (ind >= size || ind < 0) {
		printf("No matrix with the given index\n");
		free(rows);
		free(columns);
		return;
	}

	// o noua matrice pe care o construim cu indicii liniilor si
	// coloanelor din rows[] si columns[]
	int **tmp = alloc_matrix(number_rows, number_columns);
	if (!tmp) {
		free_memory(mem, size);
		exit(-1);
	}
	for (int i = 0 ; i < number_rows; i++)
		for (int j = 0 ; j < number_columns; j++)
			tmp[i][j] = mem[ind].a[rows[i]][columns[j]];

	for (int i = 0; i < mem[ind].lin; i++)
		free(mem[ind].a[i]);
	free(mem[ind].a);

	// copiem matricea auxiliara in memorie
	mem[ind].a = tmp;
	mem[ind].lin = number_rows;
	mem[ind].col = number_columns;
	free(rows);
	free(columns);
}

// produsul a doua matrici (operatia 'M')
int **product(memory_t *mem, int size)
{
	int ind1, ind2;
	scanf(" %d%d", &ind1, &ind2);
	if (ind1 >= size || ind2 >= size || ind1 < 0 || ind2 < 0) {
		printf("No matrix with the given index\n");
		return NULL;
	}

	int rows1 = mem[ind1].lin;
	int columns1 = mem[ind1].col;
	int rows2 = mem[ind2].lin;
	int columns2 = mem[ind2].col;

	if (columns1 != rows2) {
		printf("Cannot perform matrix multiplication\n");
		return NULL;
	}

	mem[size].lin = rows1;
	mem[size].col = columns2;
	// matrice auxiliara in care realizam produsul
	int **aux = alloc_matrix(rows1, columns2);
	if (!aux) {
		free_memory(mem, size);
		exit(-1);
	}
	for (int i = 0; i < rows1; i++)
		for (int j = 0; j < columns2; j++) {
			aux[i][j] = 0;
			for (int k = 0; k < columns1; k++) {
				aux[i][j] += (mem[ind1].a[i][k] * mem[ind2].a[k][j]) % MOD;
				aux[i][j] %= MOD;
				if (aux[i][j] < 0)
					aux[i][j] += MOD;
			}
		}
	return aux;
}

// suma elementelor unei matrici pentru operatia de sortare
int sum(int ind, memory_t *mem)
{
	int rows = mem[ind].lin;
	int columns  = mem[ind].col;
	int s = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++) {
			s = (s + mem[ind].a[i][j]) % MOD;
			if (s < 0)
				s += MOD;
		}
	return s;
}

void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// sortarea matricilor in functie de suma (operatia 'S')
// folosind sortarea prin selectie
void sort(memory_t *mem, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++)
			if (sum(i, mem) > sum(j, mem)) {
				int **aux = mem[i].a;
				mem[i].a = mem[j].a;
				mem[j].a = aux;
				swap(&mem[i].lin, &mem[j].lin);
				swap(&mem[i].col, &mem[j].col);
			}
}

// transpunerea unei matrici (operatia 'T')
void transpose(memory_t *mem, int size)
{
	int ind;
	scanf(" %d", &ind);
	if (ind >= size || ind < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	int rows = mem[ind].lin;
	int columns  = mem[ind].col;
	// matrice auxiliara in care construim transpusa
	int **tmp = alloc_matrix(columns, rows);
	if (!tmp) {
		free_memory(mem, size);
		exit(-1);
	}

	for (int i = 0; i < columns; i++)
		for (int j = 0; j < rows; j++)
			tmp[i][j] = mem[ind].a[j][i];

	for (int i = 0; i < rows; i++)
		free(mem[ind].a[i]);
	free(mem[ind].a);

	// copiem matricea auxiliara in memorie
	mem[ind].a = tmp;
	mem[ind].lin = columns;
	mem[ind].col = rows;
}

// stergerea unei matrici din memorie (operatia 'F')
void erase(memory_t *mem, int *size)
{
	int ind, rows;
	scanf("%d", &ind);
	if (ind >= *size || ind < 0) {
		printf("No matrix with the given index\n");
		return;
	}

	rows = mem[ind].lin;
	for (int i = 0; i < rows; i++)
		free(mem[ind].a[i]);
	free(mem[ind].a);
	// realizam permutarea matricilor cu o pozitie la dreapta
	for (int i = ind; i < *size - 1; i++) {
		mem[i].a = mem[i + 1].a;
		mem[i].lin = mem[i + 1].lin;
		mem[i].col = mem[i + 1].col;
	}

	*size = *size - 1;
}

// impartirea unei matrici patratice de dimensiune 2^n
// in 4 submatrici de dimensiune 2^(n-1)
void partition(memory_t *AP, int **A, int dim, int *crash)
{
	for (int i = 0; i < 4; i++) {
		AP[i].a = alloc_matrix(dim, dim);
		if (!AP[i].a) {
			AP = NULL;
			return;
		}
		AP[i].lin = dim;
	}

	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			AP[0].a[i][j] = A[i][j];
			AP[1].a[i][j] = A[i][j + dim];
			AP[2].a[i][j] = A[i + dim][j];
			AP[3].a[i][j] = A[i + dim][j + dim];
		}
	*crash = 1;
}

// suma a doua matrici
int **sum_matrix(int **A, int **B, int dim, int sign)
{
	int **a = alloc_matrix(dim, dim);
	if (!a)
		return NULL;
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			a[i][j] = (A[i][j] + sign * B[i][j]) % MOD;
			a[i][j] %= MOD;
			if (a[i][j] < 0)
				a[i][j] += MOD;
		}
	return a;
}

// initializarea cu 0 a matricilor folosite pentru efectuarea
// operatiilor in algoritmul strassen
void clear(int **A, int **B, int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			A[i][j] = 0;
			B[i][j] = 0;
		}
}

//eliberarea memoriei unei matrici alocate dinamic
void free_matrix(int **A, int dim)
{
	for (int i = 0; i < dim; i++)
		free(A[i]);
	free(A);
}

//construirea submatricilor care alcatuiesc matricea rezultat
//in algoritmul strassen
void build_CP(memory_t *CP, memory_t *M, int dim, int *crash)
{
	int **a, **b;
	a = sum_matrix(M[0].a, M[3].a, dim, 1);
	if (!a)
		return;
	b = sum_matrix(M[6].a, M[4].a, dim, -1);
	if (!b)
		return;
	CP[0].a = sum_matrix(a, b, dim, 1);
	free_matrix(a, dim);
	free_matrix(b, dim);

	CP[1].a = sum_matrix(M[2].a, M[4].a, dim, 1);

	CP[2].a = sum_matrix(M[1].a, M[3].a, dim, 1);

	a = sum_matrix(M[0].a, M[1].a, dim, -1);
	if (!a)
		return;
	b = sum_matrix(M[2].a, M[5].a, dim, 1);
	if (!b)
		return;
	CP[3].a = sum_matrix(a, b, dim, 1);
	free_matrix(a, dim);
	free_matrix(b, dim);

	for (int i = 0; i < 4; i++)
		CP[i].lin = dim;

	*crash = 1;
}

void build_M(memory_t *, memory_t *, memory_t *, int, int *);

//algoritmul strassen de inmultire a matricilor (operatia 'S')
int **strassen(int **A, int **B, int dim)
{
	if (!A || !B)
		return NULL;
	if (dim == 1) {
		int **C = alloc_matrix(1, 1);
		if (!C)
			return NULL;
		C[0][0] = (A[0][0] * B[0][0]) % MOD;
		C[0][0] %= MOD;
		if (C[0][0] < 0)
			C[0][0] += MOD;
		return C;
	}

	int new_dim = dim / 2;
	memory_t *AP = (memory_t *)malloc(4 * sizeof(memory_t));
	memory_t *BP = (memory_t *)malloc(4 * sizeof(memory_t));
	memory_t *M = (memory_t *)malloc(7 * sizeof(memory_t));
	memory_t *CP = (memory_t *)malloc(4 * sizeof(memory_t));
	int **C = alloc_matrix(dim, dim);
	if (!AP || !BP || !M || !CP || !C) {
		fprintf(stderr, "malloc failed");
		free_memory(AP, 4);
		free_memory(BP, 4);
		free_memory(CP, 4);
		free_memory(M, 7);
		free_matrix(C, dim);
		return NULL;
	}
	int crash = 0;
	partition(AP, A, new_dim, &crash);
	if (crash == 0) {
		fprintf(stderr, "malloc failed");
		free_memory(AP, 4);
		free_memory(BP, 4);
		return NULL;
	}
	crash = 0;
	partition(BP, B, new_dim, &crash);
	if (crash == 0) {
		fprintf(stderr, "malloc failed");
		free_memory(AP, 4);
		free_memory(BP, 4);
		return NULL;
	}

	crash = 0;
	build_M(M, AP, BP, new_dim, &crash);
	if (crash == 0) {
		fprintf(stderr, "malloc failed");
		free_memory(AP, 4);
		free_memory(BP, 4);
		free_memory(M, 7);
		return NULL;
	}

	crash = 0;
	build_CP(CP, M, new_dim, &crash);
	if (crash == 0) {
		fprintf(stderr, "malloc failed");
		free_memory(AP, 4);
		free_memory(BP, 4);
		free_memory(CP, 4);
		free_memory(M, 7);
		return NULL;
	}

	for (int i = 0; i < new_dim; i++)
		for (int j = 0; j < new_dim; j++) {
			C[i][j] = CP[0].a[i][j];
			C[i][j + new_dim] = CP[1].a[i][j];
			C[i + new_dim][j] = CP[2].a[i][j];
			C[i + new_dim][j + new_dim] = CP[3].a[i][j];
		}
	free_memory(AP, 4);
	free_memory(BP, 4);
	free_memory(CP, 4);
	free_memory(M, 7);
	return C;
}

//construirea celor 7 matrici folosite in algoritmul strassen
void build_M(memory_t *M, memory_t *AP, memory_t *BP, int dim, int *crash)
{
	int **a, **b;
	a = sum_matrix(AP[0].a, AP[3].a, dim, 1);
	if (!a)
		return;
	b = sum_matrix(BP[0].a, BP[3].a, dim, 1);
	if (!b)
		return;
	M[0].a = strassen(a, b, dim);
	if (!M[0].a)
		return;
	free_matrix(a, dim);
	free_matrix(b, dim);

	a = sum_matrix(AP[2].a, AP[3].a, dim, 1);
	if (!a)
		return;
	b = BP[0].a;
	M[1].a = strassen(a, b, dim);
	if (!M[1].a)
		return;
	free_matrix(a, dim);

	a = AP[0].a;
	b = sum_matrix(BP[1].a, BP[3].a, dim, -1);
	if (!b)
		return;
	M[2].a = strassen(a, b, dim);
	if (!M[2].a)
		return;
	free_matrix(b, dim);

	a = AP[3].a;
	b = sum_matrix(BP[2].a, BP[0].a, dim, -1);
	if (!b)
		return;
	M[3].a = strassen(a, b, dim);
	if (!M[3].a)
		return;
	free_matrix(b, dim);

	a = sum_matrix(AP[0].a, AP[1].a, dim, 1);
	if (!a)
		return;
	b = BP[3].a;
	M[4].a = strassen(a, b, dim);
	if (!M[4].a)
		return;
	free_matrix(a, dim);

	a = sum_matrix(AP[2].a, AP[0].a, dim, -1);
	if (!a)
		return;
	b = sum_matrix(BP[0].a, BP[1].a, dim, 1);
	if (!b)
		return;
	M[5].a = strassen(a, b, dim);
	if (!M[5].a)
		return;
	free_matrix(a, dim);
	free_matrix(b, dim);

	a = sum_matrix(AP[1].a, AP[3].a, dim, -1);
	if (!a)
		return;
	b = sum_matrix(BP[2].a, BP[3].a, dim, 1);
	if (!b)
		return;
	M[6].a = strassen(a, b, dim);
	if (!M[6].a)
		return;
	free_matrix(a, dim);
	free_matrix(b, dim);

	for (int i = 0; i < 7; i++)
		M[i].lin = dim;

	*crash = 1;
}
