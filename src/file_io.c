#include "file_io.h"

/* Initialize the given file in read or write mode
-------------------------------------------------- */
FILE * file_initialize(char * filename, enum file_mode mode) {
	if (mode == READ) {
		return fopen(filename,"r");
	}
	return fopen(filename,"w");
}

/* Load the size of the grid
---------------------------- */
void getSize (FILE * file, struct grid * grid) {
	fscanf(file, "%d %d\n", &(grid->line_number), &(grid->column_number));
}

/* Load all contraints
---------------------- */
void loadContraints (FILE * file, struct grid * grid) {
	int contraint = 0;
	int j = 0;
	int number;
	int k = 0;
	while (k != 2) {
		if (k == 0) {
			number = grid->line_number;
		}
		else {
			number = grid->column_number;
		}
		for (int i = 0 ; i < number ; i++) {
			char c = fgetc(file);
			while (c != ';' && c != '\n') {
				if (c == ' ') {
					c = fgetc(file);
				}
				else {
					contraint = 0;
					while (c != ' ' && c != ';' && c != '\n') {
						contraint = contraint * 10 + (c - '0');
						c = fgetc(file);
					}
					if (k == 0) {
						grid->line_contraints[i].contraint[j] = contraint;
					}
					else {
						grid->column_contraints[i].contraint[j] = contraint;
					}
					j++;
				}
			}
			if (k == 0) {
				grid->line_contraints[i].number = j;
			}
			else {
				grid->column_contraints[i].number = j;
			}
			j = 0;
		}
		k++;
	}
}

/* Auxiliary function get corresponding cell state with given char
------------------------------------------------------------------ */
enum cell mask_file(char input) {
	int value = input - '0';
	switch (value) {
		case 0:
			return WHITE;
			break;
		case 1:
			return BLACK;
			break;
		default:
			return UNDEFINED;
	}
}

void loadCells(FILE * file, struct grid * grid) {
	int i = 0;
	char c = fgetc(file);
	while (c != EOF) {
		if (i > grid->column_number * grid->line_number) {
			statusMessage(INCORRECT_FILE);
			statusMessage(EXITING);
			exit(1);
		}
		if (c != '\n') {
			grid->cell[i] = mask_file(c);
			i++;
		}
		c = fgetc(file);
	}
	if (i < grid->column_number * grid->line_number) {
			statusMessage(INCORRECT_FILE);
			statusMessage(EXITING);
			printf("YOTE");
			exit(1);
	}
}

/* Dinamicaly allocate the grid
------------------------------- */
void allocGrid(struct grid * grid) {
	grid->cell = malloc(grid->column_number * grid->line_number * sizeof(enum cell));
	if (grid->cell == NULL) {
		statusMessage(LOAD_FAILED);
		statusMessage(EXITING);
		exit(1);
	}
	grid->added = 0;
}

void unallocGrid(struct grid * grid) {
	free(grid->cell);
	grid->cell = NULL;
}