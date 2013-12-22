#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "nonograms.h"
#include "view.h"

enum file_mode {READ, WRITE};

FILE * file_initialize(char * filename, enum file_mode mode);
void getSize (FILE * file, struct grid * grid);
void allocGrid(struct grid * grid);
void unallocGrid(struct grid * grid);
void loadContraints (FILE * file, struct grid * grid);
enum cell mask_file(char input);
void loadCells(FILE * file, struct grid * grid);

#endif