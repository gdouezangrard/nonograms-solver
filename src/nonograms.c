#include "nonograms.h"

int hypothesis = 0;

/* Return the line id of a cell
   ---------------------------- */
int getLine(int i_cell, struct grid * grid) {
	return (i_cell - i_cell%grid->column_number)/grid->column_number;
}

/* Return the column id of a cell
   ---------------------------- */
int getColumn(int i_cell, struct grid * grid) {
	return i_cell%grid->column_number;
}

/* Return the number of BLACK cells of a line
   --------------------------------------------- */
int definedLineCells(int line, struct grid * grid) {
	int count = 0;
	for (int i = 0 ; i < grid->column_number ; i++) {
		if (grid->cell[line * grid->column_number + i] == BLACK) {
			count++;
		}
	}
	return count;
}

/* Return the number of BLACK cells of a column
   --------------------------------------------- */
int definedColumnCells(int column, struct grid * grid) {
	int count = 0;
	for (int i = 0 ; i < grid->line_number ; i++) {
		if (grid->cell[i * grid->column_number + column] == BLACK) {
			count++;
		}
	}
	return count;
}

/* Return the number of BLACK cases to fill in the line
   ------------------------------------------------------- */
int lineContraintSum(int line, struct grid * grid) {
	int sum = 0;
	for (int i = 0 ; i < grid->line_contraints[line].number ; i++) {
		sum+=grid->line_contraints[line].contraint[i];
	}
	return sum;
}

/* Return the number of BLACK cases to fill in the column
   ------------------------------------------------------- */
int columnContraintSum(int column, struct grid * grid) {
	int sum = 0;
	for (int i = 0 ; i < grid->column_contraints[column].number ; i++) {
		sum+=grid->column_contraints[column].contraint[i];
	}
	return sum;
}

/* Return an integer which caraterize the criticity of a cell
   ------------------------------------------------------------- */
int criticite(int i_cell, struct grid * grid) {
	int line = getLine(i_cell, grid);
	int column = getColumn(i_cell, grid);
	if (lineContraintSum(line, grid) != 0 && columnContraintSum(column, grid) != 0) {
		return definedLineCells(line, grid) * 100 / lineContraintSum(line, grid) + definedColumnCells(column, grid) * 100 / columnContraintSum(column, grid);
	}
	else {
		return grid->column_number * grid->line_number * 100;
	}
}

/* Return the id of the most critical cell
   ------------------------------------------ */
int cellChoice(struct grid * grid, int avoid) {
	int critic = -1;
	int criticCell = -1;
	for (int i = 0 ; i < grid->line_number * grid->column_number ; i++) {
		if (grid->cell[i] == UNDEFINED && criticite(i, grid) > critic && i != avoid) {
			critic = criticite(i, grid);
			criticCell = i;
		}
	}
	return criticCell;
}

int neighboorDefined(struct grid * grid, int cell) {
	int number = 0;
	int line = getLine(cell, grid);
	int column = getColumn(cell, grid);
	if (line == 0 || grid->cell[(line - 1) * grid->column_number + column] != UNDEFINED) {
		number++;
	}
	if (line == grid->line_number - 1 || grid->cell[(line + 1) * grid->column_number + column] != UNDEFINED) {
		number++;
	}
	if (column == 0 || grid->cell[line * grid->column_number + column - 1] != UNDEFINED) {
		number++;
	}
	if (column == grid->column_number - 1 || grid->cell[line * grid->column_number + column + 1] != UNDEFINED) {
		number++;
	}
	return number;
}

int cellChoiceContradiction(struct grid * grid) {
	int found = 0;
	for (int i = 0 ; i < grid->line_number * grid->column_number ; i++) {
		if (grid->cell[i] == UNDEFINED) {
			if (neighboorDefined(grid, i) >= 2) {
				return i;
			}
		}
	}
	if (found == 0) {
		return cellChoice(grid, -1);
	}
}

/* Convert array representation to blocs representation of a cells array, length -> cell array size
   --------------------------------------------------------------------------------------------------- */
int arrayToBlocs(enum cell * cell, struct bloc * bloc, int cell_number) {
	int i = 0, fill = 0;
	while (i < cell_number) {
		if (cell[i] == BLACK) {
			bloc[fill].i_begin = i;
			bloc[fill].length = 0;
			while (i < cell_number && cell[i] == BLACK) {
				bloc[fill].length++;
				i++;
			}
			fill++;
		}
		else {
			i++;
		}
	}
	return fill;
}

