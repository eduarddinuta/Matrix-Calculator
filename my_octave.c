// Copyright Dinuta Eduard-Stefan 311CA
#include <stdio.h>
#include <stdlib.h>
#include "my_functions.h"

#define MOD 10007

int main(void)
{
	// initializarea memoriei, la inceput cu dimensiunea 0 si capacitatea 1
	int size = 0;
	int capacity = 1;
	memory_t *mem = (memory_t *)malloc(capacity * sizeof(memory_t));

	// citim comenzi pana cand intalnim Q
	char command;
	scanf(" %c", &command);
	while (command != 'Q') {
		// daca la urmatoarea adaugare in memorie am depasi capacitatea
		if (size >= capacity)
			resize(&mem, &capacity, 2, size);
		switch (command) {
		case 'L': {
			int rows, columns, **a;
			read_matrix(&rows, &columns, &a);
			if (!a) {
				free_memory(mem, size);
				return -1;
			}
			mem[size].lin = rows;
			mem[size].col = columns;
			add(mem, &size, a);
			break;
		}
		case 'D':
			print_matrix_dimension(mem, size);
			break;
		case 'P':
			print_matrix(mem, size);
			break;
		case 'C':
			crop_matrix(mem, size);
			break;
		case 'M': {
			int **b = product(mem, size);
			if (b)
				add(mem, &size, b);
			break;
		}
		case 'O':
			sort(mem, size);
			break;
		case 'T':
			transpose(mem, size);
			break;
		case 'F':
			erase(mem, &size);
			// daca am sters mult matrici si folosim inutil memorie
			if (size < capacity / 2)
				resize(&mem, &capacity, 0.5, size);
			break;
		case 'S': {
			int ind1, ind2;
			scanf(" %d%d", &ind1, &ind2);
			int dim1 = mem[ind1].lin;
			int dim2 = mem[ind2].lin;
			if (verify_ind(ind1, ind2, size) && dim1 == dim2) {
				int **b = strassen(mem[ind1].a, mem[ind2].a, dim1);
				mem[size].lin = dim1;
				mem[size].col = dim1;
				add(mem, &size, b);
			}
			break;
		}
		default:
			printf("Unrecognized command\n");
			break;
		}
		scanf(" %c", &command);
		if (command == 'Q')
			free_memory(mem, size);
	}
	return 0;
}
