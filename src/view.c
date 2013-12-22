#include "view.h"

/* Print a line full of '-' of the specified size
------------------------------------------------- */
void printHLine(int length) {
	for (int i = 0 ; i < length ; i++) {
		printf("-");
	}
	printf("\n");
}

/* Return the number of lines required to print columns contraints
------------------------------------------------------------------ */
int columnContraintsLines(struct grid * grid) {
	int number = 0;
	for (int i = 0 ; i < grid->column_number ; i++) {
		if (grid->column_contraints[i].number > number) {
			number = grid->column_contraints[i].number;
		}
	}
	return number;
}

/* Print the column contraints lines (on top)
--------------------------------------------- */
void printColumnsContraints(struct grid * grid) {
	int linesToPrint = columnContraintsLines(grid);

	for (int i = 0 ; i < linesToPrint ; i++) {
		for (int j = 0 ; j < grid->column_number ; j++) {
			if (grid->column_contraints[j].number > i) {
				printf(" %d", grid->column_contraints[j].contraint[i]);
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}
	//printHLine(grid->column_number * 2 + 1);
}

/* Print line contraints as first part of each line
------------------------------------------------ */
void printLineContraints(int index, struct grid * grid) {
	for (int i = 0 ; i < grid->line_contraints[index].number ; i++) {
		printf("%d ", grid->line_contraints[index].contraint[i]);
	}
}

/* Print a single cell given by its line and column
--------------------------------------------------- */
char printCell(int line, int column, struct grid * grid) {
	int i = grid->cell[line * grid->column_number + column];
	switch (i) {
		case BLACK:
			return 'B';
			break;
		case WHITE:
			return ' ';
			break;
		default:
			return '?';
	}
}

/* Print a full line (contraints + cells)
----------------------------------------- */
void printLine(int index, struct grid * grid) {
	for (int i = 0 ; i < grid->column_number ; i++) {
		printf("|%c", printCell(index, i, grid));
	}
	printf("|\t");
	printLineContraints(index, grid);
	printf("\n");
}

/* Print grid to stdout
----------------------- */
void printGrid(struct grid * grid) {
	printColumnsContraints(grid);

	for (int i = 0 ; i < grid->line_number ; i++) {
		printLine(i, grid);
		//printHLine(grid->column_number * 2 + 1);
	}
}

void statusMessage(enum status status) {
	switch (status) {
		case STARTED:
		printf("[i] Starting...\n");
		break;

		case LOADED:
		printf("[i] Grid loaded !\n");
		break;
		
		case LOAD_FAILED:
		printf("[x] Grid loading failed...\n");
		break;
		
		case SOLUTION:
		printf("[i] Grid solved !\n");
		break;
		
		case NO_SOLUTIONS:
		printf("[x] There is no solutions to this grid...\n");
		break;
		
		case EXITING:
		printf("[i] Exiting...\n");
		break;

		case INCORRECT_FILE:
		printf("[i] Wrong file format...\n");
		break;
	}
}