/* Convert bloc representation to array representation of a bloc array, length -> bloc array size, array_length -> cell array size
   ---------------------------------------------------------------------------------------------------------------------------------- */
void blocsToArray(struct bloc * bloc, enum cell * cell, int bloc_number, int cell_number) {
	int i = 0, fill = 0;
	while (i < bloc_number && fill < cell_number) {
		while (fill < bloc[i].i_begin && fill < cell_number) {
			cell[fill] = WHITE;
			fill++;
		}
		while (fill < (bloc[i].i_begin + bloc[i].length) && fill < cell_number) {
			cell[fill] = BLACK;
			fill++;
		}
		i++;
	}
	if (cell_number > 0) {
		while (fill < cell_number) {
			cell[fill] = WHITE;
			fill++;
		}
	}
}

/* Check if the two blocs are compatible from i_from array indicator
   ------------------------------------------------------------------------- */
int isCompatible(enum cell * array1, struct bloc * bloc2, int i_from, int length_array1, int length_bloc2) {
	enum cell * array2 = malloc(length_array1 * sizeof(enum cell));
	blocsToArray(bloc2, array2, length_bloc2, length_array1);
	for (int i = i_from ; i < length_array1 ; i++) {
		if ((array1[i] == BLACK && array2[i] == WHITE) || (array1[i] == WHITE && array2[i] == BLACK)) {
			free(array2);
			return 0;
		}
	}
	free(array2);
	array2 = NULL;
	return 1;
}

/* Move specified bloc number by a specified number of cells (and return false if limit is reached)
   --------------------------------------------------------------------------------------------------- */
int moveLeft(struct bloc * bloc, int length, int moves, int decalage, int limit) {
	if ((bloc[length - 1].i_begin + decalage + bloc[length - 1].length) > limit || (bloc[0].i_begin + decalage) < 0) {
		return 0;
	}
	for (int i = length - moves ; i < length ; i++) {
		bloc[i].i_begin+=decalage;
	}
	return 1;
}

int moveRight(struct bloc * bloc, int length, int moves, int decalage, int limit) {
	if ((bloc[0].i_begin + decalage) < 0 || (bloc[length - 1].i_begin + decalage + bloc[length - 1].length) > limit) {
		return 0;
	}
	for (int i = 0 ; i < moves ; i++) {
		bloc[i].i_begin+=decalage;
	}
	return 1;
}

/* Align the blocs to the most-left alignment possible
   ------------------------------------------------------ */
int alignLeft(enum cell * source, struct bloc * bloc, int bloc_number, int grid_limit) {
	int moves = bloc_number;
	int valid = 0;
	while (valid != 1) {
		valid = 0;
		if (moves == 0) {
			if (isCompatible(source, bloc, 0, grid_limit, bloc_number)) {
				valid = 1;
			}
			else {
				moves++;
				if (moveLeft(bloc, bloc_number, moves, +1, grid_limit) == 0) {
					return -1;
				}
			}
		}
	    else if(isCompatible(source, bloc, 0, bloc[bloc_number - moves].i_begin + bloc[bloc_number - moves].length /*- 1 OU PAS !*/, bloc_number)) {
		    moves--;
	    }
	    else if (moveLeft(bloc, bloc_number, moves, +1, grid_limit) == 0) {
	    	for (int i = bloc_number - moves ; i < bloc_number ; i++) {
	    		bloc[i].i_begin = bloc[i-1].i_begin + bloc[i-1].length + 1;
	    	}
	    	if (moves != bloc_number) {
	    		moves++;
	    		if (moveLeft(bloc, bloc_number, moves, +1, grid_limit) == 0) {
	    			return -1;
	    		}
	    	}
	    	else {
	    		return -1;
	    	}
	    }
	}
	return 0;
}

