#ifndef NONOGRAMS_H_INCLUDED
#define NONOGRAMS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum cell {WHITE, BLACK, UNDEFINED, INCOMPATIBLE};

struct contraints {
	int contraint[100];
	int number;
};

/* Used to represent a simpler form of a line or a column of the grid
---------------------------------------------------------------------- */
struct bloc {
	int i_begin;
	int length;
};

struct grid {
	enum cell * cell;
	struct contraints line_contraints[100];
	struct contraints column_contraints[100];
	int line_number;
	int column_number;
	int added;
	int last_cell;
	enum cell last_added[1000];
	int last_added_cursor;
	int last_added_length;
};

enum lineSolveState { CONTRADICTION = -3, SOLVED = -2, NO_CHANGES = 0, ADDED};

#include "view.h"
#include "file_io.h"

int load_line_buffer(struct bloc * bloc_line_buffer_left, struct bloc * bloc_line_buffer_right, int line, struct grid * grid);
int load_column_buffer(struct bloc * bloc_column_buffer_left, struct bloc * bloc_column_buffer_right, int column, struct grid * grid);
int isCompatible(enum cell * array1, struct bloc * bloc2, int i_from, int length_array1, int length_bloc2);
void blocAnd(struct bloc * bloc1, struct bloc * bloc2, enum cell * cell, int length, int array_length);
int lineSolve(struct grid * grid);
int arrayToBlocs(enum cell * cell, struct bloc * bloc, int cell_number);
void blocsToArray(struct bloc * bloc, enum cell * cell, int bloc_number, int cell_number);
int moveRight(struct bloc * bloc, int length, int moves, int decalage, int limit);
int alignLeft(enum cell * source, struct bloc * bloc, int bloc_number, int grid_limit);
int alignRight(enum cell * source, struct bloc * bloc, int bloc_number, int grid_limit);
void blocAnd(struct bloc * bloc1, struct bloc * bloc2, enum cell * cell, int length, int array_length);
enum lineSolveState ignitionSolving(struct grid * g, int stopWhenStall);

#endif