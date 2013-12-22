# Picture

Picture is a [nonogram](https://en.wikipedia.org/wiki/Nonogram) solver in C.

[Report](doc/main.pdf)

## Usage

```
$ ./picture grids/hards/2-Cat
 5 5 2 1 8 9 9 8 7 8 9 10 13 6 4 6 6 5 6 6
   3 3 7                   2
     4 2

| | |B|B| | | | | | | | | | | | | | | | |	2
| |B|B| | | | | | | | | | | | | | | | | |	2
| |B| | | | | | | | | | | | | | | | | | |	1
| |B| | | | | | | | | | | | | | | | | | |	1
| |B| | | | | |B|B|B| | | | | | | | | | |	1 3
|B|B| | | | |B|B|B|B|B| | | | | | | | | |	2 5
|B| | | | |B|B|B|B|B|B|B| | | |B| | | |B|	1 7 1 1
|B| | | | |B|B|B|B|B|B|B|B| | |B|B| |B|B|	1 8 2 2
|B| | | |B|B|B|B|B|B|B|B|B| | |B|B|B|B|B|	1 9 5
|B|B| | |B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|	2 16
| |B| |B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|B|	1 17
| |B|B|B|B|B|B|B| |B|B|B|B|B|B|B|B|B|B|B|	7 11
| | |B|B|B|B|B| | | |B|B|B|B|B| |B|B|B| |	5 5 3
| | |B|B|B|B|B| | | |B|B|B|B| | | | | | |	5 4
| | | |B|B|B| | | | | |B|B|B| | | | | | |	3 3
| | | |B|B| | | | | | |B|B| | | | | | | |	2 2
| | |B|B| | | | | | | | |B| | | | | | | |	2 1
| | |B| | | | | | | | | |B| | | | | | | |	1 1
| | |B|B| | | | | | | | |B|B| | | | | | |	2 2
| | |B|B| | | | | | | | |B|B| | | | | | |	2 2
```

## Build

```
$ cd src
$ make
```