int alignRight(enum cell * source, struct bloc * bloc, int bloc_number, int grid_limit) {
	int moves = bloc_number;
	int valid = 0;
	int res = 0;
	int retrograde = 0;
	while (valid != 1) {
	//printf("MOVES : %d\n", moves);
		valid = 0;
		if (moves == 0) {
			if (isCompatible(source, bloc, 0, grid_limit, bloc_number)) {
				valid = 1;
			}
			else {
				moves++;
				if (moveRight(bloc, bloc_number, moves, -1, grid_limit) == 0) {
					return -1;
				}
			}
		}
		else if (isCompatible(source, bloc, bloc[moves - 1].i_begin, grid_limit, bloc_number)) {
			moves--;
		}
		else if (moveRight(bloc, bloc_number, moves, -1, grid_limit) == 0) {
			for (int i = moves - 1 ; i >= 0 ; i--) {
				bloc[i].i_begin = bloc[i + 1].i_begin - bloc[i].length - 1;
			}
			if (moves != bloc_number) {
				moves++;
				if (moveRight(bloc, bloc_number, moves, -1, grid_limit) == 0) {
					return -1;
				}
			}
			else {
				return -1;
			}
		}
	}
	return 0;
}// ATTENTION : PAS DE RETOUR ICI, PEUT ETRE -1 DANS LA MEMOIRE...

/* Load the line in the most-left and most-right possible arrangement
   ------------------------------------------------------ */
int load_line_buffer(struct bloc * bloc_line_buffer_left, struct bloc * bloc_line_buffer_right, int line, struct grid * grid) {
    // Copy grid line
	enum cell * source = malloc(grid->column_number * sizeof(enum cell));
	enum cell * line_buffer_left = malloc(grid->column_number * sizeof(enum cell));
	enum cell * line_buffer_right = malloc((grid->column_number) * sizeof(enum cell));

	for (int i = 0 ; i < grid->column_number ; i++) {
		source[i] = grid->cell[line * grid->column_number + i];
	}

    //Load initial most-left and most-right LINE buffer
	int sum = lineContraintSum(line, grid);
	int i = 0, fill = 0;
	for (int i = 0 ; i < grid->column_number ; i++) {
		line_buffer_left[i] = WHITE;
		line_buffer_right[i] = WHITE;
	}
	for (int i = 0 ; i < grid->line_contraints[line].number ; i++) {
		for (int j = 0 ; j < grid->line_contraints[line].contraint[i] ; j++) {
			line_buffer_left[fill] = BLACK;
			fill++;
		}
		fill++;
	}

	for (int i = 0 ; i < grid->column_number - (grid->column_number - sum - (grid->line_contraints[line].number - 1)) ; i++) {
		line_buffer_right[i + grid->column_number - sum - (grid->line_contraints[line].number - 1)] = line_buffer_left[i];
	}

    //Align blocs in right extremes positions
	int l_left = arrayToBlocs(line_buffer_left, bloc_line_buffer_left, grid->column_number);
	int l_right = arrayToBlocs(line_buffer_right, bloc_line_buffer_right, grid->column_number);

	int resL = alignLeft(source, bloc_line_buffer_left, l_left, grid->column_number);
	int resR = alignRight(source, bloc_line_buffer_right, l_right, grid->column_number);

	if (resL == -1 || resR == -1) {
		return -1;
	}

	blocsToArray(bloc_line_buffer_left, line_buffer_left, l_left, grid->column_number);
	blocsToArray(bloc_line_buffer_right, line_buffer_right, l_right, grid->column_number);

	free(source);
	free(line_buffer_left);
	free(line_buffer_right);
	return l_left;
}

