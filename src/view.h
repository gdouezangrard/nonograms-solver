#ifndef VIEW_H
#define VIEW_H

#include "nonograms.h"
#include <stdio.h>

enum status {
	STARTED, LOADED, LOAD_FAILED, SOLUTION, NO_SOLUTIONS, EXITING, INCORRECT_FILE
};

void printGrid(struct grid * grid);

void statusMessage(enum status status);

#endif