#include <stdio.h>
// GLOBAL KAYWORDS ARRAYS
#ifndef GLOBEL_ARRS_H
char *mnem[] = {"STOP", "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT", NULL};
char *registers[] = {"AREG", "BREG", "CREG", "DREG", NULL};
char *cc[] = {"LT", "LE", "EQ", "GT", "GE", "ANY", NULL};
char *ad[] = {"START", "END", "ORIGIN", "EQU", "LTORG", NULL};
char *ds[] = {"DS", "DC", NULL};
#endif