int load_column_buffer(struct bloc * bloc_column_buffer_left, struct bloc * bloc_column_buffer_right, int column, struct grid * grid) {
    // Copy grid line
	enum cell * source = malloc(grid->line_number * sizeof(enum cell));
	enum cell * column_buffer_left = malloc(grid->line_number * sizeof(enum cell));
	enum cell * column_buffer_right = malloc(grid->line_number * sizeof(enum cell));

	for (int i = 0 ; i < grid->line_number ; i++) {
		source[i] = grid->cell[i * grid->column_number + column];
	}

    //Load initial most-left and most-right LINE buffer    CORRESTION DE LA METHODE CI-DESSUS !!!!!!
	int sum = columnContraintSum(column, grid);
	int i = 0, fill = 0;
	for (int i = 0 ; i < grid->line_number ; i++) {
		column_buffer_left[i] = WHITE;
		column_buffer_right[i] = WHITE;
	}
	for (int i = 0 ; i < grid->column_contraints[column].number ; i++) {
		for (int j = 0 ; j < grid->column_contraints[column].contraint[i] ; j++) {
			column_buffer_left[fill] = BLACK;
			fill++;
		}
		fill++;
	}
	for (int i = 0 ; i < grid->line_number - (grid->line_number - sum - (grid->column_contraints[column].number - 1)) ; i++) {
		column_buffer_right[i + grid->line_number - sum - (grid->column_contraints[column].number - 1)] = column_buffer_left[i];
	}

    //Align blocs in right extremes positions
	int l_left = arrayToBlocs(column_buffer_left, bloc_column_buffer_left, grid->line_number);
	int l_right = arrayToBlocs(column_buffer_right, bloc_column_buffer_right, grid->line_number);

	int resL = alignLeft(source, bloc_column_buffer_left, l_left, grid->line_number);
	int resR = alignRight(source, bloc_column_buffer_right, l_right, grid->line_number);

	if (resL == -1 || resR == -1) {
		return -1;
	}

	blocsToArray(bloc_column_buffer_left, column_buffer_left, l_left, grid->line_number);
	blocsToArray(bloc_column_buffer_right, column_buffer_right, l_right, grid->line_number);

	free(source);
	free(column_buffer_left);
	free(column_buffer_right);
	return l_left;
  }

  int max(int a, int b) {
  	if (a < b) {
  		return b;
  	}
  	return a;
  }

  int min(int a, int b) {
  	if (a > b) {
  		return b;
  	}
  	return a;
  }

// And of two blocs
  void blocAnd(struct bloc * bloc1, struct bloc * bloc2, enum cell * cell, int length, int array_length) {
  	int fill = 0;
  	if (length != 0) {
  		for (int i = 0 ; i < array_length ; i++) {
  			cell[i] = UNDEFINED;
  		}

  		for (int i = 0 ; i < min(bloc1[0].i_begin, bloc2[0].i_begin) ; i++) {
  			cell[i] = WHITE;
  		}
  		for (int i = array_length - 1 ; i >= max(bloc1[length - 1].i_begin + bloc1[length - 1].length, bloc2[length - 1].i_begin + bloc2[length - 1].length) ; i--) {
  			cell[i] = WHITE;
  		}

  		int i = 1;
  		while (i < length) {
  			for (int j = max(bloc1[i-1].i_begin + bloc1[i-1].length, bloc2[i-1].i_begin + bloc2[i-1].length) ; j < min(bloc1[i].i_begin, bloc2[i].i_begin) ; j++) {
  				cell[j] = WHITE;
  			}
  			i++;
  		}
  	}
  	else {
  		for (int i = 0 ; i < array_length ; i++) {
  			cell[i] = WHITE;
  		}
  	}

    for (int i = 0 ; i < length ; i++) {
      	for (int j = max(bloc1[i].i_begin, bloc2[i].i_begin) ; j < min(bloc1[i].i_begin + bloc1[i].length, bloc2[i].i_begin + bloc2[i].length) ; j++) {
      		cell[j] = BLACK;
      	}
    }
  }

  int saveLine(enum cell * cell, int line, struct grid * grid) {
  	int added = 0;
  	for (int i = 0 ; i < grid->column_number ; i++) {
  		if ((cell[i] == BLACK || cell[i] == WHITE) && grid->cell[line * grid->column_number + i] != cell[i]) {
  			grid->cell[line * grid->column_number + i] = cell[i];
  			added++;
  		}
  	}
  	return added;
  }

  int saveColumn(enum cell * cell, int column, struct grid * grid) {
  	int added = 0;
  	for (int i = 0 ; i < grid->line_number ; i++) {
  		if ((cell[i] == BLACK || cell[i] == WHITE) && grid->cell[i * grid->column_number + column] != cell[i]) {
  			grid->cell[i * grid->column_number + column] = cell[i];
  			added++;
  		}
  	}
  	return added;
  }

