#include "main.h"
	
int main(int argc, char **argv) {
	statusMessage(STARTED);
	// Check first argument (file name)
	if (argc == 1) {
		printf("Must have the grid source file as first argument : picture [FILE]\n");
		return EXIT_FAILURE;
	}
	else {
		FILE * file = file_initialize(argv[1], READ);
		if (file == NULL) {
			printf("Error loading the specified file... Is it here ?\n");
			return EXIT_FAILURE;
		}
		
		struct grid grid;
		getSize(file, &grid);

		allocGrid(&grid);

		loadContraints(file, &grid);

		loadCells(file, &grid);

		statusMessage(LOADED);

		printGrid(&grid);

		enum lineSolveState res = ignitionSolving(&grid, 0);
		if (res == CONTRADICTION) {
			statusMessage(NO_SOLUTIONS);
			statusMessage(EXITING);
			exit(0);
		}

		unallocGrid(&grid);

		// This part will return found errors
		int err;
		if ((err = fclose(file)) != 0) {
			printf("%d : unable to close the file !\n", err);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
}