/* Fill at least one case, if possible, a line and a column, based on the criticity
   ----------------------------------------------------------------------------------- */
  enum lineSolveState lineSolve(struct grid * grid) {
  	int i_cell = cellChoice(grid, SOLVED) - 1;
    if (i_cell == SOLVED) {// That means no cells to be completed left
    	return SOLVED;
    }

    int retrograde = -1;
    int added = 0;
    int save = i_cell + 1;

    while (added == 0) {
    	i_cell++;

	if (i_cell == grid->column_number * grid->line_number) {// VERIFY THE UPPER LIMIT (line - 1) * (column) ?
		i_cell = 0;
	}
	if (i_cell == save) {
		if (retrograde == 1) {
			return added;
		}
		retrograde++;
	}

	int line = getLine(i_cell, grid);
	int column = getColumn(i_cell, grid);

	enum cell * line_buffer = malloc(grid->column_number * sizeof(enum cell));
	enum cell * column_buffer = malloc(grid->line_number * sizeof(enum cell));

	struct bloc line_buffer_left[20], line_buffer_right[20], column_buffer_left[20], column_buffer_right[20];

	int l_length = load_line_buffer(line_buffer_left, line_buffer_right, line, grid);
	if (l_length == -1) {
		return CONTRADICTION;
	}

	blocAnd(line_buffer_left, line_buffer_right, line_buffer, l_length, grid->column_number);
	added += saveLine(line_buffer, line, grid);

	int c_length = load_column_buffer(column_buffer_left, column_buffer_right, column, grid);
	if (c_length == -1) {
		return CONTRADICTION;
	}

	blocAnd(column_buffer_left, column_buffer_right, column_buffer, c_length, grid->line_number);
	added += saveColumn(column_buffer, column, grid);

	free(line_buffer);
	free(column_buffer);
}
return added;
}

void gridcpy(struct grid *dest, struct grid *source) {
	dest->column_number = source->column_number;
	dest->line_number = source->line_number;
	dest->added = source->added;
	for (int i = 0 ; i < source->line_number ; i++) {
		dest->line_contraints[i].number = source->line_contraints[i].number;
		for (int j = 0 ; j < source->line_contraints[i].number ; j++) {
			dest->line_contraints[i].contraint[j] = source->line_contraints[i].contraint[j];
		}
	}
	for (int i = 0 ; i < source->column_number ; i++) {
		dest->column_contraints[i].number = source->column_contraints[i].number;
		for (int j = 0 ; j < source->column_contraints[i].number ; j++) {
			dest->column_contraints[i].contraint[j] = source->column_contraints[i].contraint[j];
		}
	}
	allocGrid(dest);
	for (int i = 0 ; i < source->line_number ; i++) {
		dest->line_contraints[i].number = source->line_contraints[i].number;
		for (int j = 0 ; j < source->column_number ; j++) {
			dest->cell[i * source->column_number + j] = source->cell[i * source->column_number + j];
		}
	}
}

enum lineSolveState ignitionSolving(struct grid * g, int stopWhenStall) {
	int res;
	if ((res = lineSolve(g)) == SOLVED) {
		statusMessage(SOLUTION);
		printGrid(g);
		printf("Hypothèses : %d\n", hypothesis);
		statusMessage(EXITING);
		exit(0);
	}
    else if (res > 0) {// That means we succeded in adding new cells !
    	g->added += res;
    	ignitionSolving(g, 0);
    }
    else if (res == NO_CHANGES && stopWhenStall == 0) {
    	hypothesis++;
    	struct grid * g2 = malloc(sizeof(*g));
    	gridcpy(g2, g);
    	res = cellChoiceContradiction(g);
    	g->cell[res] = WHITE;
    	g2->cell[res] = BLACK;
    	int res1 = ignitionSolving(g, 1);
    	int res2 = ignitionSolving(g2, 1);
    	if (res1 == CONTRADICTION || res1 < res2) {
    		if (res2 != CONTRADICTION) {
    			ignitionSolving(g2, 0);
    		}
    		else if (res1 != CONTRADICTION) {
    			ignitionSolving(g,0);
    		}
    		else {
    			return CONTRADICTION;
    		}
    	}
    	else {
    		if (res1 != CONTRADICTION) {
    			ignitionSolving(g, 0);
    		}
    		else if (res2 != CONTRADICTION) {
    			ignitionSolving(g2, 0);
    		}
    		else {
    			return CONTRADICTION;
    		}
    	}
    	return NO_CHANGES;
    }
    else if (stopWhenStall == 1) {
    	return g->added;
    }
    else {
    	unallocGrid(g);
    }
    return CONTRADICTION;
}